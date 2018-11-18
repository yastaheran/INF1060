#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
int main(void) {
    char* str="Hi all.\nMy name is Matteo.\n\nHow are you?\n\nThanks";
    char **result = NULL;
    unsigned int index = 0;
    unsigned int i = 0;
    size_t size = 0;
    char *ptr, *pstr;
    ptr = NULL;
    pstr = str;

    while(pstr) {
        ptr = strstr(pstr, "\n\n");
        result = realloc(result, (index + 1) * sizeof(char *));
        size = strlen(pstr) - ((ptr)?strlen(ptr):0);
        result[index] = malloc(size * sizeof(char));
        strncpy(result[index], pstr, size);
        index++;
        if(ptr) {
            pstr = ptr + 2;
        } else {
            pstr = NULL;
        }
    } ;

	for(i = 0; i < index; i++) {
        printf("Array[%d] : >%s<\n", i, result[i]);
	}
	
    return 0;
} */

/*int main () {
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok_r(str," ,.-"); 
  int i;
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }

  
  return 0;
}*/


int main() {
    char str1[100];
    char newString[10][10]; 
    int i,j,ctr;
       printf("\n\n Split string by space into words :\n");
       printf("---------------------------------------\n");    
 
    printf(" Input  a string : ");
    fgets(str1, sizeof str1, stdin);	
 
    j=0; ctr=0;
    for(i=0;i<=(strlen(str1));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str1[i]==' '||str1[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str1[i];
            j++;
        }
    }
    printf("\n Strings or words after split by space are :\n");
    for(i=0;i < ctr;i++)
        printf(" %s\n",newString[i]);
    return 0;
}

char** split(char* s) {
	
}