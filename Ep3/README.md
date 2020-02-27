# debug a small os

```bash
# Download kernel source && uncompress the archive
wget https://mirrors.edge.kernel.org/pub/linux/kernel/v3.x/linux-3.18.6.tar.xz
xz -d liinux-3.18.6.tar.xz
tar -xvf linux-3.18.6.tar
# Compile kernel
cd linux-3.18.6
make i386_defconfig
make
# Download menuOS
cd .. && git clone https://github.com/mengning/menu.git
# Compile init file
cd menu
gcc -o init linktable.c menu.c test.c -m32 -static -lpthread
# Pack boot image
mkdir rootfs && cd rootfs
cp ../menu/init .
find . | cpio -o -Hnewc | gzip -9 > ../rootfs.img
# Compile with debug info
cd linux-3.18.6
make menuconfig # kernel hacking --> [*] compile the kernel with debug info
make
# Start small os with listening on port 1234 and freezing CPU at startup
qemu -kernel linux-3.18.6/arch/x86/boot/bzImage -initrd rootfs.img -s -S
```

Use gdb to debug:

```
$ gdb
GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.04) 7.11.1
Copyright (C) 2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
(gdb) file linux-3.18.6/vmlinux
Reading symbols from linux-3.18.6/vmlinux...done.
(gdb) target remote:1234
Remote debugging using :1234
0x0000fff0 in ?? ()
(gdb) b start_kernel
Breakpoint 1 at 0xc1a4978f: file init/main.c, line 501.
(gdb) c
Continuing.

Breakpoint 1, start_kernel () at init/main.c:501
501	{
(gdb) l
496		pgtable_init();
497		vmalloc_init();
498	}
499
500	asmlinkage __visible void __init start_kernel(void)
501	{
502		char *command_line;
503		char *after_dashes;
504
505		/*
(gdb)
```
