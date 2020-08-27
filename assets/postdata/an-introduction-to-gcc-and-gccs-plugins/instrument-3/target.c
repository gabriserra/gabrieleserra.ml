#include <stdio.h>


void __inst_profile()
{
    printf("Do something here!\n");
}

__attribute((profiled)) void f1()
{
    int a;
    a = a + 1;
    return;
}

int main()
{
    printf("I'm a target C program.\n");
    f1();
    return 1;
}