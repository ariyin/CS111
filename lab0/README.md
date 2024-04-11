# A Kernel Seedling
Counts the current number of running processes (or tasks) running.

## Building
```shell
make
```

## Running
```shell
sudo insmod proc_count.ko
cat /proc/count
```
Result: 173

## Cleaning Up
```shell
sudo rmmod proc_count
make clean
```

## Testing
```python
python -m unittest
```
Result: Ran 3 tests in 2.207s, OK

Report which kernel release version you tested your module on
(hint: use `uname`, check for options with `man uname`).
It should match release numbers as seen on https://www.kernel.org/.

```shell
uname -r -s -v
```
Kernel version: Linux 5.14.8-arch1-1 #1 SMP PREEMPT Sun, 26 Sun 2021 19:36:15 +0000