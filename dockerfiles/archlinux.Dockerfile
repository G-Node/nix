FROM archlinux/base

RUN pacman -Syu --noconfirm base-devel cmake clang hdf5 boost cppunit
RUN mkdir /src
WORKDIR /src
