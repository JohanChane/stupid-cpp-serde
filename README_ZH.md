# Stupid C++ Serde

## 编写该项目的目的

在学习 [libnop](https://github.com/google/libnop) 之后, 写一个简单的 demo 来实现 C++ 的序列化与反序列化。希望能帮助想了解 C++ 是如何序列化与反序列化的人。

## build & install

build:

```sh
mkdir build && cd build
cmake ..
# OR
#cmake .. -DSCS_BUILD_TESTS=ON -DSCS_BUILD_EXAMPLES=ON
make -j8
```

install:

```sh
cmake .. -DSCS_BUILD_TESTS=OFF -DSCS_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
cmake --install .  # 安装到系统
```

## 序列化的二进制编码

See [ref](./doc/serde_zh.md)

## Doc

See [ref](./doc)