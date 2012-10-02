# TI-NSPIRE Linux In-Place Bootloader

Boots a Linux kernel from inside the CAS software. For CX only right now (RAM sizes and machine ID are hardcoded).

## Usage

Copy Linux kernel to ```/documents/linux/zImage.tns``` then run the program.

## Developer's notes

We want to behave well while we're still relying on the host system (i.e. when reading from files or allocating memory).

After the ramdisk, kernel and ATAGs have all been setup and loaded into memory, we can start messing about. At this point, we must be completely self sustained and must not use any function exported by ```os.h``` or else we will crash. We also need to be careful that any operation we do will not overwrite ourselves.

Since ```printf``` is buffered and relys on the host system, ```printk``` is provided for printing debugging messages. It should be safe to call any time.

## Brokenness

Ramdisk loading is broken.