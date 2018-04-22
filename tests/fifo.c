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

#define MAXBUFF 1024
#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"
#define PERMS   0666

int jobExecutor(int *wr){
	FILE *fp = fopen("./docfile","r");
	size_t a;
   char *str=NULL;
	int counter=0;
	struct pollfd pfds[5];

   while(!feof(fp)){
		for(int i=0;i<5;i++){
			pfds[i].fd = wr[i];
			pfds[i].events = POLLOUT;
		}
		poll(pfds,5,-1);

      int l=getline(&str,&a,fp);
      str[l-1]='\0';
		if(l>=0 && (pfds[counter%5].revents & POLLOUT))
			if(write(wr[counter++%5],str,l+1)<=0) perror("write error");
   }
//	write(wr[0],"Bravo mori panatha",strlen("bravo mori panatha")+1) ;
	poll(pfds,5,-1);
	for(int i=0;i<5;i++){
		pfds[i].fd = wr[i];
		pfds[i].events = POLLOUT;}

		poll(pfds,5,-1);
for(int i=0;i<5;i++){
		if(pfds[i].revents & POLLOUT)
			write(wr[i],"stop",strlen("stop")+2);
	//	poll(pfds,5,-1);
	}
	return 0;
}

int worker(int rfd){
	int n;
	char buff[256];
	struct pollfd pfds[1];

	while(1){
		pfds[0].fd=rfd;
		pfds[0].events = POLLIN;
		poll(pfds,1,-1);
		if(pfds[0].revents & POLLIN){
		if((n=read(rfd,buff,MAXBUFF))>0){
			buff[n]='\0';
			printf(" : %s\n",buff );
			if(!strcmp(buff,"stop")) return 0;
		}
	}
	}
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
	if(p==0) worker(read_fd);
else {
	jobExecutor(writefd);
	for(int i=0;i<5;i++){
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
   exit(0);
}
