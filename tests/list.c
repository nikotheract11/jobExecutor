#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list{
   char *path;
   struct list *next;
};

typedef struct list list;

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

   list *i = *l;
   while(i!=NULL){
      printf("%s\n",i->path );
      i = i->next;
   }
   return 0;

}
int main(int argc, char const *argv[]) {
   list *l=NULL;
   insertAtStart("/tmp",&l);
   printf("================\n");
   insertAtStart("~/home",&l);

   return 0;
}
