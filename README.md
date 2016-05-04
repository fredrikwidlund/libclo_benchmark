# libclo_benchmark

Very simple JSON print benchmark

## Description

Measures and compares JSON print statements using three different implementations

## Requirements

- libclo
- jansson
- cJSON (included)

## Build

```
git clone https://github.com/fredrikwidlund/libclo_benchmark.git
cd libclo_benchmark
./autogen.sh
./configure
make
```

## Run

```
$ ./benchmark 10000000 > /dev/null
          libclo:          393.646 ns
         jansson:           2406.5 ns
           cJSON:          1524.68 ns
```
