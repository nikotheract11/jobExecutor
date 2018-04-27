#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./trie.h"
#include "./heaps/PQInterface.h"
#include <poll.h>


/*
 * Returns tokens of user input
*/
char **getok(char* s,int *k){
   char **st;
   int count=0;
   char *token;
   for(unsigned int i=0;i<=strlen(s);i++){
      if(s[i] == ' ' || s[i] == '\0') count++;
   }
   st = (char**) malloc(count*sizeof(char*));

   token = strtok(s," ");
   int j=0;
   while(token != NULL) {
      st[j] = (char*) malloc(sizeof(char)*(strlen(token)+1));
      strcpy(st[j++],token);
      token = strtok(NULL," ");
      if(j>11) break;
   }
   *k=j;
   free(token);
   return st;

}



int count(int i){
   int counter=0;
   if(i==0) return 1;
   while (i!=0) {
      counter++;
      i/=10;
   }
   return counter;
}

int count_digits(int id,int text, double score){
   int rest=14;      // 14 bcs of 4 digit_precision on score and the characters "." "()" etc
   if(score > 0) rest--;
   return count(id)+count(text)+count((int)score)+rest;
}

int writeLog(char *op,char *keyword,char *path,char *t){
   fprintf(fp, "%s:%s:%s:%s\n",t,op,keyword,path );
   return 0;
}



/*
 * Searching on trie, implementation of /search feature
*/
char * search(char** s,int j,char *date){
   if(j < 2) {
      printf("Wrong arguments for /search\n");
      return NULL;
   }

   char *reply;
   int sz=32;  // first, allocate 32 bytes for reply, realloc if needed
   int total=0;   // total bytes to be written

   reply = malloc(sz*sizeof(char));

   printf("-------%s\n",s[0] );
   for(int i=1;i<j;i++){
      printf("#####################%s\n",s[i] );
      p_list *p = find(s[i]);    // p_list of this word if exists
      if(p == NULL) continue;       // if not exists break

      //int p_length = p->plen;    // # of texts containing s[i]
      while(p != NULL) {
         printf("Word found in text->%d line->%d\n",p->text_id,p->line );
         total += sizeof(int) + strlen(files[p->text_id]) \
                  + strlen(str[p->text_id][p->line]) +4; // Bcs reply is Path Line# Line_Content  , +4 bcs of ":",'\n','\0'
         if(total > sz) {
            sz *= 2;    // Double size
            if(realloc(reply,sz) == NULL) perror("realloc failed");      // error check
         }

         sprintf(reply,"%s:%d:%s\n",files[p->text_id],p->line,str[p->text_id][p->line]);
          p = p->next;
      }

      /* Keep log */
      writeLog("search",s[i],files[p->text_id],date);

      return reply;
   }

   printf("Search called to operate: %s\n",s[0] );
   return 0;
}

char *maxcount(char **s,int j,char *date){
   if(j < 2) {
      printf("Wrong arguments for /search\n");
      return NULL;
   }

   printf("-------%s\n",s[0] );
   for(int i=1;i<j;i++){
      printf("#####################%s\n",s[i] );
      p_list *p = find(s[i]);    // p_list of this word if exists
      if(p == NULL) break;       // if not exists break

      //int p_length = p->plen;    // # of texts containing s[i]
      pair *Pair = malloc(p->plen*sizeof(pair));
      int l=0;
      while(p != NULL){
   		printf("i=%d\n", i);
   		Pair[l].text_id =  p->text_id;
   		Pair[l].score = 0;
   		printf("plen=%d --Found on text -> %d %s, line -> %d, freq -> %d\n %s\n",p->plen,p->text_id,files[p->text_id],p->line,p->freq,str[p->text_id][p->line] );
   		if(p->next != NULL){
   		 while(p->text_id == (p->next)->text_id) {
   			Pair[l].score++;
   			p = p->next;
   			}
   		}
   		p = p->next;
   		l++;
   	}
   	pair max = Pair[0];
   	for(int k=1;k<i;k++){
   		if(Pair[k].score <= max.score) max = Pair[k];
   	}
   }
   return 0;
}

char *mincount(char **s,int j,char *date){
   if(j < 2) {
      printf("Wrong arguments for /search\n");
      return NULL;
   }

   printf("-------%s\n",s[0] );
   for(int i=1;i<j;i++){
      printf("#####################%s\n",s[i] );
      p_list *p = find(s[i]);    // p_list of this word if exists
      if(p == NULL) break;       // if not exists break
      pair *Pair = malloc(p->plen*sizeof(pair));
      int l=0;
      while(p != NULL){
   		printf("i=%d\n", i);
   		Pair[l].text_id =  p->text_id;
   		Pair[l].score = 0;
   		printf("plen=%d --Found on text -> %d %s, line -> %d, freq -> %d\n %s\n",p->plen,p->text_id,files[p->text_id],p->line,p->freq,str[p->text_id][p->line] );
   		if(p->next != NULL){
   		 while(p->text_id == (p->next)->text_id) {
   			Pair[l].score++;
   			p = p->next;
   			}
   		}
   		p = p->next;
   		l++;
   	}
   	pair min = Pair[0];
   	for(int k=1;k<i;k++){
   		if(Pair[k].score <= min.score) min = Pair[k];
   	}
   }
   return 0;
}

int wc(char ***s){
   int i=0,bytes=0,words=0,lines=0;
   while(s[i] != NULL){
      int j=0;
      while(s[i][j] != NULL){
         lines++;
         bytes += strlen(s[i][j]);
         int tmp=0;
         getok(s[i][j],&tmp);
         words += tmp;
         j++;
      }
      i++;
   }
   printf("b=%d w=%d l=%d\n",bytes,words,lines );
   return 0;
}

/*
 * /df implementation
*/
int df(char** s,int j){
   printf("Df called to operate: %s\n",s[0] );
   if(j==2){
      p_list *p = find(s[1]);
      if(p != NULL)  printf("%s %d\n",s[1],p->plen );
   }
   else{
      char *bf=malloc(256*sizeof(char));      // not possible word with length > 256
      pr_trie(t,bf,0);
      free(bf);
   }
   return 0;
}

/*
 * /tf implementation
*/
int tf(char** s,int j){
   printf("Tf called to operate: %s\n",s[0] );
   if(j<3) {
      printf("Not enough arguments\n");
      return -1;
   }
   int text_id = atoi(s[1]);
   p_list *p = find(s[2]);       // s[2] = key s[1] = id becuase s[0] = /tf
   if(p == NULL) {
      printf("Word \'%s\' not exists\n",s[2] );
   }
   while(p != NULL){
      if(p->text_id==text_id) {
         printf("%d %s %d\n",p->text_id, s[2],p->freq );
         return 0;
      }
      else p = p->next;
   }

   return -2;
}

/*
 * Calls the right function
*/
char * operate( char** s,int j,char *k){
   if(!strcmp(s[0],"/exit")) return NULL;
   if(!strcmp(s[0],"/search")) return search(s,j,k);
   else if(!strcmp(s[0],"/df")) return df(s,j);
   else if(!strcmp(s[0],"/tf")) return tf(s,j);
   else {
      printf("Not such a command available\n");
      return NULL;
   }
}

int sendMsg(char *buf,int *wr,int w){
   struct pollfd *pfds;
   pfds = malloc(w*sizeof(struct pollfd));
   for(int i=0;i<w;i++){
      pfds[i].fd = wr[i];
      pfds[i].events = POLLOUT;
   }

   int sent,bytesToSend,n;
   bytesToSend = strlen(buf);
   poll(pfds,w,-1);
   for(int i=0;i<w;i++){
      sent = sizeof(int);

      if(write(wr[i],&bytesToSend,sizeof(int)) < 0)
         perror("write error");


      if(pfds[i].revents & POLLOUT){
         while((n=write(wr[i],(buf),strlen(buf))) > 0){
            sent += n;     // Total bytes sent, dont stop until all bytes have been sent
            if(sent >= strlen(buf)+sizeof(int)) break;
         //   poll(pfds,w,-1);     // ==================== NEEDED?????
         }
      }
   }
   return 0;
}

char * readMsg(int *rfd,int w){
   struct pollfd *pfds;
   char *buf;
   pfds = malloc(w*sizeof(struct pollfd));
   for(int i=0;i<w;i++){
      pfds[i].fd = rfd[i];
      pfds[i].events = POLLIN;
   }

   poll(pfds,w,1);
   for(int i=0;i<w;i++){
      if(pfds[i].revents & POLLIN){
      /*   int bytesToRead;
         if(read(rfd[i],&bytesToRead,sizeof(int)) < 0 )
            perror("read failed");
         buf = malloc((bytesToRead+1)*sizeof(char));      // NEEDED +1??
         while(bytesToRead > 0){
            int b_read;
            if((b_read = read(rfd[i],&buf[sizeof(buf)-bytesToRead-1],bytesToRead)) < 0)
               perror("read msg failed");
            bytesToRead -= b_read;
         }
         printf("%s\n",buf );*/
         int bytesToRead,bytesRead=0;
         read(rfd[i],&bytesToRead,sizeof(int));
         printf("Need to read %d bytes\n",bytesToRead );
         buf = malloc((bytesToRead+1)*sizeof(char));
         while((bytesRead += read(rfd[i],&(buf[bytesRead]),bytesToRead)) < bytesToRead);
         printf("bytesRead = %d==========================\n %s\n ======================\n",bytesRead,buf );
      }
   }
   return buf;
}

/*
 * The Application interface
*/
int interface(int *wfd,int *rfd,int w){
   char *buf;

   while(1){
      printf("Type command: ");
      scanf("%m[^\n]s\n",&buf );
      getchar();

      /* Send query to workers */
      sendMsg(buf,wfd,w);
      readMsg(rfd,w);
      free(buf);
   }

   return 0;
}
