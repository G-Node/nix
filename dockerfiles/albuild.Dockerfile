FROM archlinux/base

RUN pacman -Syu --noconfirm base-devel cmake clang hdf5 boost cppunit
COPY . /nixio
RUN mkdir -p /nixio/build
WORKDIR /nixio/build
ENTRYPOINT ["/usr/bin/bash"]
