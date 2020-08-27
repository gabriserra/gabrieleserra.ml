#include <stdio.h>

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