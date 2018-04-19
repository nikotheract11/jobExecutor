#include  <sys/types.h>
#include  <sys/stat.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define PERMS   0666

int main(){
	int readfd[5],writefd[5],w=5;
   for(int i=0;i<w;i++){
      char fifo_r[32],fifo_w[32];

      /*===== Create fifos and open for jobExecutor =====*/
      sprintf(fifo_r,"/tmp/fifor_%d",i);
      sprintf(fifo_w,"/tmp/fifow_%d",i);

      if((mkfifo(fifo_r,PERMS) < 0) && (errno != EEXIST)){
         perror("cant create fifo");
      }
      if((mkfifo(fifo_w,PERMS) < 0) && (errno != EEXIST)){
         perror("cant create fifo");
      }

      if ( (readfd[i] = open(fifo_r, 0|O_NONBLOCK))  < 0)  {
         perror("server: can't open read fifo");
      }

      if ( (writefd[i] = open(fifo_w, 1|O_NONBLOCK))  < 0)  {
         perror("server: can't open write fifo");
      }
   }
   printf("SKATAAAA\n");
}
