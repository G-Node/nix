// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <cstdio>
#include <nix.hpp>
#include <nix/NDArray.hpp>
#include <queue>
#include <random>
#include <type_traits>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stdexcept>


/* ************************************ */

class Stopwatch {

public:
    typedef std::chrono::high_resolution_clock::time_point time_point_t;
    typedef std::chrono::high_resolution_clock clock_t;

    Stopwatch() : t_start(clock_t::now()) { };

    ssize_t ms() {
        time_point_t t_end = clock_t::now();
        ssize_t count = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
        return count;
    }

private:
    time_point_t t_start;
};

/* ************************************ */

class RndGenBase {
public:
    RndGenBase() : rd(), rd_gen(rd()) { };

protected:
    std::random_device rd;
    std::mt19937 rd_gen;
};

template<typename T, typename Enable = void>
class RndGen { };

template<typename T>
class RndGen<T, typename std::enable_if<std::is_floating_point<T>::value >::type> : RndGenBase {
public:
    RndGen() : dis(-1024.0, +1024.0) { };

    T operator()(void) {
        return dis(rd_gen);
    };

private:
    std::uniform_real_distribution<T> dis;
};

template<typename T>
class RndGen<T, typename std::enable_if<std::is_integral<T>::value >::type> : RndGenBase {
public:
    RndGen() : dis(-1024, +1024) { };

    T operator()(void) {
        return dis(rd_gen);
    };

private:
    std::uniform_real_distribution<T> dis;
};

/* ************************************ */

template<typename T>
class BlockGenerator {
public:

    BlockGenerator(size_t blocksize, size_t bufsize) : blocksize(blocksize), bufsize(bufsize) { };

    std::vector<T> make_block() {
        std::vector<T> block(blocksize);
        std::generate(block.begin(), block.end(), std::ref(rnd_gen));
        return block;
    }

    std::vector<T> next_block() {
        std::unique_lock<std::mutex> lock(mtx);

        if (queue.empty()) {
            cnd.wait(lock, [this]{ return !queue.empty(); });
        }

        std::vector<T> x(std::move(queue.front()));
        queue.pop();
        cnd.notify_all();
        return x;
    }

    void worker_thread() {
        while (do_run) {
            std::unique_lock<std::mutex> lock(mtx);
            std::vector<T> block = make_block();

            if (queue.size() > bufsize) {
                //wait until there is room
                cnd.wait(lock, [this]{ return !do_run || queue.size() < bufsize;});
                if (!do_run) {
                    return;
                }
            }

            queue.push(std::move(block));
            cnd.notify_all();
        }
    }

    void start_worker() {
        workers.emplace_back(&BlockGenerator::worker_thread, this);
    }

    ~BlockGenerator() {
        do_run = false;
        cnd.notify_all();
        for (auto &w : workers) {
            w.join();
        }
    }

    double speed_test() {

        Stopwatch sw;

        size_t N = 100;
        size_t iterations = 0;

        do {
            Stopwatch inner;

            for (size_t i = 0; i < N; i++) {
                std::vector<double> block = next_block();
                iterations++;
            }

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while (sw.ms() < 1000);

        ssize_t count = sw.ms();
        return iterations * blocksize * sizeof(T) * (1000.0/count) / (1024.0*1024.0);
    }

private:
    bool do_run = true;
    size_t blocksize;
    size_t bufsize;
    RndGen<T> rnd_gen;
    std::mutex mtx;
    std::condition_variable cnd;
    std::queue<std::vector<T>> queue;
    std::vector<std::thread> workers;
};


class Benchmark {

public:
    Benchmark(nix::DataType dtype, const nix::NDSize &blocksize)
            : dtype(dtype), blocksize(blocksize) {

        sdim = find_single_dim();
        shape = blocksize;
        shape[sdim] = 0;

        make_name();
    };

    size_t find_single_dim() {
        size_t sdim;
        bool have_rdim = false;
        for (size_t i = 0; i < blocksize.size(); i ++) {
            if (blocksize[i] == 1) {
                sdim = i;
                have_rdim = true;
            }
        }

        if (!have_rdim) {
            throw std::runtime_error("Could not find singelton dimension");
        }
        return sdim;
    }


    void test_write_io(nix::Block block) {
        block_id = block.id();

        nix::DataArray da = block.createDataArray(my_name, "nix.test.da", dtype, shape);
        dset_id = da.id();

        switch (dtype) {

            case nix::DataType::Double:
                do_write_test<double>(da);
                break;

            default:
                throw std::runtime_error("Unsupported DataType!");
        }
    }

    void test_read_io(nix::File fd) {
        nix::Block block = fd.getBlock(block_id);
        nix::DataArray da = block.getDataArray(dset_id);

        nix::NDArray array(dtype, blocksize);
        nix::NDSize extend = da.dataExtent();
        size_t N = extend[sdim];

        nix::NDSize pos = {0, 0};

        ssize_t ms = time_it([this, &da, &N, &pos, &array] {
            for(size_t i = 0; i < N; i++) {
                da.getData(dtype, array.data(), blocksize, pos);
                pos[sdim] += 1;
            }
        });

        speed_read = calc_speed_mbs(ms, N);
    }

    void make_name() {
        std::stringstream s;

        s << dtype << "@{ ";
        for (auto x : blocksize) {
            s << x << " ";
        }
        s << "}";

        my_name = s.str();
    }

    void report() {
        std::cout << my_name << ": "
                << "G: " << speed_generator << " MB/s, "
                << "W: " << speed_write << " MB/s, "
                << "R: " << speed_read << " MB/s" << std::endl;
    }

    template<typename F>
    ssize_t time_it(F func) {
        Stopwatch watch;
        func();
        return watch.ms();
    }

private:
    template<typename T>
    void do_write_test(nix::DataArray da) {
        size_t nelms = blocksize.nelms();
        BlockGenerator<T> generator(nelms, 10);
        generator.start_worker();
        speed_generator = generator.speed_test();


        size_t N = 100;
        size_t iterations = 0;

        nix::NDSize pos = {0, 0};
        Stopwatch sw;
        ssize_t ms = 0;
        do {
            Stopwatch inner;

            for (size_t i = 0; i < N; i++) {
                std::vector<T> block = generator.next_block();
                da.dataExtent(blocksize + pos);
                da.setData(dtype, block.data(), blocksize, pos);
                pos[sdim] += 1;
                iterations++;
            }

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while ((ms = sw.ms()) < 3*1000);
        speed_write = calc_speed_mbs(ms, iterations);
    }

    double calc_speed_mbs(ssize_t ms, size_t iterations) {
        return iterations * blocksize.nelms() * nix::data_type_to_size(dtype) * (1000.0/ms) / (1024.0*1024.0);
    }

private:
    nix::DataType dtype;
    nix::NDSize   blocksize;

    size_t        sdim;
    nix::NDSize   shape;

    std::string   my_name;

    std::string   dset_id;
    std::string   block_id;

    double        speed_write;
    double        speed_read;
    double        speed_generator;
};

/* ************************************ */

static std::vector<Benchmark> make_benchmarks() {
    std::vector<Benchmark> marks;

    //TODO: auto-generate
    marks.emplace_back(nix::DataType::Double, nix::NDSize{2048, 1});
    marks.emplace_back(nix::DataType::Double, nix::NDSize{1, 2048});

    return marks;
}

int main(int argc, char **argv)
{
    nix::File fd = nix::File::open("iospeed.h5", nix::FileMode::Overwrite);
    nix::Block block = fd.createBlock("speed", "nix.test");

    std::vector<Benchmark> marks = make_benchmarks();

    std::cout << "Performing write tests..." << std::endl;
    for (Benchmark &mark : marks) {
        mark.test_write_io(block);
    }

    std::cout << "Performing read tests..." << std::endl;
    for (Benchmark &mark : marks) {
        mark.test_read_io(fd);
    }
    std::cout << " === Reports ===" << std::endl;
    for (Benchmark &mark : marks) {
        mark.report();
    }

    return 0;
}
