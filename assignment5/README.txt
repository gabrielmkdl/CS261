1. Gabriel de Leon deleong@oregonstate.edu

2. My program implements a struct that represents a hash table. The hash table has 8 functions: a function that creates the hash tables, one that frees the hash table,
one that checks if the hash table is empty, one that outputs the size of the hash table, one that calculates the hash of a key and finds the next open spot by probing 
through double hashing and open addressing, one that inserts a value into the hash table, one that looks up if a key is in the hash table, and one that removes a value
and replaces it with a 'tombstone' value. I used dynamic arrays to represent the hash table and used open addressing to address collision as well as double hashing to 
help with the load factor. A hash table basically is a table that has a key where a certain value will go through this conversion function that makes that value have a 
certain key. Using that key, I am able to directly find where in the array the value is which causes the average runtimes to be O(1). If two values have the same key, I 
would use probing through double hashing to figure out the next spot of the value. 

3. To compile and run this file, make sure you are in the correct directory as the assignment. Then all you have to do is type "make" and then either "valgrind test_ht" or just
"test_ht" depending on if you want to see if there are any memory leaks/errors in the code. From there, the code will run on its own as it has its own test file which tests all
the functions in the hash table. 

4. Limitations: The only limitation of this program is that a separate file has to be included that runs the program correctly. The program itself won't run unless the 
test_hash_table.c file is correctly implemented and properly tests the functionality of the hash table struct I made. Without that file, nothing will be ran only a struct would be 
implemented, but not tested. 
