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
#include <cstdio>
#include <string>
#include <cstdint>
#include <utility>

/* ************************************ */
namespace nix {

template<typename Func, typename... Args>
auto data_type_dispatch(nix::DataType dtype, Func &&F, Args&&... args)
    -> decltype(std::forward<Func>(F)(double(), std::forward<Args>(args)...))
{
    switch (dtype) {

        case DataType::Double:
            return std::forward<Func>(F)(double(), std::forward<Args>(args)...);
            break;

        case DataType::Int8:
            return std::forward<Func>(F)(int8_t(), std::forward<Args>(args)...);
            break;

        case DataType::Int16:
            return std::forward<Func>(F)(int16_t(), std::forward<Args>(args)...);
            break;

        default:
            throw std::invalid_argument("Unkown DataType");
    }

}


}

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
    RndGen() : dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) { };

    T operator()(void) {
        return dis(rd_gen);
    };

private:
    std::uniform_int_distribution<T> dis;
};

/* ************************************ */

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



class BlockGenerator : public RndGenBase {
public:

    BlockGenerator(const Config &cfg, size_t bufsize)
            : blocksize(cfg.size()), dtype(cfg.dtype()), uni_dis(0, bufsize-1) {
        for(size_t i = 0; i < bufsize; i++) {
            blocks.push_back(make_block());
        }
    };

    struct BlockMaker {

    public:
        template<typename U>
        nix::NDArray operator()(U tag, const nix::NDSize &size) {
            RndGen<U> rnd_gen;

            nix::NDArray data(nix::to_data_type<U>::value, size);
            for(size_t i = 0; i < data.num_elements(); i++) {
                data.set(i, rnd_gen());
            }

            return data;
        };
    };

    nix::NDArray make_block() {
        BlockMaker maker;
        return nix::data_type_dispatch(dtype, maker, std::ref(blocksize));
    }

    nix::NDArray next_block() {
        size_t index = uni_dis(rd_gen);
        nix::NDArray block = blocks[index];
        return block;
    }

    double speed_test(size_t &iterations) {

        Stopwatch sw;

        size_t N = 100;
        iterations = 0;

        do {
            Stopwatch inner;

            for (size_t i = 0; i < N; i++) {
                nix::NDArray data = next_block();
                assert(data.size() == blocksize);
                iterations++;
            }

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while (sw.ms() < 1000);

        ssize_t count = sw.ms();
        return count;
    }

private:
    nix::NDSize blocksize;
    nix::DataType dtype;
    std::uniform_int_distribution<size_t> uni_dis;
    std::vector<nix::NDArray> blocks;
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

    virtual double speed_in_mbs() {
        return count * config.size().nelms() * nix::data_type_to_size(config.dtype()) *
                (1000.0/millis) / (1024 * 1024);
    }

    virtual double speed_in_nps() {
        return count * config.size().nelms() * (1000.0/millis);
    }

    template<typename F>
    ssize_t time_it(F func) {
        Stopwatch watch;
        func();
        return watch.ms();
    }


    virtual void run(nix::Block block) = 0;
    virtual std::string id() = 0;

protected:
    const Config config;
    size_t       count;
    double       millis;
};

class GeneratorBenchmark : public Benchmark {
public:
    GeneratorBenchmark(const Config &cfg)
            : Benchmark(cfg) {

    };

    void run(nix::Block block) override {
        BlockGenerator generator(config, 10);
        this->millis = generator.speed_test(this->count);
    }

    std::string id() override {
        return "G";
    }
};


class WriteBenchmark : public Benchmark {

public:
    WriteBenchmark(const Config &cfg)
            : Benchmark(cfg) {
    };

    void run(nix::Block block) override {
        nix::DataArray da = openDataArray(block);

        BlockGenerator generator(config, 10);

        size_t N = 100;
        size_t iterations = 0;

        nix::NDSize pos = {0, 0};
        Stopwatch sw;
        ssize_t ms = 0;
        do {
            Stopwatch inner;

            for (size_t i = 0; i < N; i++) {
                nix::NDArray block = generator.next_block();
                da.dataExtent(config.size() + pos);
                da.setData(config.dtype(), block.data(), config.size(), pos);
                pos[config.singleton_dimension()] += 1;
                iterations++;
            }

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while ((ms = sw.ms()) < 3*1000);

        this->count = iterations;
        this->millis = ms;
    }

    std::string id() override {
        return "W";
    }
};


class ReadBenchmark : public Benchmark {

public:
    ReadBenchmark(const Config &cfg)
            : Benchmark(cfg) {
    };

    void test_read_io(nix::DataArray da) {

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

        this->count = N;
        this->millis = ms;
    }

    void run(nix::Block block) override {
        nix::DataArray da = openDataArray(block);
        test_read_io(da);
    }

    std::string id() override {
        return "R";
    }
};


class ReadPolyBenchmark : public ReadBenchmark {

public:
    ReadPolyBenchmark(const Config &cfg)
            : ReadBenchmark(cfg) {
    };

    void run(nix::Block block) override {
        nix::DataArray da = openDataArray(block);
        da.polynomCoefficients({3, 4, 5, 6});
        test_read_io(da);
    }

    std::string id() override {
        return "P";
    }
};

class DiskBenchmark : public Benchmark {
public:
    DiskBenchmark(const Config &cfg)
            : Benchmark(cfg), fd(nullptr) {

    };

    void openFile(bool readOnly) {
        std::string path = config.name() + "io.raw";
        const char *mode = readOnly ? "rb" : "wb";
        fd = fopen(path.c_str(), mode);
        setvbuf(fd, nullptr, _IONBF, 0);
    }

    void read_test() {
        const size_t nelms = config.size().nelms();
        const size_t elmsize = nix::data_type_to_size(config.dtype());
        const size_t nbytes = nelms * elmsize;
        std::vector<char> buffer(nbytes, 0);

        Stopwatch sw;
        ssize_t n;
        size_t count = 0;

        while ((n = fread(buffer.data(), nbytes, 1, fd)) > 0) {
            count += n;
        }

        this->millis = sw.ms();
        this->count = count;
    }

    void write_test() {
        BlockGenerator generator(config, 10);

        size_t N = 100;
        size_t iterations = 0;

        nix::NDSize pos = {0, 0};
        Stopwatch sw;
        ssize_t ms = 0;
        const size_t elm_size = nix::data_type_to_size(config.dtype());
        do {
            Stopwatch inner;

            for (size_t i = 0; i < N; i++) {
                nix::NDArray block = generator.next_block();

                ssize_t nwritten = fwrite(block.data(), elm_size, block.num_elements(), fd);

                if (nwritten != block.num_elements()) {
                    throw std::runtime_error("Output error in disk write test.");
                }

                iterations++;
            }

            fflush(fd);

            if (inner.ms() < 100) {
                N *= 2;
            }

        } while ((ms = sw.ms()) < 3 * 1000);

        count = iterations;
        millis = ms;
    }

    void closeFile() {
        if (fd) {
            fclose(fd);
            fd = nullptr;
        }
    }


    ~DiskBenchmark() {
        closeFile();
    }

private:
    std::FILE *fd;
};

class DiskReadBenchmark : public DiskBenchmark {
public:
    DiskReadBenchmark(const Config &cfg)
            : DiskBenchmark(cfg) {

    };

    void run(nix::Block b) override {
        openFile(true);
        read_test();
        closeFile();
    }

    std::string id() override {
        return "I";
    }
};

class DiskWriteBenchmark : public DiskBenchmark {
public:
    DiskWriteBenchmark(const Config &cfg)
            : DiskBenchmark(cfg) {

    };

    void run(nix::Block b) override {
        openFile(false);
        write_test();
        closeFile();
    }

    std::string id() override {
        return "O";
    }
};

/* ************************************ */

static std::vector<Config> make_configs() {

    std::vector<Config> configs;

    configs.emplace_back(nix::DataType::Double, nix::NDSize{2048, 1});
    configs.emplace_back(nix::DataType::Double, nix::NDSize{1, 2048});

    return configs;
}

int main(int argc, char **argv)
{
    nix::File fd = nix::File::open("iospeed.h5", nix::FileMode::Overwrite);
    nix::Block block = fd.createBlock("speed", "nix.test");

    std::vector<Config> configs = make_configs();
    std::vector<Benchmark *> marks;

    std::cout << "Performing generators tests..." << std::endl;
    for (const Config &cfg : configs) {
        GeneratorBenchmark *benchmark = new GeneratorBenchmark(cfg);
        benchmark->run(block);
        marks.push_back(benchmark);
    }

    std::cout << "Performing disk IO tests..." << std::endl;
    for (const Config &cfg : configs) {
        DiskWriteBenchmark *b = new DiskWriteBenchmark(cfg);
        b->run(block);
        marks.push_back(b);
    }

    std::cout << "Performing read tests..." << std::endl;
    for (const Config &cfg : configs) {
        DiskReadBenchmark *b = new DiskReadBenchmark(cfg);
        b->run(block);
        marks.push_back(b);
    }

    std::cout << "Performing write tests..." << std::endl;
    for (const Config &cfg : configs) {
        WriteBenchmark *benchmark = new WriteBenchmark(cfg);
        benchmark->run(block);
        marks.push_back(benchmark);
    }

    std::cout << "Performing read tests..." << std::endl;
    for (const Config &cfg : configs) {
        ReadBenchmark *benchmark = new ReadBenchmark(cfg);
        benchmark->run(block);
        marks.push_back(benchmark);
    }

    std::cout << "Performing read (poly) tests..." << std::endl;
    for (const Config &cfg : configs) {
        ReadPolyBenchmark *benchmark = new ReadPolyBenchmark(cfg);
        benchmark->run(block);
        marks.push_back(benchmark);
    }

    std::cout << " === Reports ===" << std::endl;
    std::cout.precision(5);
    std::cout.unsetf (std::ios::floatfield);
    for (Benchmark *mark : marks) {
        std::cout << mark->cfg().name() << ", " << mark->id() << ", "
                << mark->speed_in_mbs() << " MB/s, "
                << mark->speed_in_nps() << " N/s" << std::endl;
        delete mark;
    }


    return 0;
}
