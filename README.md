# Keystone
Keystone is a x86_64 kernel, trying to be compatible with Linux binaries, as it's planning to use musl (a libc).

## Building
This doesn't come with Limine binaries, so you need to get them:
```
make limine
```
This will get the limine binaries (from v9.x-binary) and copy them to the needed directories.

Then you can build the kernel:
```
make
```
This will build the kernel in the `bin` directory.

You need to also make the Limine ISO, which can be later be put on a USB stick:
```
make iso
```
## Running
With QEMU:
```
make qemu
```
You can customize the QEMU command by editing the `qemucmd` variable in the `GNUmakefile`.

## Contributing
Contributions are welcome! If you have any ideas, feel free to open an issue or a pull request.