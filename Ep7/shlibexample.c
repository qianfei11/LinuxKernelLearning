#include <stdio.h>
#include "shlibexample.h"

int SharedLibApi()
{
    printf("This is a shared library!\n");
    return SUCCESS;
}