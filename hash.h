void inithashtab();
unsigned int hash(int s);
typedef struct _node{
  int name;
  int desc;
  struct _node *next;
}node;
int hash_get(int name);
int hash_set(int name,int desc);
void cleanup();
void hash_display();
node* lookup(int n);
