# You Spin Me Round Robin

Implements round robin scheduling for a given workload and quantum length.

## Building

```shell
make
```

## Running

cmd for running
```shell
./rr processes.txt 3
```
arg1: file path
arg2: quantum length

results
```shell
Average waiting time: 7.00
Average response time: 2.75
```

## Cleaning up

```shell
make clean
```

## Testing
```python
python -m unittest
```
Result: Ran 2 tests in 0.096s, OK