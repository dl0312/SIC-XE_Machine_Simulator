#include <stdio.h>

char a[10][10][10];
struct AA {
    char a;
    int b;
    char c;
    float d;
} aa;

enum days {Sun, Mon, Tue, Wed, Thu, Fri, Sat} dl;

union BB {
    char a;
    int b;
    float c;
    long d;
} bb;

int *cc, k;
float i, j;

void main (void){
    k = a[3][0][0];
    aa.d = a[2][1][5] + a[1][2][3];
    bb.b = k;
    j = i + k;
    j = i + (float)k;
    cc = &bb.b;
}