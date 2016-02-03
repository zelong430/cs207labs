#include <stdlib.h>
#include <stdio.h>

int fib(int n){
    if(n == 1){
        return 0;
    }
    if(n == 2){
        return 1;
    }
    return fib(n-1) + fib(n-2);
}

int main(int argc, char **argv)
{
    int a = fib(8);
    printf("The result is: %d\n",a);
}
