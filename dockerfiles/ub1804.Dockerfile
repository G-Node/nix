## -*- mode: dockerfile; indent-tabs-mode: nil; tab-width: 4 -*-
FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -q && apt-get install -yy \
    cmake=3.10.* \ 
    clang-7=1:7-3~ubuntu0.* \ 
    doxygen=1.8.* \ 
    git=1:2.17.* \ 
    gcc-8=8.4.* \ 
    g++-8=8.4.* \ 
    libcppunit-dev=1.14.* \ 
    libboost-all-dev=1.65.* \ 
    libhdf5-dev=1.10.* \ 
    libhdf5-serial-dev=1.10.* \ 
    libstdc++-8-dev=8.4.* \ 
    python-pip=9.0.* \ 
    valgrind=1:3.13.* \ 
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-7 1000
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 1000
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 1000
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 1000
RUN update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-8 1000

RUN mkdir /src
WORKDIR /src
