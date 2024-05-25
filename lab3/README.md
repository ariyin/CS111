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
- `THREADS`: Number of threads (default 4)
- `ENTRIES`: Number of hash table entries per thread (default 25,000)

## First Implementation
In the `struct hash_table_v1` function, I added a mutex, resulting in one mutex per hash table. 

In the `hash_table_v1_create` function, the mutex for the hash table is initialized. 

In the `hash_table_v1_add_entry` function, I locked the mutex in the beginning of the function and unlocked it at the end of the function, potentially blocking all other threads from adding entries to any linked list in the hash table, even if they are working on different lists.

In the `hash_table_v1_destroy` function, the mutex for the hash table is destroyed.

### Performance
```shell
$ ./hash-table-tester -t 4 -s 50000

Results:

Hash table base: 206,681 usec
    - 0 missing
Hash table v1: 495,392 usec
    - 0 missing

Hash table base: 202,339 usec
    - 0 missing
Hash table v1: 490,429 usec
    - 0 missing

Hash table base: 210,524 usec
    - 0 missing
Hash table v1: 505,152 usec
    - 0 missing
```
Version 1 is slower than the base version. This is because the entire hash table is locked (there's no longer concurrency) and we need to lock/unlock the mutexes (adding overhead).

## Second Implementation
In the `hash_table_entry` function, I added a mutex, resulting in `HASH_TABLE_CAPACITY` mutexes per hash table. 

In the `hash_table_v2_create` function, the mutex for each entry is initialized. 

In the `hash_table_v2_add_entry` function, I locked the mutex in the beginning of the function for each hash table entry and unlocked it at the end of the function to prevent race conditions when adding entires. Unlike in v1, this approach only blocks other threads that are trying to add an entry to the same linked list, instead of blocking all other threads attempting to add entries to the hash table (regardless of whether they are in the same linked list or not). 

In the `hash_table_v2_destroy` function, the mutex for each entry is destroyed.

### Performance
```shell
$ ./hash-table-tester -t 4 -s 50000

Results:

Hash table base: 206,681 usec
    - 0 missing
Hash table v1: 86,425 usec
    - 0 missing

Hash table base: 202,339 usec
    - 0 missing
Hash table v1: 95,239 usec
    - 0 missing

Hash table base: 210,524 usec
    - 0 missing
Hash table v1: 66,156 usec
    - 0 missing
```
Version 2 is faster than the base version. On lnxsrv with 4 cores, Version 2 runs 2.5x faster on average. In its fastest run, it ran 3.18x faster than the base version. This is because Version 2 uses concurrency as long as the hash table isn't adding to the same entry. 

## Cleaning up
```shell
make clean
```

## Testing
```python
python -m unittest
```
Result: Ran 3 tests in 13.618s, OK

```
valgrind ./hash-table-tester -t [THREADS] -s [ENTRIES]
```
Result: All heap blocks were freed -- no leaks are possible