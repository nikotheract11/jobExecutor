#include  <sys/types.h>
#include  <sys/stat.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <time.h>
#include "../trie.h"

#define MAXBUFF 1024
#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"
#define PERMS   0666


pid_t exited;

/* Insert list node with a path for jobExecutor */
int insertAtStart(char *p,list **l){
   if(*l == NULL){
      (*l) = malloc(sizeof(list));
      (*l)->path = malloc(strlen(p)+1);
      strcpy((*l)->path,p);
      (*l)->next = NULL;
      return 0;
   }
   list *tmp = *l;
   list *new = malloc(sizeof(list));
   new->path = malloc((strlen(p)+1));
   strcpy(new->path,p);
   new->next = tmp;
   *l = new;

   /*list *i = *l;
   while(i!=NULL){
      printf("%s\n",i->path );
      i = i->next;
   }*/
   return 0;

}

void handler(int signum){
	//for(int i=0;i<)
	int status;
	exited = waitpid(-1,&status,WNOHANG);

}

int jobExecutor(int *wr,int *rfd,int w){
	int fd,n;
   char str[50],c;
	int counter=0,j=0;
	static struct sigaction act;

	sigfillset(&(act.sa_mask));
	act.sa_handler = handler;

	list **paths;
	paths = malloc(w*sizeof(list));


	if((fd= open("./docfile",0)) < 0) perror("open");

	/* Send paths to workers */
   while((n=read(fd,&c,1)) > 0){
		if(c == '\n') {
			str[j] = '\0';
			if(write(wr[counter%w],str,j+1)<=0) perror("write error");
			insertAtStart(str,&paths[counter++%w]);
			j=0;
		}
		else str[j++] = c;
   }

	/* All paths have been sent, now send STOP to let them know */
	for(int i=0;i<w;i++){
		write(wr[i],"stop",w);
	}

	/* Here comes the user interface */
//	interface(wr,rfd,w);



	return 0;
}



int worker(int rfd,int wfd){
	int n;
	char buff[1024];
   char s[32];
   time_t arrival;
   //int ar_flag=1;
   sprintf(s,"worker%d",getpid());
   fp = fopen(s,"a");

   fprintf(fp, "%s\n","kalisperaa ");

	while((n=read(rfd,buff,MAXBUFF)) > 0){
		/* Here open files, create map and trie */
		if(!strcmp(buff,"stop")) return 0;
		printf("worker%d : %s\n",getpid(),buff );
	}

	/* Now, that files are read, read the query */
	while(1){
		/* Tokenize buffer, read the first token to execute the query */
      char *query= readMsg(&rfd,1);
      arrival = time(NULL);
      char * c_time = ctime(&arrival);
      int argc;
      char **tokens = getok(query,&argc);    // break query to tokens
      char *reply;
      reply = operate(tokens,argc,c_time);       // delete k
      printf("%s\n",reply );
      sendMsg(reply,&wfd,1);
	}

	/* Compose reply to jobExecutor with the results */

	return 0;
}

int main(){
	int readfd[5],writefd[5],w=5;
	pid_t p,workers[5];


	for(int i=0;i<w;i++){
		char fifo_1[32],fifo_2[32];

		/*===== Create fifos and open for jobExecutor =====*/
		sprintf(fifo_1,"/tmp/fifo1_%d",i);
		sprintf(fifo_2,"/tmp/fifo2_%d",i);

		if((mkfifo(fifo_1,PERMS) < 0) && (errno != EEXIST)){
			perror("cant create fifo");
		}
		if((mkfifo(fifo_2,PERMS) < 0) && (errno != EEXIST)){
			perror("cant create fifo");
		}
	}
	/*======= fork() w Workers =======*/
	int read_fd,write_fd;
	for(int i=0;i<w;i++){
		char fifo_1[32],fifo_2[32];
		p = fork();
		sprintf(fifo_1,"/tmp/fifo1_%d",i);
		sprintf(fifo_2,"/tmp/fifo2_%d",i);
		if(p == 0) {
			/*======	Open fifos for workers ======*/
			if((write_fd = open(fifo_1,1)) < 0){
				perror("worker:cant open write");
			}
			if((read_fd = open(fifo_2,0)) < 0){
				perror("worker: cant open read fifo");
			}
			break;
		}
		else if(p > 0){
			/*	Open write fifo for jobExecutor */
			workers[i]=p;
			if ( (readfd[i] = open(fifo_1, 0|O_NONBLOCK))  < 0)  {
		      perror("server: can't open read fifo");
		   }
			if ( (writefd[i] = open(fifo_2, 1))  < 0)  {
	         perror("server: can't open write fifo");
	      }
			printf("B:AAAA\n");
		}
		else {
			perror("fork:failed");
		}
	}
	if(p==0) {
		worker(read_fd,write_fd);
		return 0;
	}
 else {
	jobExecutor(writefd,readfd,w);
	for(int i=0;i<w;i++){
		char fifo_1[32],fifo_2[32];
		int status;

		sprintf(fifo_1,"/tmp/fifo1_%d",i);
		sprintf(fifo_2,"/tmp/fifo2_%d",i);
		waitpid(workers[i],&status,0);
		close(writefd[i]);
		close(readfd[i]);
		unlink(fifo_1);
		unlink(fifo_2);
	}
}
close(write_fd);
close(read_fd);
   return 0;
}
