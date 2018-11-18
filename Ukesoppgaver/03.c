#include <stdio.h>
#include <limits.h>

int main() {
    short a, b, sum;
	long c, d, s;
	
    a = 20000;  b = 20000;  sum = a+b;
    
    if(a > 0 && b > SHRT_MAX - a) {
    	printf("short overflow");
    } else {
    	printf("%d + %d = %d\n", a, b, sum);
	}
	
    c = 12L; d = 14L; s = c+d;
    
    if(a > 0 && b > SHRT_MAX - a) {
    	printf("short overflow");
    } else {
    	printf("%d + %d = %d\n", c, d, s);
	}
}