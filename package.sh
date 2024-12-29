#!/bin/bash

BUILD_WITH_MUSL=$1

# 创建构建目录
mkdir -p build
pushd build

if [ "$BUILD_WITH_MUSL" == "true" ]; then
    echo "Building with musl"
    # 设置安装前缀
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/musl \
        -DCMAKE_C_COMPILER=musl-gcc \
        ..
else
    echo "Building with glibc"
    # 设置安装前缀
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_C_COMPILER=gcc-12 \
        ..
fi

popd
cmake --build build

# 生成安装包
pushd build
cpack -G DEB -C Release
popd

echo "Package generated in build directory"