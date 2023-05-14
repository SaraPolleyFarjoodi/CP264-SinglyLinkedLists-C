/*
-------------------------------------
File:    myrecord_sllist.h
-------------------------------------
-------------------------------------
 */

#ifndef SLLIST_H
#define SLLIST_H

typedef struct {
  char name[40];
  float score;
} RECORD;

typedef struct node {
    RECORD data;
    struct node *next;
} NODE;

typedef struct sllist {
    int length; //the number of nodes of the linked list
    NODE *start;
} SLLIST;

NODE *search(SLLIST *sllistp, char *name);
void insert(SLLIST *sllistp, char *name, float score);
int delete(SLLIST *sllistp,  char *name);
void clean(SLLIST *sllistp);

#endif
