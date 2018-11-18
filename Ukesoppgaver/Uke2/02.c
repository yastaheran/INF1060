#include <stdio.h>

/* Gjør om denne for-løkken slik at den bruker pekere istedenfor vanlige array-indekser 
(slik som arr[i]) til å gå igjennom arrayen arr:
int i;
short arr[] = {2, 1, -1, 10, 22, 21};
int arrlen = 6;
for(i = 0; i < arrlen; i++)
    printf("[%d]: %d\n", i, arr[i]);
*/    
    
int main(void) {
    int i, j;
    int arr[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int arrlen = 3;

    printf("Array:\n");
    for(i = 0; i < arrlen; i++) {
        for (j = 0; j < arrlen; j++) {
            printf("[%d][%d]: %d\n", i, j, arr[i][j]);
        }
    }

    printf("Pekere:\n");
    for(i = 0; i < arrlen; i++) {
        for (j = 0; j < arrlen; j++) {
            int *row_pointer = *(arr + i);
            printf("[%d][%d]: %d\n", i, j, *(row_pointer + j));
            //arr = {{x, x, x, ...}, ...}
            //arr[0] = {x, x, x, ...}
            //arr[0][0] = x;
        }
    }
}