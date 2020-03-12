# debug execve and analysis how process dispatch and handover using gdb

Start debug:

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
(gdb) b sys_execve
Breakpoint 1 at 0xc1135af0: file fs/exec.c, line 1607.
(gdb) b load_elf_binary
Breakpoint 2 at 0xc1171ae0: file fs/binfmt_elf.c, line 572.
(gdb) b start_thread
Breakpoint 3 at 0xc1000e80: file arch/x86/kernel/process_32.c, line 199.
(gdb) c
Continuing.
```

When execute `exec`, the program will stop by breakpoint at `sys_execve`:

```gdb
(gdb) c
Continuing.

Breakpoint 1, SyS_execve (filename=0x80bf1ae, argv=0xbfb09880, envp=0xbfb0ae5c)
    at fs/exec.c:1607
1607			const char __user *const __user *, envp)
(gdb) l
1602	}
1603
1604	SYSCALL_DEFINE3(execve,
1605			const char __user *, filename,
1606			const char __user *const __user *, argv,
1607			const char __user *const __user *, envp)
1608	{
1609		return do_execve(getname(filename), argv, envp);
1610	}
1611	#ifdef CONFIG_COMPAT
(gdb) s
SYSC_execve (envp=<optimized out>, argv=<optimized out>,
    filename=<optimized out>) at fs/exec.c:1609
1609		return do_execve(getname(filename), argv, envp);
(gdb) s
getname (filename=0x80bf1ae "/hello") at fs/namei.c:211
211	{
(gdb) l
206		return err;
207	}
208
209	struct filename *
210	getname(const char __user * filename)
211	{
212		return getname_flags(filename, 0, NULL);
213	}
214
215	/*
```

And continuing, the program will stop at `load_elf_binary`:

```gdb
(gdb) c

Breakpoint 2, load_elf_binary (bprm=0xc0013f00) at fs/binfmt_elf.c:572
572	{
(gdb) l
567		return PAGE_ALIGN(stack_top) - random_variable;
568	#endif
569	}
570
571	static int load_elf_binary(struct linux_binprm *bprm)
572	{
573		struct file *interpreter = NULL; /* to shut gcc up */
574	 	unsigned long load_addr = 0, load_bias = 0;
575		int load_addr_set = 0;
576		char * elf_interpreter = NULL;
(gdb) n
593		loc = kmalloc(sizeof(*loc), GFP_KERNEL);
(gdb)
572	{
(gdb) l
567		return PAGE_ALIGN(stack_top) - random_variable;
568	#endif
569	}
570
571	static int load_elf_binary(struct linux_binprm *bprm)
572	{
573		struct file *interpreter = NULL; /* to shut gcc up */
574	 	unsigned long load_addr = 0, load_bias = 0;
575		int load_addr_set = 0;
576		char * elf_interpreter = NULL;
```

Continuing a `start_thread`, and we can print the value of `new_ip`:

```gdb
(gdb) c
Continuing.

Breakpoint 3, start_thread (regs=0xc7b53fb4, new_ip=0x8048736,
    new_sp=0xbfacdf60) at arch/x86/kernel/process_32.c:199
199	{
(gdb) p new_ip
$1 = 0x8048736
```

The entry point of `hello` is equal to `new_ip`:

```bash
readelf -h hello
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 03 00 00 00 00 00 00 00 00
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - GNU
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x8048736
  Start of program headers:          52 (bytes into file)
  Start of section headers:          724052 (bytes into file)
  Flags:                             0x0
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         6
  Size of section headers:           40 (bytes)
  Number of section headers:         31
  Section header string table index: 28
```

keep step into, the new thread start to copy regs from the father process:

```gdb
(gdb) n
200		set_user_gs(regs, 0);
(gdb) s
201		regs->fs		= 0;
(gdb)
202		regs->ds		= __USER_DS;
(gdb)
203		regs->es		= __USER_DS;
(gdb)
204		regs->ss		= __USER_DS;
(gdb)
205		regs->cs		= __USER_CS;
(gdb)
206		regs->ip		= new_ip;
(gdb)
207		regs->sp		= new_sp;
(gdb)
208		regs->flags		= X86_EFLAGS_IF;
(gdb)
213		set_thread_flag(TIF_NOTIFY_RESUME);
(gdb) c
Continuing.
```
