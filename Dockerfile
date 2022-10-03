FROM ubuntu:22.04
# install requirements
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    autopoint \
    bash \
    bison \
    bzip2 \
    flex \
    g++ \
    g++-multilib \
    gettext \
    git \
    gperf \
    intltool \
    libc6-dev-i386 \
    libgdk-pixbuf2.0-dev \
    libltdl-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    python3 \
    python3-mako \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils

RUN ln -s /usr/bin/python3 /usr/bin/python
# clone mxe to build directory
RUN mkdir /build
WORKDIR /build
RUN git clone https://github.com/mxe/mxe.git

# Build mxe with the necesary packages
#RUN cd mxe && make download-qtbase download-qtmultimedia --jobs=2
RUN cd mxe && make download-qt5 --jobs=4

#RUN cd mxe && make qtbase MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=
#RUN cd mxe && make qtmultimedia MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=
RUN cd mxe && make qt5 MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=

# Add mxe to path
ENV PATH /build/mxe/usr/bin:$PATH


# Add alias for qmake
RUN ln -s /build/mxe/usr/bin/x86_64-w64-mingw32.static-qmake-qt5 /build/mxe/usr/bin/qmake

# COPY source files to image
RUN mkdir /src
COPY . /src

# Set workdir to /src for ease
WORKDIR /src

#Build project
RUN qmake
RUN make
