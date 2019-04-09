#include<stdio.h>

int a, b;

int fun() {
    return a = a + 10;
}

int main() {
    a = 10;
    b = a + fun();
    printf("r");
    printf("b is %d\n", b);
    a = 10;
    b = fun() + a;
    printf("l");
    printf("b is %d\n", b);
    return 0;
}
