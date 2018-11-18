#include <stdio.h>
#include <stdlib.h>

void sum(int a, int b, int* result);

int main() {
	int res;
	sum(5, 4, &res);

}

void sum(int a, int b, int* result) {
	int r = a + b;
	result = &r;
	printf("resultat %d \n", *result);
}