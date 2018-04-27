/* this is the file "PQInterface.h"      */

#include "PQTypes.h"
/* defines types PQItem and PriorityQueue */

void Initialize (PriorityQueue *,int);
int Empty (PriorityQueue *);
int Full (PriorityQueue *);
void Insert (pair*, PriorityQueue *);
pair* Remove (PriorityQueue *);
