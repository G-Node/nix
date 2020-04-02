FROM archlinux/base

RUN pacman -Syu --noconfirm base-devel cmake clang hdf5 boost cppunit yaml-cpp doxygen git

RUN useradd builduser -m

# install perl-perlio-gzip from aur (dependency for lcov-git)
USER builduser
RUN git clone --depth=1 https://aur.archlinux.org/perl-perlio-gzip.git /tmp/perl-perlio-gzip
WORKDIR /tmp/perl-perlio-gzip
RUN makepkg
USER root
WORKDIR /tmp/perl-perlio-gzip
RUN pacman -U --noconfirm perl-perlio-gzip-0.20-1-x86_64.pkg.tar.xz
RUN pacman -S --noconfirm perl-json

# install lcov-git from aur (dev patches required for gcc compat)
USER builduser
RUN git clone --depth=1 https://aur.archlinux.org/lcov-git.git /tmp/lcov
WORKDIR /tmp/lcov
RUN makepkg
USER root
WORKDIR /tmp/lcov
RUN pacman -U --noconfirm lcov-git-r327.40580cd-1-any.pkg.tar.xz

RUN mkdir /src
WORKDIR /src
