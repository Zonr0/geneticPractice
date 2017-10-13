#include "Postfix.h"
node::node() : value(NOOP), pNext(0) {}

void node::deleteAll() {
	if (pNext) {
		pNext->deleteAll();
		delete pNext;
		pNext = NULL;
	}
}

Postfix::Postfix() : pOutputQueue(0), pExpressionStack(0) {}

Postfix::~Postfix() {
	node * pFront;
	//Clear the expression stack
	if (pExpressionStack) {
		pExpressionStack->deleteAll();
		delete pExpressionStack;
		pExpressionStack = NULL;
	}
	//Clear the output queue
	if (pOutputQueue) {
		pFront = pOutputQueue->pNext;
		pOutputQueue->pNext = NULL;
		pFront->deleteAll();
		delete pFront;
		pFront = NULL;
	}
}

void Postfix::push(int value) {
	node * pOldHead;
	node * pNewNode = new node;
	pNewNode->value = value;

	//Insert at head of list
	pOldHead = pExpressionStack;
	pExpressionStack = pNewNode;
	pExpressionStack->pNext = pOldHead;
}

int Postfix::pop() {
	int ret;
	node * pCurHead;
	if(!pExpressionStack) {
		return NOOP;
	}
	ret = pExpressionStack->value;

	pCurHead = pExpressionStack;
	pExpressionStack = pExpressionStack->pNext;
	delete pCurHead;

	return ret;
}

//Peek defined inline

//Queue functions

int Postfix::dequeue() {
	int ret;
	node * pFront;
	if (!pOutputQueue) return NOOP;

	pFront =  pOutputQueue->pNext;
	ret = pFront->value;
	//Link rear to second item in CLL.
	pOutputQueue->pNext = pFront->pNext;
	if (pFront == pOutputQueue) { //If only one item
		delete pFront;
		pOutputQueue = NULL;
	}
	else delete pFront;	
	return ret;
}

void Postfix::enqueueOutput(int value) {
	node * pNewNode;
	pNewNode = new node;
	pNewNode->value = value;

	if (!pOutputQueue) {
		pOutputQueue = pNewNode;
		pOutputQueue->pNext = pOutputQueue;
		return;
	}
	pNewNode->pNext = pOutputQueue->pNext;
	pOutputQueue->pNext = pNewNode;
	pOutputQueue = pNewNode;
	return;
}

int Postfix::performOperator(int lhs, int op, int rhs) {
	if(op == SUBTRACTION) return lhs - rhs;
	else if (op == ADDITION) return lhs + rhs;
	else if (op == MULTIPLICATION) return lhs * rhs;
	else if (op == DIVISION && rhs != 0) return lhs / rhs;
	else return 0;
}

inline int Postfix::peekOutput() {
	if (!pOutputQueue) return NOOP;
	return pOutputQueue->pNext->value;
}

//UNTESTED
void Postfix::add(char input) {
	int precedence;
	int topOfStack;
	if (input < '0') {
		precedence = getPrecedence(input);	
		//If incoming operator has a lower precedence than the top of
		//the stack, pop off the stack into the queue unil not.
		topOfStack = peek();
		while (topOfStack != NOOP && precedence <= getPrecedence(topOfStack)) {
			enqueueOutput(pop());
			topOfStack = peek();
		}
		push(convertChar(input));
	}
	else {
		enqueueOutput(convertChar(input));
	}
}

//UNTESTED
int Postfix::evaluate() {
	int intermediary = 0;
	int lhs = 0;
	int rhs = 1;
	bool bStackOneItem= false;
	while (peek() != NOOP) {
		enqueueOutput(pop());
	}
	//push values onto the stack until an operator is encountered
	while(peekOutput() != NOOP || !bStackOneItem) { //While output is not empty
		while(peekOutput() >= 0 && peekOutput() != NOOP) { //While queue front is a value
			push(dequeue());
		}

		//pop last two values and pass as arguments to the operator.
		rhs = pop();
		lhs = pop();
		intermediary = performOperator(lhs, dequeue(), rhs);
		if (peek() == NOOP) bStackOneItem = true;
		else bStackOneItem = false;
		//Push the result onto the stack
		push(intermediary);
	} //TODO: Stop when queue is empty and stack only has 1 item at the end.

	return pop();
}

//Untested

inline int Postfix::getPrecedence(char input) {
	return getPrecedence(convertChar(input));
}

inline int Postfix::getPrecedence(int input) {
	//Simple hardcoded calculation. No need for fancy abstraction yet.
	if (input == MULTIPLICATION || input == DIVISION) return PREC_HIGH;
	else if (input == ADDITION || input == SUBTRACTION) return PREC_NORMAL;
	else {
		return PREC_ERROR;
	}
}

inline int Postfix::convertChar(char rawInput) {
	if (rawInput >= '0') return rawInput - '0';
	switch(rawInput) {
		case('+'): return ADDITION;
		case('-'): return SUBTRACTION;
		case('*'): return MULTIPLICATION;
		case('/'): return DIVISION;
		default: return NOOP;
	}
}

//DEBUG FUNCTIONS

#ifdef DEBUG
void Postfix::testClass() {
	using namespace std;
	const int TEST_SIZE = 5;
	cout << "Attempting to push " << TEST_SIZE << " items to the stack" << endl;
	for(int i =0; i < TEST_SIZE; ++i) {
		push(i);
	}
	cout << "Attempting to peek and pop entire stack" << endl;
	for(int i =0; i < TEST_SIZE; ++i) {
		cout << peek() << "(" << pop() << ") ";
	}
	cout << endl << "Attempting to enqueue items" << endl;
	for(int i =0; i < TEST_SIZE; ++i) {
		enqueueOutput(i);
	}
	cout << "Attempting to peek and dequeue items" << endl;
	for(int i =0; i < TEST_SIZE; ++i) {
		cout << peekOutput() << "(" << dequeue() << ") ";
	}
	cout << endl;

	cout << "Testing the convertChar functions" << endl;
	cout << "KEY:\n" << ADDITION << " +" << endl;
	cout << SUBTRACTION << " -" << endl;
	cout << MULTIPLICATION << " *" << endl;
	cout << DIVISION << " /" << endl;
	cout << NOOP << " NULL OP" << endl;

	cout << "+ " << convertChar('+') << " - " << convertChar('-') << " * "
		<< convertChar('*') << " / " << convertChar('/') << endl;

	cout << "Digits:" << endl;
	cout << convertChar('0') << convertChar('1') << convertChar('2') <<
		convertChar('3') << convertChar('4') << convertChar('5') <<
		convertChar('6') << convertChar('7') << convertChar('8') <<
		convertChar('9') << endl;

	cout << "Testing expressions:" << endl;
	add('5'); add('+'); add('7');
	cout << "5 + 7 = " << evaluate() << endl;

	add('5'); add('+'); add('7');
	cout << "5 + 7 = " << evaluate() << endl;

	add('3'); add('+'); add('3'); add('/'); add('3');
	cout << "3 + 3 / 3 = " << evaluate() << endl;


}

#endif
