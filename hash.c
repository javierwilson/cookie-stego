#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define HASHSIZE 100

typedef struct _node{
  int name;
  int desc;
  struct _node *next;
}node;

static node* hashtab[HASHSIZE];

void inithashtab(){
  int i;
  for(i=0;i<HASHSIZE;i++)
    hashtab[i]=NULL;
}

// ????????????
unsigned int hash(int s){
  unsigned int h=0;
  //for(;s;s++)
  //  h=s+h*31;
  return h%HASHSIZE;
}

node* lookup(int n){
  unsigned int hi=hash(n);
  printf("hi = %d", hi);
  node* np=hashtab[hi];
  for(;np!=NULL;np=np->next){
    if(np->name == n)
      return np;
  }
  return NULL;
}

int hash_get(int name){
  node* n=lookup(name);
  if(n == NULL)
    return 0;
  else
    return n->desc;
}

int hash_set(int name,int desc){
  unsigned int hi;
  node* np;
  if((np=lookup(name))==NULL){
    hi=hash(name);
    np=(node*)malloc(sizeof(node));
    if(np==NULL)
      return 0;
    np->name=name;
    np->next=hashtab[hi];
    hashtab[hi]=np;
  }
  np->desc=desc;
  return 1;
}

void cleanup(){
  int i;
  node *np,*t;
  for(i=0;i<HASHSIZE;i++){
    if(hashtab[i]!=NULL){
      np=hashtab[i];
      while(np!=NULL){
	t=np->next;
	free(np);
	np=t;
      }
    }
  }
}

void hash_display(){
  int i;
  node *t;
  for(i=0;i<HASHSIZE;i++){
    if(hashtab[i]==NULL)
      printf("()");
    else{
      t=hashtab[i];
      printf("(");
      for(;t!=NULL;t=t->next)
        printf("(%d,%d) ",t->name,t->desc);
      printf(")");
    }
  }
}

/* // sample code
main(){
  int i;
  int names[]={2,4,6,8,10};
  int descs[]={22,44,66,88,100};

  inithashtab();
  for(i=0;i<5;i++)
    hash_set(names[i],descs[i]);
  printf("4 = %d \n",hash_get(4));
  hash_set(30,333);
  printf("30 = %d and 8 = %d \n",hash_get(30),hash_get(8));
  cleanup();
  return 0;
}
*/
