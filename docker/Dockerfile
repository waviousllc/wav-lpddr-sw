FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y
RUN apt full-upgrade -y
RUN apt-get install -y \
autoconf \
automake \
autotools-dev \
curl \
libmpc-dev \
libmpfr-dev \
libgmp-dev \
gawk \
build-essential \
bison \
flex \
texinfo \
gperf \
libtool \
patchutils \
bc \
zlib1g-dev \
libexpat-dev \
expect \
libssl-dev \
wget \
vim \
git

# Python3
WORKDIR /usr/src/python3.7.6

RUN wget https://www.python.org/ftp/python/3.7.6/Python-3.7.6.tgz \
&& tar -xvf Python-3.7.6.tgz \
&& cd Python-3.7.6 \
&& ./configure --enable-optimizations \
&& make -j8 \
&& make install

# Clone RISC-V GNU Toolcahin (don't build)
RUN mkdir -p /opt/riscv

WORKDIR /usr/src/riscv

RUN git clone --recursive https://github.com/riscv/riscv-gnu-toolchain

WORKDIR /usr/src/riscv/riscv-gnu-toolchain

RUN ./configure --prefix=/opt/riscv --with-arch=rv32imc --with-abi=ilp32 \
&& make newlib

ENV PATH="/opt/riscv/bin:/opt/riscv-linux/bin:${PATH}"

WORKDIR /usr/local/build/src/

RUN apt-get install -y \
g++ \
cmake \
libboost-dev \
libglib2.0-dev \
libpixman-1-dev \
swig \
libcap-dev \
libattr1-dev \
cpio \
unzip

WORKDIR /usr/src/cmake

# CMAKE 3.14
RUN wget -O cmake-3.14.5.tgz https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5.tar.gz \
&& tar xzvf cmake-3.14.5.tgz

RUN cd /usr/src/cmake/cmake-3.14.5/ \
&& cmake . \
&& make \
&& make install

WORKDIR /usr/local/build/src
