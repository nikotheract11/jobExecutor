#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
   FILE *fp = fopen("./docfile","r");
   size_t a;
   char *str=NULL;
   while(!feof(fp)){
      int l=getline(&str,&a,fp);
      str[l-1]='\0';
      printf("%s\n",str );
   }
   return 0;
}
