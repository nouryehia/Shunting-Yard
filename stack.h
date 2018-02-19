#include <iostream> //Includes.
#include <cstring>

using namespace std;


#ifndef STACK_H //If undefined
#define STACK_H

//Create a stack template so it can be used for char and trNode*
template <class T>
class stack {
	protected:

		//Declare node.
		struct node{ 
			T data;
			node* next;
		};
		node* head;
	public:

		//Declare functions.
		stack();
		void push(T data);
		T pop();
		T peek();
		bool isEmpty();
};

//Template wouldn't work unless I put my functions in .h file and got rid of .cpp file.

//Constructor.
template <class T>
stack<T>::stack(){
	head = NULL;
}

//Pushes to the stack.
template <class T>
void stack<T>::push(T data){
	node* toPush = new node;
	toPush->data = data;
	toPush->next = head;
	head = toPush;
}

//Pops from the stack.
template <class T>
T stack<T>::pop(){
	if(isEmpty()){
		cout<< "Trying to pop from an empty stack!!\n";
		return (T) NULL;
	}
	node* temp = head;
	head = head->next;
	T data = temp->data;
	delete temp;
	return data;
}

//Peeks at the stack.
template <class T>
T stack<T>::peek(){
	if(isEmpty()){
		cout<< "Trying to peek from an empty stack!!\n";
		return (T) NULL;
	}
	T data = head->data;
	return data;
}

//Checks if stack is empty. 
template <class T>
bool stack<T>::isEmpty(){
	return (head==NULL);
}
#endif

