#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

static void put(Rep r, End e, Data d) {
  Node n = malloc(sizeof(*n));
  if (!n) ERROR("malloc() failed");
  n->data = d;

  if (e == Head) {
    n->np[Head] = 0;
    n->np[Tail] = r->ht[Head];
    if (r->ht[Head]) {
      r->ht[Head]->np[Head] = n;
    } else {
      r->ht[Tail] = n;
    }
    r->ht[Head] = n;
  } else if (e == Tail) {
    n->np[Tail] = 0;
    n->np[Head] = r->ht[Tail];
    if (r->ht[Tail]) {
      r->ht[Tail]->np[Tail] = n;
    } else {
      r->ht[Head] = n;
    }
    r->ht[Tail] = n;
  }
  r->len++;
}

static Data ith(Rep r, End e, int i) { 
  if (i < 0 || i >= r->len) {
      return 0; 
  }
  int iter;
  if (e == Head) { //head
    Node current = r->ht[Head];
    for (iter = 0; iter < i; iter++) {
      current = current->np[Tail];
    }
    return current->data;
  } else if (e == Tail) { //tail
    Node current = r->ht[Tail];
    for (iter = 0; iter < i; iter++) {
      current = current->np[Head];
    }
    return current->data;
  } else {
    return 0;
  }
}

static Data get(Rep r, End e) {
  if (r->len <= 0 || r->ht[Head] == NULL || r->ht[Tail] == NULL) {
      return 0; 
  }
  //if head
  if (e == Head) {
    //grab head and assign returnNode to have it's pointer
    Node returnNode = r->ht[Head];
    //update rep's head
    r->ht[Head] = returnNode->np[Tail];
    // remove pointer
    returnNode->np[Tail]->np[Head] = NULL;
    // Make len shorter
    r->len--;
    return returnNode->data;
  } else if (e == Tail) {
    //grab tail and assign returnNode to have it's pointer
    Node returnNode = r->ht[Tail];
    //update rep's tail
    r->ht[Tail] = returnNode->np[Head];
    // remove pointer
    returnNode->np[Head]->np[Tail] = NULL;
    // Make len shorter
    r->len--;
    return returnNode->data;
  } else {
    return 0;
  }
}

static Data rem(Rep r, End e, Data d) {
  //Loop through from selected end to find equal node data
  //If head
  Node current;
  if (e == Head) {
    Node current = r->ht[Head];
    while (current->data != d) {
      if (current->np[Tail] == NULL) {
        return 0;
      }
      current = current->np[Tail];
    }
  } else if (e == Tail) {
    Node current = r->ht[Tail];
    while (current->data != d) {
      if (current->np[Head] == NULL) {
        return 0;
      }
      current = current->np[Head];
    }
  } else {
    //e isnt valid
    return 0;
  }

  //Found the correct node
  //check if end? if so use get function
  if (current == r->ht[Head] || current == r->ht[Tail]) {
    return get(r, e);
  }
  //if in mid
  //Fix head of current
  current->np[Head]->np[Tail] = current->np[Tail];
  //Fix tail of current
  current->np[Tail]->np[Head] = current->np[Head];
  //reduce len
  r->len--;

  //return current data
  return current->data;
  }

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}