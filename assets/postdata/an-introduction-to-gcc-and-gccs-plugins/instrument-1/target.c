#include <stdio.h>

// Note:
// We need 'no_instrument_function' attribute on __cyg_profile_fun_*

// called every time a function get called
__attribute__ ((no_instrument_function)) void __cyg_profile_func_enter (void *this_fn, void *call_site)
{
    printf("Entering in 0x%lx from 0x%lx...\n", (unsigned long)this_fn, (unsigned long)call_site);
}

// called every time a function returns
__attribute__ ((no_instrument_function)) void __cyg_profile_func_exit  (void *this_fn, void *call_site)
{
    printf("Exiting from 0x%lx to 0x%lx...\n", (unsigned long)this_fn, (unsigned long)call_site);
}

void f1()
{
    return;
}

int main()
{
  printf("I'm a target C program.\n");
  f1();
  return 1;
}