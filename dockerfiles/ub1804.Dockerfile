## -*- mode: dockerfile; indent-tabs-mode: nil; tab-width: 4 -*-
FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -q && apt-get install -yy \
    cmake \
    clang-7 \
    doxygen \
    git \
    gcc-8 \
    g++-8 \
    libcppunit-dev \
    libboost-all-dev \
    libhdf5-dev \
    libhdf5-serial-dev \
    libstdc++-8-dev \
    python-pip \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-7 1000
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 1000
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 1000
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 1000
RUN update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-8 1000

RUN mkdir /src
WORKDIR /src
