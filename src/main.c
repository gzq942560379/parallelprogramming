#include <stdio.h>

int main() {
    int c[20];

    void * v = c;
    printf("%ld %ld\n", v, v + 1);
    return 0;
}