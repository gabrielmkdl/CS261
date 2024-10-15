/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Gabriel de Leon
 * Email: deleong@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

struct path {
	int node;
	int cost;
	struct path* prev;
};

int main(int argc, char const *argv[]) {
	/*.
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);
	

	//create a 2D array to store edge costs between nodes
	int arr[n_nodes][n_nodes];
	
	//initalize all the values to 0
	for(int i = 0; i < n_nodes; i++) { 
		for(int j = 0; j < n_nodes; j++) {
			arr[i][j] = 0;
		}
	}

	//keeps track of row, col, and cost
	int x, y, cost;
	
	//transfers all the edge values between the nodes
	for(int h = 0; h < n_edges; h++) {
		fscanf(file, "%d %d %d", &x, &y, &cost);
		arr[x][y] = cost;
	}

	//array that holds the distance of the path
	struct path* paths[n_nodes];

	//initalizes values 
	for(int i = 0; i < n_nodes; i++) {
		paths[i] = malloc(sizeof(struct path));
		paths[i]->node = i;
		paths[i]->cost = 9999; 
		paths[i]->prev = NULL;
	}
	
	//create priority queue and add first node 
	struct pq* Q = pq_create();
	struct path* start = malloc(sizeof(struct path));
	start->node = START_NODE;
	start->prev = NULL;
	start->cost = 0;
	pq_insert(Q, start, 0);


	//Dijkstra's algorithm
	while(!pq_isempty(Q)) {
		struct path* curr = pq_first(Q);
		pq_remove_first(Q);

		//if the path hasn't been traversed yet
		if(paths[curr->node]->cost == 9999) {
			
			struct path* temp = paths[curr->node];
			paths[curr->node] = curr;
			free(temp);

			//explore all the neighbors of the current node
			for(int i = 0; i < n_nodes; i++) {
				if(arr[curr->node][i] != 0) {
					int total_cost = curr->cost + arr[curr->node][i];

					struct path* next = malloc(sizeof(struct path));
					next->node = i;
					next->prev = curr;
					next->cost = total_cost;

					//insert neighbor into priority queue
					pq_insert(Q, next, total_cost);
				}
			}
		}	
		//shortest path was already found
		else {
			free(curr);
		}
	}

	//print the costs and previous nodes for each node
	for(int i = 0; i < n_nodes; i++) {
		int prev_node;
		if(paths[i]->prev == NULL)
			prev_node = 0;

		else
			prev_node = paths[i]->prev->node;
			
		printf("Cost to node %d: %d -- Previous node: %d\n", i, paths[i]->cost, prev_node);
	} 

	//free allocated memory
	for(int i = n_nodes - 1; i >=0; i--) {
		free(paths[i]);
	}
	pq_free(Q);

	fclose(file);

    return 0;
}
