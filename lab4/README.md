# Hey! I'm Filing Here

Created a 1 MiB ext2 file system with 2 directories, 1 regular file, and 1 symbolic link.

## Building
```shell
make
```

## Running
```shell
./ext2-create
mkdir mnt
sudo mount -o loop cs111-base.img mnt
```

## Cleaning up
```shell
make clean
rmdir mnt
sudo umount mnt
```

## Testing
```python
python -m unittest
```
Result: Ran 2 tests in 0.222s, OK

```shell
dumpe2fs cs111-base.img
fsck.ext2 cs111-base.img
ls -ain mnt/
```