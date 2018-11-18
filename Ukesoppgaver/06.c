#include <stdio.h>

int strcmpx(unsigned char s1[], unsigned char s2[]) {
	int ix = 0;
	while(s1[ix]==s2[ix] && s1[ix]!=0) {
		++ix;
	}
	
	return s1[ix] - s2[ix];
}

void test(unsigned char s1[], unsigned char s2[]) {
    printf("strcmpx(\"%s\", \"%s\") gir %d\n",
        s1, s2, strcmpx(s1,s2));
}

int main() {
  test("Abc", "Ab");
  test("Abc", "Abc");
  test("Abc", "Abcd");
  return 0;
}