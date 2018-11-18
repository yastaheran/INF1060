#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char strgetc(char s[], int pos) {
	if(strlen(s) > pos && pos 0 >= pos)
		return s[pos];
	return 0;
}

int main (int argc, char *argv[]) {
	char* s = "test1";
	printf("strgetc(s, 0) = %c\n", strgetc(s, 0));
	printf("strgetc(s, 3) = %c\n", strgetc(s, 3));

	return 0;
}
