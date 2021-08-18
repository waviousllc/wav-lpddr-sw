FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y \
&& apt full-upgrade -y \
&& apt-get install -y \
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
&& make install \
&& cd / \
&& rm -rf /usr/src/python3.7.6


# Clone RISC-V GNU Toolchain and build
WORKDIR /usr/src/riscv
RUN mkdir -p /opt/riscv \
&& git clone --recursive https://github.com/riscv/riscv-gnu-toolchain \
&& cd /usr/src/riscv/riscv-gnu-toolchain \
&& ./configure --prefix=/opt/riscv-linux \
&& make linux \
&& ./configure --prefix=/opt/riscv --enable-multilib \
&& make newlib \
&& cd / \
&& rm -rf /usr/src/riscv

ENV PATH="/opt/riscv/bin:/opt/riscv-linux/bin:${PATH}"

RUN apt-get install -y \
cmake \
gdisk \
cpio \
unzip \
rsync \
libncurses-dev \
&& pip3 install pyyaml

# Make 4.3
WORKDIR /usr/src/make

RUN wget -O make-4.3.tar.gz https://ftp.gnu.org/gnu/make/make-4.3.tar.gz \
&& tar xvf make-4.3.tar.gz \
&& cd make-4.3 \
&& ./configure --prefix=/usr/local \
&& make \
&& make install \
&& cd / \
&& rm -rf /usr/src/cmake

# CMAKE 3.14
WORKDIR /usr/src/cmake

RUN wget -O cmake-3.14.5.tgz https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5.tar.gz \
&& tar xzvf cmake-3.14.5.tgz \
&& cd /usr/src/cmake/cmake-3.14.5/ \
&& cmake . \
&& make \
&& make install \
&& cd / \
&& rm -rf /usr/src/cmake

WORKDIR /usr/local/build/src/

CMD ["sh", "-c", "./configure; cd build; make -j8"]
