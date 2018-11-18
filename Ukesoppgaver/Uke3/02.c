#include <stdlib.h>
#include <stdio.h>

int main() {
    char *s = "hello world";
    *s = 'H';
    printf("%s\n", s);
    return 0;
}


/*Dette eksempelet vil resultere i en Segmentation fault. Bruk gdb og ddd til å finne ut hvorfor. */