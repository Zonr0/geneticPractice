#ifndef POSTFIX_H
#define POSTFIX_H
#define NULL 0
#ifdef DEBUG
#include <iostream>
#endif
namespace PostfixNodes {
	struct node {
		int value;
		struct node * pNext;
		node();
		void deleteAll();
	};

	//TODO: Write comment explaining this
	const int SUBTRACTION = -1;
	const int ADDITION = -2;
	const int MULTIPLICATION = -3;
	const int DIVISION = -4;
	const int NOOP = -5;

}

using namespace PostfixNodes;
class Postfix {
	public:
		Postfix();
		~Postfix();

		void add(char);
		int evaluate();

#ifdef DEBUG
		void testClass();
#endif


	private:
		//Data structure operations
		//Stack operations
		void push(int value);
		int pop();
		inline int peek() {if (!pExpressionStack)
			 return NOOP; else return pExpressionStack->value;};


		inline int peekOutput();
		int dequeue();
		void enqueueOutput(int value);
		int performOperator(int lhs, int op, int rhs);

		inline int getPrecedence(char input);
		inline int getPrecedence(int input);
		inline int convertChar(char);
		node * pOutputQueue;
		node * pExpressionStack;

		const static int PREC_HIGHEST = 4;
		const static int PREC_HIGH = 3;
		const static int PREC_NORMAL = 2;
		const static int PREC_LOW = 1;
		const static int PREC_VERY_LOW = 0;
		const static int PREC_ERROR = -1;

};

#endif
