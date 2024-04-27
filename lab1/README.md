## UID: 806023162

## Pipe Up
Implements the low level code performed by the pipe (|) operator in shells.

## Building
```shell
make
```

## Running
```
$ ./pipe

$ echo $?
22
```
```
$ ./pipe bogus

$ echo $?
2
```
```
$ ./pipe ls
Makefile  pipe  pipe.c  pipe.o  README.md test_lab1.py

$ echo $?
0
```
This should have the same output as `ls`
```
$ ./pipe ls cat wc
6  6  51

$ echo $?
0
```
This should have the same output as `ls | cat | wc`

## Cleaning up
```shell
make clean
```

## Testing
```python
python -m unittest
```
Result: Ran 3 tests in 0.066s, OK
