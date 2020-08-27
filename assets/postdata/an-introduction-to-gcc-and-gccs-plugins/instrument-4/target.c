#include <stdio.h>


__attribute((naked)) void __inst_profile(long retaddr)
{
    printf("Do something here!\n");
    asm("jmp *%0" :: "r" (retaddr));
}

__attribute((profiled)) void f1()
{
    int a = 5;
    a = a + 5;  
    return;
}

int main()
{
    printf("I'm a target C program.\n");
    f1();
    return 1;
}