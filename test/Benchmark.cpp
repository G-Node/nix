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

class Config {

public:
    Config(nix::DataType data_type, const nix::NDSize &blocksize)
            : data_type(data_type), block_size(blocksize) {

        sdim = find_single_dim();
        shape = blocksize;
        shape[sdim] = 0;

        make_name();
    };

    size_t find_single_dim() {
        size_t sdim;
        bool have_rdim = false;
        for (size_t i = 0; i < block_size.size(); i ++) {
            if (block_size[i] == 1) {
                sdim = i;
                have_rdim = true;
            }
        }

        if (!have_rdim) {
            throw std::runtime_error("Could not find singelton dimension");
        }
        return sdim;
    }

    nix::DataType dtype() const { return data_type; }
    const nix::NDSize& size() const { return block_size; }
    const nix::NDSize& extend() const { return shape; }
    size_t singleton_dimension() const { return sdim; }
    const std::string & name() const { return my_name; };


private:
    void make_name() {
        std::stringstream s;

        s << data_type << "@{ ";
        for (auto x : block_size) {
            s << x << " ";
        }
        s << "}";

        my_name = s.str();
    }

private:
    const nix::DataType data_type;
    const nix::NDSize block_size;

    size_t        sdim;
    nix::NDSize   shape;

    std::string   my_name;
};


class Benchmark {
public:
    Benchmark(const Config &cfg) : config(cfg) { }
    const Config & cfg() const { return config; }

    nix::DataArray openDataArray(nix::Block block) const {
        const std::string &cfg_name = config.name();
        std::vector<nix::DataArray> v = block.dataArrays(nix::util::NameFilter<nix::DataArray>(cfg_name));
        if (v.empty()) {
            return block.createDataArray(cfg_name, "nix.test.da", config.dtype(), config.extend());
        } else {
            return v[0];
        }
    }

    virtual ~Benchmark() { }

    double speed_in_mbs() {
        return count * config.size().nelms() * nix::data_type_to_size(config.dtype()) *
                (1000.0/millis) / (1024 * 1024);
    }

    template<typename F>
    ssize_t time_it(F func) {
        Stopwatch watch;
        func();
        return watch.ms();
    }


protected:
    const Config config;
    size_t       count;
    double       millis;
};

class IOBenchmark : Benchmark {

public:
    IOBenchmark(const Config &cfg)
            : Benchmark(cfg) {
    };

    void test_write_io(nix::Block block) {
        block_id = block.id();

        nix::DataArray da = openDataArray(block);
        dset_id = da.id();

        switch (config.dtype()) {

            case nix::DataType::Double:
                do_write_test<double>(da);
                break;

            default:
                throw std::runtime_error("Unsupported DataType!");
        }
    }

    void test_read_io(nix::Block block) {
        nix::DataArray da = openDataArray(block);
        speed_read = test_read_io(da);
    }

    double test_read_io(nix::DataArray da) {

        nix::NDArray array(config.dtype(), config.size());
        nix::NDSize extend = da.dataExtent();
        size_t N = extend[config.singleton_dimension()];

        nix::NDSize pos = {0, 0};

        ssize_t ms = time_it([this, &da, &N, &pos, &array] {
            for(size_t i = 0; i < N; i++) {
                da.getData(config.dtype(), array.data(), config.size(), pos);
                pos[config.singleton_dimension()] += 1;
            }
        });

        return calc_speed_mbs(ms, N);
    }

    void test_read_io_polynom(nix::Block block) {
        nix::DataArray da = openDataArray(block);

        da.polynomCoefficients({3, 4, 5, 6});

        speed_read_poly = test_read_io(da);
    }


    void report() {
        std::cout << config.name() << ": "
                << "G: " << speed_generator << " MB/s, "
                << "W: " << speed_write << " MB/s, "
                << "R: " << speed_read << " MB/s, "
                << "P: " << speed_read_poly << " MB/s" << std::endl;
    }

private:
    template<typename T>
    void do_write_test(nix::DataArray da) {
        size_t nelms = config.size().nelms();
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
                da.dataExtent(config.size() + pos);
                da.setData(config.dtype(), block.data(), config.size(), pos);
                pos[config.singleton_dimension()] += 1;
                iterations++;
            }

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while ((ms = sw.ms()) < 3*1000);
        speed_write = calc_speed_mbs(ms, iterations);
    }

    double calc_speed_mbs(ssize_t ms, size_t iterations) {
        return iterations * config.size().nelms() * nix::data_type_to_size(config.dtype()) * (1000.0/ms) /
                (1024.0*1024.0);
    }

private:
    std::string   dset_id;
    std::string   block_id;

    double        speed_write;
    double        speed_read;
    double        speed_generator;
    double        speed_read_poly;
};

/* ************************************ */

static std::vector<IOBenchmark> make_benchmarks() {
    std::vector<IOBenchmark> marks;

    //TODO: auto-generate
    marks.emplace_back(Config(nix::DataType::Double, nix::NDSize{2048, 1}));
    marks.emplace_back(Config(nix::DataType::Double, nix::NDSize{1, 2048}));

    return marks;
}

int main(int argc, char **argv)
{
    nix::File fd = nix::File::open("iospeed.h5", nix::FileMode::Overwrite);
    nix::Block block = fd.createBlock("speed", "nix.test");

    std::vector<IOBenchmark> marks = make_benchmarks();

    std::cout << "Performing write tests..." << std::endl;
    for (IOBenchmark &mark : marks) {
        mark.test_write_io(block);
    }

    std::cout << "Performing read tests..." << std::endl;
    for (IOBenchmark &mark : marks) {
        mark.test_read_io(block);
    }

    std::cout << "Performing read (poly) tests..." << std::endl;
    for (IOBenchmark &mark : marks) {
        mark.test_read_io_polynom(block);
    }

    std::cout << " === Reports ===" << std::endl;
    for (IOBenchmark &mark : marks) {
        mark.report();
    }

    return 0;
}
