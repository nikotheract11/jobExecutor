#ifndef _INCL_GUARD
#define _INCL_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  k_1 1.2
#define b 0.75
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*uint N;     // Number of texts
uint *D; */   // Array with the number of words from each text
struct t_node *t;  // Trie root node
char ***str; // the map to the texts
char **files; // paths for each worker

FILE *fp; // log file


struct t_node *t;  // Trie root node

struct list{
   char *path;
   struct list *next;
};

typedef struct list list;

int insertAtStart(char *p,list **l);

struct pair {     // A struct using to store id and score in the heap
   int text_id;
   int score;
};

struct st {    // A struct to store position and length of words for nice print
	int pos;
	int len;
};

struct p_list{    // this is a posting list node
   int line;      // the line that the word is found
   int text_id;   // the id of the text
   int freq;
   char *path;    // full path to the file


   int plen;      // the length of the plist only found on the first element of each p_list
   struct p_list *next;
};

struct t_node {   // this is a trie node
   struct t_node *child;
   struct t_node *sibling;
   char value;
   struct p_list *plist;

};

struct query {
   char *operation;
   char **args;
   int argc;
};

typedef struct query query;
typedef struct pair pair;
typedef struct t_node t_node;
typedef struct p_list p_list;
typedef unsigned int uint;

// trie.c
int p_init(p_list **,int ,int );
int t_init(t_node **);
int get_terminal_width();
void pr(char *,char **,int,int,int);
int addplist(t_node **, int ,int);
int append(t_node **,const char *,int,int);
int insert(const char *,int,int);
char** get(const char*);
int getLinesNumber(FILE *);
int pr_trie(t_node *,char *,int );
void form_pr(char *,char **,int,int);

// api.c
p_list * find(const char *);
double score(int ,p_list *);
int init(const char*);
void mfree();
void p_free(p_list **);
void t_free(t_node **);

// app_functions.c
int mygetopt(int , char * const arg[], int *,char **);
double avgdl();
double idf(int);
double score(int n,p_list *);
int interface(int*,int*,int);
int digit_precision();
char * search(char** s,int j,char *k);
char * operate(char **,int,char *);
char * readMsg(int *rfd,int w);
int sendMsg(char *buf,int *wr,int w);
char **getok(char* s,int *k);
int wc(char ***s);

#endif
