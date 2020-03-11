#include <stdio.h>
#include "shlibexample.h"
#include <dlfcn.h>

int main()
{
    printf("This is a Main Program!\n");
    // Use Shared Lib
    printf("Calling SharedLibApi() function of libshlibexample.so!\n");
    SharedLibApi();
    // Use Dynamical Loading Lib
    void *handle = dlopen("libdllibexample.so", RTLD_NOW);
    if (handle == NULL)
    {
        printf("Open Lib libdllibexample.so Error: %s\n", dlerror());
        return FAILURE;
    }
    int (*func)(void);
    char *error;
    func = dlsym(handle, "DynamicalLoadingLibApi");
    if ((error = dlerror()) != NULL)
    {
        printf("DynamicalLoadingLibApi not found: %s\n", error);
        return FAILURE;
    }
    func();
    dlclose(handle);
    return SUCCESS;
}
