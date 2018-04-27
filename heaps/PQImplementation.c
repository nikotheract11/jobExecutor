/* This is the file "PQImplementation.c" */

#include "PQInterface.h"

void Initialize(PriorityQueue *PQ,int n)
{
    PQ->Count=0;
    PQ->ItemArray = (pair**) malloc(n*sizeof(pair*));
}

int Empty(PriorityQueue *PQ)
{
    return(PQ->Count==0);
}

void Insert(pair *Item, PriorityQueue *PQ)
{
    int ChildLoc;
    int ParentLoc;

    (PQ->Count)++;
    ChildLoc=PQ->Count;
    ParentLoc=ChildLoc/2;
    while (ParentLoc != 0){
       if (Item->score <= PQ->ItemArray[ParentLoc]->score){
         PQ->ItemArray[ChildLoc]=Item;
         return;
       } else {
         PQ->ItemArray[ChildLoc]=PQ->ItemArray[ParentLoc];
         ChildLoc=ParentLoc;
         ParentLoc=ParentLoc/2;
       }
    }
    PQ->ItemArray[ChildLoc]=Item;
}


pair* Remove(PriorityQueue *PQ)
{
   int CurrentLoc;
   int ChildLoc;
   pair *ItemToPlace;
   pair *ItemToReturn;

   if(Empty(PQ)) {
      free(PQ);
      return NULL;
   }

   ItemToReturn=PQ->ItemArray[1];
   ItemToPlace=PQ->ItemArray[PQ->Count];
   (PQ->Count)--;
   CurrentLoc=1;
   ChildLoc=2*CurrentLoc;

   while (ChildLoc <= PQ->Count){
       if (ChildLoc < PQ->Count){
          if (PQ->ItemArray[ChildLoc+1]->score > PQ->ItemArray[ChildLoc]->score){
             ChildLoc++;
          }
       }
       if (PQ->ItemArray[ChildLoc]->score <= ItemToPlace->score){
           PQ->ItemArray[CurrentLoc]=ItemToPlace;
           return(ItemToReturn);
       } else {
           PQ->ItemArray[CurrentLoc]=PQ->ItemArray[ChildLoc];
           CurrentLoc=ChildLoc;
           ChildLoc=2*CurrentLoc;
       }
    }
    PQ->ItemArray[CurrentLoc]=ItemToPlace;

    return(ItemToReturn);
}
