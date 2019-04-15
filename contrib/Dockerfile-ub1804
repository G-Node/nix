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
    libyaml-cpp-dev \
    libstdc++-8-dev \
    python-pip \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

RUN pip install cpp-coveralls && \
    rm -rf /tmp/pip && \
    rm -rf /root/.cache

RUN mkdir /src
WORKDIR /src
