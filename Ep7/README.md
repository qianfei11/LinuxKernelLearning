# how linux kernel load and execute a program

shared library: link during loading.

```bash
# shlibexample.h - Interface of Shared Lib Example
# shlibexample.c - Implement of Shared Lib Example
gcc -shared shlibexample.c -o libshexample.so -m32
```

dynamical loading lib: link during executing.

```bash
# dllibexample.h - Implement of Dynamical Loading Lib Example
# dllibexample.c - Implement of Dynamical Loading Lib Example
gcc -shared dllibexample.c -o libdlexample.so -m32
```

Compile main program:

```bash
# main.c - Main Program
gcc main.c -o main -L$PWD -lshlibexample -ldl -m32
export LD_LIBRARY_PATH=$PWD
./main
```
