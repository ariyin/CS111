# Hash Hash Hash
Implements a concurrent hash table.

## Building
```shell
make
```

## Running
```shell
./hash-table-tester -t [THREADS] -s [ENTRIES]
```
- THREADS: number of threads (default 4)
- ENTRIES: number of hash table entries per thread (default 25,000)

## First Implementation
In the `hash_table_v1_add_entry` function, I added TODO

### Performance
```shell
TODO how to run and results
```
Version 1 is a little slower/faster than the base version. As TODO

## Second Implementation
In the `hash_table_v2_add_entry` function, I TODO

### Performance
```shell
TODO how to run and results
```

TODO more results, speedup measurement, and analysis on v2

## Cleaning up
```shell
make clean
```