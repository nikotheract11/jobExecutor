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

int main(int argc, char const *argv[]) {
   mkfifo(FIFO1,PERMS);
   mkfifo(FIFO2,PERMS);

   pid_t p;

   if((p=fork()) == 0){
      int rfd = open(FIFO1,O_RDONLY);
      int wfd = open(FIFO2,O_WRONLY);
      int k=0;

      while(k++<2){
         char *buf = "Kalimera nikos trele";
         sendMsg(buf,&wfd,1);
         printf("333333\n");
         readMsg(&rfd,1);
         printf("44444\n");
      }
   }
   else {
      int wfd = open(FIFO1,O_WRONLY);
      int rfd = open(FIFO2,O_RDONLY);

      int k=0;

      while(k++<2){
      readMsg(&rfd,1);
      char *buf = "Kalispera nasa ansdasd n ";
      printf("11111\n");
      sendMsg(buf,&wfd,1);
      printf("22222\n");
   }
   }
   return 0;
}
