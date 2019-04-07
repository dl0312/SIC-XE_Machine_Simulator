#include <stdio.h>

int a[10][10][10];
struct AA {
    int a;
    char b;
    float c;
} aa;

enum days {Sun, Mon, Tue, Wed, Thu, Fri, Sat} dl;

union BB {
    char a;
    int b;
} bb;

int *cc, k;

void main (void){
    k = a[4][5][7];
    dl = Sun;
    bb.b = k;
    cc = &bb.b;
}