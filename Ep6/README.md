# debug fork in MenuOS

Add `fork-example.c` into MenuOS and start os with qemu. Open gdb and start debugging:

```bash
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
(gdb) b sys_clone
Breakpoint 1 at 0xc1043de0: file kernel/fork.c, line 1727.
(gdb) b do_fork
Breakpoint 2 at 0xc1043a30: file kernel/fork.c, line 1628.
(gdb) b dup_task_struct
Breakpoint 3 at 0xc1042485: file kernel/fork.c, line 309.
(gdb) b copy_process
Breakpoint 4 at 0xc10423f0: copy_process. (2 locations)
(gdb) b copy_thread
Breakpoint 5 at 0xc10010c0: file arch/x86/kernel/process_32.c, line 134.
(gdb) b ret_from_fork
Breakpoint 6 at 0xc1767340: file arch/x86/kernel/entry_32.S, line 292.
(gdb) c
Continuing.
```
