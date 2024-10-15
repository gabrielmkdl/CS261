/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Gabriel de Leon
 * Email: deleong@oregonstate.edu
 */

#include <stdlib.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"


/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
struct ht {
    struct dynarray* arr; 
    double size;
};


/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));
    ht->arr = dynarray_create();
    ht->size = 0;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    dynarray_free(ht->arr);
    free(ht);
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){

    if(ht->size == 0) {
        return 1;
    }

    return 0;
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 *
 * Params:
 *   ht - the hash table whose size is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return the size of the given hash table.
 */
int ht_size(struct ht* ht){
    return ht->size;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash function is passed in as a function pointer, stored in 'convert'
 * Assuming the passed in hash function is well designed.
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element used to calculate the index
 *   convert - a pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the index value of 'key' in the hash table .
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
    
    //creates the index 
    int hash = convert(key) % dynarray_capacity(ht->arr);

    //sets up double hashing
    int j = 1; 

    //while the index isn't empty 
    while(dynarray_get(ht->arr, hash) != NULL  && dynarray_get(ht->arr, hash) != "__TS__") {
        
        //checks if key already exists
        if(*(int*)dynarray_get(ht->arr, hash) == *(int*)key) {
            return -1;
        }

        //create a new index
        hash = (hash + j * convert(key)) % dynarray_capacity(ht->arr); 
        
        //in case need to probe again
        j++;
    }

    return hash;
}


/*
 * This function should insert a given element into a hash table with a
 * specified key. Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){       
    
    //creates the index
    int hash = ht_hash_func(ht, key, convert);

    //if hash = -1 then key already exists 
    if(hash == -1) {
        return;
    }

    //insert the value at the calculated index
    dynarray_insert(ht->arr, hash, value);

    //keeps track of size of hash table/array
    ht->size++;
    dynarray_change_size(ht->arr, ht->size);

    //calculates load factor and rehashes every element if needed
    if(((double)ht->size / dynarray_capacity(ht->arr)) >= 0.75) {
        dynarray_load(ht->arr, convert);
    }
}


/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the value of the corresponding 'key' in the hash table .
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
    
    //creates index of where the key should be
    int hash = convert(key) % dynarray_capacity(ht->arr);

    //sets up double hashing probing if needed
    int j = 1;

    //iterate over the hash table's internal array to find the elemtn
    for(int i = 0; i < dynarray_capacity(ht->arr); i++) {

        //if key is the same
        if(*(int*)dynarray_get(ht->arr, hash) == *(int*)key) {
            return dynarray_get(ht->arr, hash);
        }

        //if the probed spot is NULL then the key isn't in the hash table
        else if(dynarray_get(ht->arr, hash) == NULL) {
            return NULL;
        }

        else {
            //probes next spot
            hash = (hash + j * convert(key)) % dynarray_capacity(ht->arr);

            //if we need to probe again
            j++;
        }
    }
    return NULL;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){

    //index of the key
    int hash = convert(key) % dynarray_capacity(ht->arr);

    //checks if there is a value at the specified index
    while(dynarray_get(ht->arr, hash) != NULL) {
        
        //sets up double hashing if needed
        int j = 1;

        //checks to see if the value is the same as the key
        if(dynarray_get(ht->arr, hash) == key) {

            //makes tombstone value
            dynarray_set(ht->arr, hash, "__TS__");

            break;
        }
        
        else {
            //probes next spot
            hash = (hash + j * convert(key)) % dynarray_capacity(ht->arr);

            //in case need to probe again
            j++;
        }
    }   

    //keeps track of hash table/array size
    ht->size--;
    dynarray_change_size(ht->arr, ht->size);
} 
