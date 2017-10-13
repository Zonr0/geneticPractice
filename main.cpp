#include "Chromosome.h"
#include "Postfix.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
	GeneEncode testEncode;
	cout << "Testing GeneEncode" << endl;
	cout << testEncode.decode("0101") << endl;
	cout << testEncode.decode("1111") << endl;

	cout << "Testing Operator" << endl;

	
	Chromosome testChromosome;

#ifdef DEBUG
	cout << "Testing random chromosome decode" << endl;
	cout << testChromosome.testOperator(1, '/', 8) << endl;
#endif
	cout << "Initial expression:" << endl;
	testChromosome.parseResult();
	testChromosome.displayParsed();

	cout << "Reduced expression:" << endl;
	testChromosome.reduceExpression();
	testChromosome.displayParsed();

#ifdef DEBUG

	cout << "Printing Chromosome expression size:" << endl;
	testChromosome.printExpressionSize();

#endif

	cout << "Attempting to evaluate this expression:" << endl;
	cout << testChromosome.evaluateExpression() << endl;;

	cout << "*****************************************************" << endl;
	cout << "		Combination testing		      " << endl;
	cout << "*****************************************************" << endl;

	cout << "Creating a second Chromosome:" << endl;
	Chromosome testChromosome2;
	testChromosome2.parseResult();
	testChromosome2.displayParsed();

	cout << "Reduced:" << endl;
	testChromosome2.reduceExpression();
	testChromosome2.displayParsed();

	testChromosome.combine(testChromosome2);
	cout << "Chromosome 1 after combining with Chromosome 2" << endl;
	testChromosome.parseResult();
	testChromosome.displayParsed();
	testChromosome.reduceExpression();
	testChromosome.displayParsed();

	cout << testChromosome.evaluateExpression() << endl;

	cout << "*****************************************************" << endl;
	cout << "		Genepool testing		      " << endl;
	cout << "*****************************************************" << endl;


	Genepool testPool;
	testPool.displayAll();
	//Testing Postfix
	Postfix testPostfix;

#ifdef DEBUG
//	testPostfix.testClass();
#endif
	




}
