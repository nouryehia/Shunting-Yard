/* Shunting Yard Algorithm
 * Author: Nour Yehia
 * Date: 2 - 18 - 18
 * This program reads in an expression in infix notation and allows the user to output it in infix, prefix, or postfix notation. */

#include "stack.h" //Include.

//Char node.
struct node{
	char data;
	node* next;
};

//Token node (to hold multiple digits).
struct tNode{
	char data[8];
	tNode* next;
};

//Node used to build the tree.
struct trNode{
	char data[8];
	trNode *left, *right;
};

//Declaration of functions.
bool spaces(char*);
tNode* toPostfix(char*);
bool isOperator(char);
node* addToLL(char, node*);
int precedence(char);
void printPostfix(tNode*);
trNode* binaryTree(tNode*);
trNode* newNode(char*);
void printPrefix(trNode*);
void printInfix(trNode*);
void printPostfixFromTree(trNode*);

int main(){

	char expression[80];
	char yesno[80];

	//Prompt user for expression.
	cout << endl << "Enter a mathematical expression in infix notation." << endl
		 << "Tokens must be separated by a space." << endl << endl;
	cin.getline(expression, 80);

	//Make sure expression is in infix notation (somewhat).
	while (!isdigit((int)expression[0]) && expression[0] != '('){
		cout << endl << "Please enter your expression in infix notation." << endl << endl;
		cin.getline(expression, 80);
	}

	//Make sure that tokens are spearated by spaces.
	while (!spaces(expression)){
		cout << endl << "Tokens must be separated by a space." << endl << endl;
		cin.getline(expression, 80);
	}

	tNode* PF = toPostfix(expression);

	cout << endl << "Postfix notation (Shunting Yard Algorithm): ";
	printPostfix(PF);

	cout << endl << "Would you like to print your expression in prefix, infix, and postfix notation?" << endl << endl;
	cin.getline(yesno, 80);

	while (strcmp(yesno, "Y") != 0 && strcmp(yesno, "y") != 0 && strcmp(yesno, "N") != 0 && strcmp(yesno, "n") != 0 && 
		   strcmp(yesno, "Yes") != 0 && strcmp(yesno, "yes") != 0 && strcmp(yesno, "No") != 0 && strcmp(yesno, "no")!= 0 &&
		   strcmp(yesno, "YES") != 0 && strcmp(yesno, "NO") != 0){
		cout << endl << "Enter 'Y' for yes and 'N' for no." << endl << endl;
		cin.getline(yesno, 80);
	}

	while (strcmp(yesno, "Y") != 0 && strcmp(yesno, "y") != 0 && strcmp(yesno, "Yes") != 0 
		   && strcmp(yesno, "yes") != 0 && strcmp(yesno, "YES") != 0){
		cout << endl << "I did not spend all this time on this project for you to say no. Try again!" << endl << endl;
		cin.getline(yesno, 80);
	}

	trNode* BT = binaryTree(PF);

	cout << endl << "Creating binary tree..." << endl << endl << "Transversing binary tree..." << endl;

	cout << endl<< "Prefix notation: ";
	printPrefix(BT);
	cout << endl;

	cout<< endl << "Infix notation: ";
	printInfix(BT);
	cout << endl;

	cout<< endl << "Postfix notation: ";
	printPostfixFromTree(BT);
	cout << endl << endl;;
}

// Checks if non-digit tokens are surrounded by spaces.
bool spaces(char* input){
	int c = 0;
	for (int i = 0; i < strlen(input); i++){
		if (isOperator(input[i]) || input[i] =='(' || input[i] == ')'){
			if (i == 0) {
				if (input[i+1] != ' ') c++;
			}
			else if (i == strlen(input)-1){
				if (input[i-1] != ' ') c++;
			}
			else{
				if (input[i-1] != ' ' || input[i+1] != ' ') c++;
			}
		}
	}
	if (c == 0) return true;
	else return false; 
}

//Converts an expression from infix notation to postfix notation.
tNode* toPostfix(char* input){

	//Declare variables.
	stack<char> stack;
	node* postfix = NULL;
	tNode* tokenList = NULL;
	tNode* tokenLast;
	int c = 0;

	//Implementation of Shunting Yard algorithm.
	//Converts an infix expression to a postfix linked list with each node being a character. 
	for (int i = 0; i<strlen(input); i++) {

		//If digit --> add character to output queue.
		if (isdigit(input[i]) || input[i] == ' ') postfix = addToLL(input[i], postfix);
		
		//If operator --> add any operator on top of stack with higher precedence then push to stack.
		else if (isOperator(input[i])){
			if (stack.isEmpty()) stack.push(input[i]);
			else {
				while ((precedence(stack.peek()) > precedence(input[i]) || (precedence(stack.peek()) == precedence(input[i]) && input[i] != '^')) 
						&& stack.peek() != '(' && !stack.isEmpty()){
					postfix = addToLL(stack.pop(), postfix);
				}
				stack.push(input[i]);
			}
		}

		//If left parenthesis --> push to stack.
		else if (input[i] == '(') stack.push(input[i]);

		//If right parenthesis --> add all operators on top of stack to output queue until right brace is found.
		else if (input[i] == ')'){
			while (stack.peek() != '(') postfix = addToLL(stack.pop(), postfix);
			stack.pop();
		}
	}

	//Add any remaining operator in the stack to output queue.
	while(!stack.isEmpty()){
		postfix = addToLL(' ', postfix);
		postfix = addToLL(stack.pop(), postfix);
	}

	//Creates a postfix linked list with each token as a node (for more than 1 digit numbers).
	//While postfix isn't NULL.
	while (postfix){
		tNode* temp;

		//If operator --> create an operator token.
		if (isOperator(postfix->data)){
			temp = new tNode;
			temp->next = NULL;
			temp->data[0] = postfix->data;
			temp->data[1] = '\0';
			postfix=postfix->next;

			if (!tokenList){
				tokenList = tokenLast = temp;
			}
			else{
				tokenLast->next = temp;
				tokenLast = temp;
			} 
		}

		//If digit --> add this digit to token and any digits that follow until a space is met. 
		else if (isdigit(postfix->data)){
			temp = new tNode;
			temp->next = NULL;
			int cc = 0;
			while (postfix && isdigit(postfix->data)){
				temp->data[cc] = postfix->data;
				postfix = postfix->next;
				cc++;
			}
			temp->data[cc] = '\0';
			if (!tokenList){
				tokenList = tokenLast = temp;
			}
			else{
				tokenLast->next = temp;
				tokenLast = temp;
			} 
		}

		//Get rid of the spaces. 
		else if(postfix->data == ' '){
			postfix = postfix->next;
		}
		
	}

	//Returns a linked list.
	return tokenList;
}

//Checks if a token is an operator (used in toPostfix function).
bool isOperator(char input){
	if (input == '^' || input == '*' || input == '/' || input == '+' || input == '-') return true;
	else return false;
}

//Adds a node to a linked list (used in toPostfix function).
node* addToLL(char input, node* head){
	node* toAdd = new node;
	toAdd->data = input;
	toAdd->next = NULL;

	if (!head){
		head = toAdd;
	}
	else{
		node* temp = head;
		while (temp->next){
			temp = temp->next;
		}
		temp->next = toAdd;
	}

	return head;
}

//Determines the precedence of operators (used in toPostfix function).
int precedence(char input){
	int p;

	if (input == '^') p = 3;
	if (input == '*' || input == '/') p = 2;
	if (input == '+' || input == '-') p = 1;

	return p;
}

//Prints the postfix linked list. 
void printPostfix(tNode* input){
	tNode* temp = input;
	while (temp){
		cout << temp->data << " ";
		temp = temp->next;
	}
	cout << endl; 
}

//Creates a binary tree from the postfix linked list. 
trNode* binaryTree(tNode* input){

	//Declare varuiable.
	stack<trNode*> stack;
	trNode *n, *n1, *n2;

	//While input isn't NULL. 
	while (input){

		//If number token --> create a node and push it to the stack. 
		if(isdigit(input->data[0])){
			n = newNode(input->data);
			stack.push(n);
		}

		//If operator token --> link it to its children, remove children from stack, and add familiy to stack. 
		else{
			n = newNode(input->data);
			n1 = stack.pop();
			n->right = n1;
			n2 = stack.pop();
			n->left = n2;
			stack.push(n);
		}
		input = input->next;
	}

	//Returns a linked list.
	return n;
}

//Creates a new node (used in binaryTree).
trNode* newNode(char* data){
	trNode* temp= new trNode;
	temp->left = temp->right = NULL;
	strcpy(temp->data, data);
	return temp;
}

//Transverses binary tree and prints expression in prefix form.
void printPrefix(trNode* input){
	if (input){
		cout << input->data << " ";
		printPrefix(input->left);
		printPrefix(input->right);
	}
}

//Transverses binary tree and prints expression in infix form.
void printInfix(trNode* input){ 
	if (input){
		printInfix(input->left);
		cout << input->data << " ";
		printInfix(input->right);
	}
}

//Transverses binary tree and prints expression in postfix form.
void printPostfixFromTree(trNode* input){
	if (input){
		printPostfixFromTree(input->left);
		printPostfixFromTree(input->right);
		cout << input->data << " ";
	}
}