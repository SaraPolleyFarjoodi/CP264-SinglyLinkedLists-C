/*
-------------------------------------
File:    myrecord_sllist.c
-------------------------------------
-------------------------------------
 */


/**
 * RECORD structure
 * name  - char array for person's name
 * score - float score of record
 */

/**
 * NODE structure
 * data  - RECORD data
 * next  - pointer pointing to the next node of singly linked list
 */

/**
 * Singly linked list structure
 * int length  - the number of nodes of the linked list
 * NODE *start - pointer pointing to the first node of the linked list
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "myrecord_sllist.h"


/**
 * Search singly linked list by name key.
 * @param SLLIST *sllistp provides the address of a singly linked list structure.
 * @param name Key to search
 * @return Pointer to found node if found; otherwise NULL
 */
NODE *search(SLLIST *sllistp, char *name) {
	NODE *p = sllistp->start; //start at the beginning of the list when searching

	int result;

	while(p != NULL){
		result = strcmp(p->data.name, name); //if names are equal, result = 0;
		if(result == 0){ //found pattern: name matches
			return p; //action: return the node address
		}
		p = p->next;
	}

	return NULL; //not found

}


/**
 * Insert new record to linked list at the position sorted by record name field.
 * @param @param SLLIST *sllistp provides the address of a singly linked list structure.
 * @param name The name data of new record.
 * @param score The score data of new record
 */
void insert(SLLIST *sllistp, char *name, float score) {
	//create new node
	NODE *np = (NODE*)malloc(sizeof(NODE));

	//populate information for new node
	strcpy(np->data.name, name); //populate name of new node with name given
	np->data.score = score;
	np->next = NULL;

	NODE *prev = NULL; //use prev to keep track of the node previous to the one pointed to by p since we have to insert before p
	NODE *p = sllistp->start; //pointer to beginning of list of nodes

	int result;


	while(p != NULL){
		result = strcmp(p->data.name, name); //strcmp(str1, str2). if str1 = str2, result = 0. If str1 > str2, result > 0. If str1 < str2, result < 0.
		if(result >= 0 ){ //pattern.
			break; //action, more break after
		}
		prev = p; //let prev equal to the next node which would be p
		p = p->next; //move to next
	}

	if(prev == NULL){ //if linked list turns out to be empty or if key is found at start of list
		//insert at beginning
		sllistp->start = np; //set start of linked list to new node
		np->next = p; //link new node to what used to be the first node in the linked list
	}else{
		prev->next = np; //point to new node
		np->next = p; //have new node point to node after it
	}

	sllistp->length++; //add one to linked list because we added a node

}


/**
 * Delete a node of record matched by the name key from linked list.
 * @param @param SLLIST *sllistp provides the address of a singly linked list structure.
 * @param name  The key used to find the node for deletion.
 * @return 1 if deleted a matched node, 0 otherwise.
 */
int delete(SLLIST *sllistp, char *name) {
	NODE *prev = NULL;
	NODE *p = sllistp->start;
	int result = 1;

	while(p != NULL){
		result = strcmp(p->data.name, name); //if names are equal, result = 0;
		if(result == 0){ //pattern: name matched
			break; //action, more after the break
		}

		prev = p; //save p to prev
		p = p->next; //move to next
	}


	if(prev == NULL){ //first node is matches, first node will be deleted
		sllistp->start = p->next; //reset the start of the linked list since we are deleting the first node

	}else{
		prev->next = p->next; //cut off linkage to node p because we want to delete node p
	}

	free(p); //release the memory block of the deleted node. free the space so there's no leakage
	sllistp->length--; //subtract one from linked list because we added a node

	if(result == 0){ //means that a node was found with equivalent name to the key
		return 1; //return 1 to indicate that a matched node was found and was deleted
	}else{
		return 0; //return 0 if a matched node was not deleted b/c it was not found in linked list
	}

}


/**
 * Clean singly linked list, delete and free all nodes, set start to NULL, and length to 0
 * @param SLLIST *sllistp provides the address of a singly linked list structure.
 */
void clean(SLLIST *sllistp) {
	NODE *temp, *p = sllistp->start;

	while(p != NULL){ //stop when we've reached end of linked list
		temp = p;
		p = p->next; //move to next node in linked list
		free(temp); //free memory block of current node
	}

	sllistp->start = NULL; //set start to NULL
	sllistp->length = 0; //set length to 0

}

