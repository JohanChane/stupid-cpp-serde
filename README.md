# Stupid C++ Serde

## Project Purpose

After studying [libnop](https://github.com/google/libnop), I created a simple demo to implement serialization and deserialization in C++. I hope this helps those who want to understand how C++ handles serialization and deserialization.

## Build & Install

Build:

```sh
mkdir build && cd build
cmake ..
# OR
#cmake .. -DSCS_BUILD_TESTS=ON -DSCS_BUILD_EXAMPLES=ON
make -j8
```

Install:

```sh
cmake .. -DSCS_BUILD_TESTS=OFF -DSCS_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
cmake --install .  # Install to system
```

## Binary Encoding for Serialization

See [ref](./doc/serde.md)

## Documentation

See [ref](./doc)