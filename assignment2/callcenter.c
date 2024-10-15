/*********************************************************************
** Program Filename: callcenter.c
** Author: Gabriel de Leon
** Date: 4/28/24
** Description: Simulates a callcenter using stacks and queues from previous parts
** Input: user input on name, call reason, what they want to do
** Output: callcenter simulation
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct, and call center struct here.
 */
struct call {
	int ID;
	char name[256]; 
	char reason[256]; 
};

struct call_center {
	struct stack* stack; //stack of calls answered
	struct queue* queue; //queue of phone calls
	int answered; //number of calls answered
	int need_answered; //number of calls in the queue
};

/*********************************************************************
** Function: create_cal()
** Description: allocates memory for a call struct
** Parameters: none
** Pre-Conditions: none
** Post-Conditions: call struct is made
*********************************************************************/
struct call* create_call() {

	struct call* call = malloc(sizeof(struct call));
	call->ID = 0;
	call->name[0] = '\0';
	call->reason[0] = '\0';
	return call;
}


/*********************************************************************
** Function: create_cc()
** Description: allocates memory for a callcenter struct
** Parameters: none
** Pre-Conditions: none
** Post-Conditions: callcenter struct is made
*********************************************************************/
struct call_center* create_cc() {

	struct call_center* call_center = malloc(sizeof(struct call_center));
	call_center->queue = queue_create(); //creates queue
	call_center->stack = stack_create(); //creates stack
	call_center->answered = 0;
	call_center->need_answered = 0;
	return call_center;
}


/*********************************************************************
** Function: delete_cc()
** Description: clears all the memory allocated in the callcenter
** Parameters: call_center - callcenter we want to free the memory of
** Pre-Conditions: call_center is properly allocated and isn't NULL
** Post-Conditions: frees all the data
*********************************************************************/
void delete_cc(struct call_center* call_center) {

	//frees all the call structs in the queue
	while(queue_isempty(call_center->queue) != 1) {
		free(queue_dequeue(call_center->queue));
	}

	//frees all the call structs in the stack
	while(stack_isempty(call_center->stack) != 1) {
		free(stack_pop(call_center->stack));
	}	

	queue_free(call_center->queue);
	stack_free(call_center->stack);
	free(call_center);
}


/*********************************************************************
** Function: print_call()
** Description: prints the information of the call
** Parameters: call - call we want to print the info of
** Pre-Conditions: call is properly defined and isn't NULL
** Post-Conditions: the call's information is printed out
*********************************************************************/
void print_call(struct call* call) {
	
	printf("Call ID: %d\n", call->ID);
	printf("Caller's name: %s\n", call->name);
	printf("Call reason: %s\n\n", call->reason);
}


/*********************************************************************
** Function: new_call()
** Description: take user inputs and creates call item and adds it to the queue
** Parameters: call_center - callcenter we want to add the call to, count - number to help track ID
** Pre-Conditions: call center is properly allocated 
** Post-Conditions: call is added to the queue and call center
*********************************************************************/
void new_call(struct call_center* call_center, int *count) {

	//creates call
	struct call* temp = create_call();
	temp->ID = *count;

	//takes in user's input and puts it into the call item
	printf("Enter caller's name: ");
	scanf("%255s", &temp->name);

	printf("\nEnter call reason: ");
	scanf("%255s", &temp->reason);

	//adds to the queue
	queue_enqueue(call_center->queue, temp);	

	(*count)++;
	call_center->need_answered++;

	printf("\nThe call has been successfully added to the queue!\n\n");
}


/*********************************************************************
** Function: answer_call()
** Description: remove the first call in the queue and adds it to the stack 
** Parameters: call_center - call_center we want to answer the first call of
** Pre-Conditions: call_center is properly allocated
** Post-Conditions: first call in queue is answered and added to the stack
*********************************************************************/
void answer_call(struct call_center* call_center) {
	
	//checks if there is any calls to answer
	if(queue_isempty(call_center->queue) == 1) {
		printf("No more calls need to be answered at the moment!\n\n");
	}

	else {
		
		//adds to stack
		stack_push(call_center->stack, queue_front(call_center->queue));

		printf("The following call has been answered and added to the stack!\n\n");
		
		//prints the call and removes it from the queue
		print_call(queue_dequeue(call_center->queue));

		call_center->answered++;
		call_center->need_answered--;
	}
}


/*********************************************************************
** Function: stack()
** Description: displays the number of calls answered and last call on the stack
** Parameters: call_center - call_center we want to get the stats from
** Pre-Conditions: call_center is properly allocated
** Post-Conditions: stats of the stack is printed out
*********************************************************************/
void stack(struct call_center* call_center) {

	printf("Number of calls answered: %d\n", call_center->answered);
	
	//checks if there were any calls answered
	if(call_center->answered != 0) {
		printf("Details of the last call answered:\n\n");
		print_call(stack_top(call_center->stack));
	}
}


/*********************************************************************
** Function: queue()
** Description: prints out number of calls in the queue and the details of the first call
** Parameters: call_center - call_center we want to get the stats from
** Pre-Conditions: call_center is properly allocated
** Post-Conditions: stats of the queue is printed out
*********************************************************************/
void queue(struct call_center* call_center) {

	printf("Number of calls to be answered: %d\n", call_center->need_answered);

	//checks if there are any calls in the queue
	if(call_center->need_answered != 0) {
		print_call(queue_front(call_center->queue));
	}
}


/*********************************************************************
** Function: path()
** Description: depending on user's choice, it calls on the function to run that option
** Parameters: input - option that user wanted to do, call_center - call_center that we want to do things on, count - helps keep track of ID #
** Pre-Conditions: call_center is properly allocated, input is between 1-5 inclusive
** Post-Conditions: option user picked is played out
*********************************************************************/
void path(int input, struct call_center* call_center, int *count) {
	
	if(input == 1) {
		new_call(call_center, count);
	}

	else if(input == 2) {
		answer_call(call_center);
	}

	else if(input == 3) {
		stack(call_center);
	}

	else if(input == 4) {
		queue(call_center);
	}

	return;
}


/*********************************************************************
** Function: main()
** Description: where all the options are printed and program starts to run
** Parameters: none 
** Pre-Conditions: none
** Post-Conditions: program is ran until user quits
*********************************************************************/
int main() {

	int input = 0;
	int count = 1;
	struct call_center* call_center = create_cc();
	
	//keeps running until user quits out
	do{

		printf("1. Receive a new call \n");
		printf("2. Answer a call \n");
		printf("3. Current state of the stack - answered calls \n");
		printf("4. Current state of the queue - calls to be answered \n");
		printf("5. Quit \n");
		printf("Choose an option: ");
		scanf("%d", &input);		
		printf("\n");

		//runs the option that the user wants
		path(input, call_center, &count);

	}while(input != 5);

	printf("Bye! \n");

	//frees all the memory allocated
	delete_cc(call_center);
}	