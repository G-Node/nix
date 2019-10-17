FROM archlinux/base

RUN pacman -Syu --noconfirm base-devel cmake clang hdf5 boost cppunit yaml-cpp doxygen
RUN mkdir /src
WORKDIR /src
