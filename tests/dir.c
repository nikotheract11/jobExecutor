#include  <stdio.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "../trie.h"

char **get(const char* file);
char ***map_(char **dirname,int dir_num,unsigned int *files_num);


int main(){
	char *d=".";
	unsigned int fn;
	int inn;
	char *token,*tmp;
	char ***str = map_(&d,1,&fn);
	int j=0;
	for(int i=0;i<fn;i++){	// for each file
		j=0;
		while(str[i][j] != NULL){	// for each line
			tmp =(char *) malloc(sizeof(char)*(strlen(str[i][j])+1));
			strcpy(tmp,str[i][j]);
			token = strtok(tmp," ");
			while(token != NULL){
				if(token[strlen(token)-1] =='\n' || token[strlen(token)-1] ==' ') token[strlen(token)-1]='\0'; // ==
				if((inn=insert(token,j))<0) printf("ERROR %d\n", inn);
            token  = strtok(NULL," ");
			}
			j++;
         free(tmp);
         free(token);
		}
	}
	char *bf=malloc(256*sizeof(char));      // not possible word with length > 256
	pr_trie(t,bf,0);
	free(bf);

}

/* map returns a map to texts */
char ***map_(char **dirname,int dir_num,unsigned int *files_num){
	DIR *dir_ptr;
	struct dirent *direntp;
	char ***map;

	size_t sz=32;
	map = (char***) malloc(sz*sizeof(char**));     // 32 places for texts, if more -> realloc
	unsigned int i=0;
	for(int j=0;j<dir_num;j++){

		/* open directory j */
		if ( ( dir_ptr = opendir(dirname[j])) == NULL )
			fprintf(stderr, "cannot open \n");
		else {
			while ( ( direntp=readdir(dir_ptr) ) != NULL ){
				if (direntp->d_ino == 0 ) continue;

				/* reallocate space when sz texts reached */
				if(i >= sz) {
					sz *= 2;
					map = (char***) realloc(map,sz*sizeof(char ***));
					if(map == NULL){
						perror("Realloc failed");
						return NULL;
					}
				}

				/* store each text to map */
				if(direntp-> d_type != DT_DIR)    // ignore "." and ".."
					map[i++]=get(direntp->d_name);
				//printf("%s\n", map[i-1][5]);
			}
			closedir(dir_ptr);
		}
	}
	printf("files=%d\n",i );
	*files_num = i;
	return map;
}

	/*
	 * Return the # of texts
	 */
	int getLinesNumber(FILE *fp){
		char c;
		int count = 0;
		while(!feof(fp)){
			c = fgetc(fp);
			if(c == '\n') count++;
		}
		fseek(fp,0,0);
		return ++count;        // ++count becuase the last line has not a '\n' in the end
	}

	char **get(const char* file){

		FILE *fp;
		fp = fopen(file,"r");
		if(fp == NULL) return NULL;
		int i=0,j=0;
		char c;
		char **str;

		int n = getLinesNumber(fp);
		str = (char**) malloc(n*sizeof(char*));
		for(int k=0;k<n;k++) str[k] = NULL;

		while(i++<n ){
			size_t a;
			/*   fscanf(fp,"%d",&id);    // Get id

			     if(id != j) {
			     printf("Index Error\n");
			     return NULL;
			     }

			     fgetc(fp);           // Ignore tabs and whitespaces*/

			getline(&(str[j]),&a,fp);

			if((c=fgetc(fp)) == EOF) break;
			else ungetc(c,fp);
			j++;
		}
		fclose(fp);


		return str;
	}
