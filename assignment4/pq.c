/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Gabriel de Leon
 * Email: deleong@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* arr;
	int priority;
};
 
/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {

	struct pq* pq = malloc(sizeof(struct pq));
	
	pq->arr = dynarray_create();
	pq->priority = 100;

	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	
	dynarray_free(pq->arr);
	
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {

	if(dynarray_size(pq->arr) == 0) {
		return 1;
	}

	return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {

	//updates priority of the queue if needed
	if(priority < pq->priority || pq_isempty(pq)) {
		pq->priority = priority;
	}

	//adds node to the end of the array
	dynarray_insert(pq->arr, value);

	//sorts through the pq
	dynarray_heapify(pq->arr);
}	


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	return dynarray_get(pq->arr, 0);
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	return pq->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {

	//checks if pq is empty
	if(pq_isempty(pq)) {
		return NULL;
	}

	//keeps track of removed value
	void* returned = dynarray_get(pq->arr, 0);

	//keeps track of removed val
	void* removed = dynarray_get(pq->arr, dynarray_size(pq->arr) - 1);

	//removes the last node in the array
	dynarray_remove(pq->arr, dynarray_size(pq->arr) - 1);

	//checks if the root was removed
	if(dynarray_size(pq->arr) == 0) {
		pq->priority = 0;
		return returned;
	}

	//sets the new root to the replacement node
	dynarray_set(pq->arr, 0, removed);

	//sorts through the pq
	dynarray_heapify(pq->arr);

	//makes priority equal to the new lowest priority
	pq->priority = *(int*)dynarray_get(pq->arr, 0);

	return returned;
}
