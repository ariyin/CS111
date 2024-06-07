# Hey! I'm Filing Here

In this lab, I successfully implemented the following TODO

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
```shell
dumpe2fs cs111-base.img
fsck.ext2 cs111-base.img
```