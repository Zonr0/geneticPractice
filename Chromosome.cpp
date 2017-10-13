#include "Chromosome.h"

/******************************************************************************
 *	GeneEncode
 *****************************************************************************/

GeneEncode::GeneEncode() {
	hashMap["0000"] = '0';
	hashMap["0001"] = '1';
	hashMap["0010"] = '2';
	hashMap["0011"] = '3';
	hashMap["0100"] = '4';
	hashMap["0101"] = '5';
	hashMap["0110"] = '6';
	hashMap["0111"] = '7';
	hashMap["1000"] = '8';
	hashMap["1001"] = '9';
	hashMap["1010"] = '+';
	hashMap["1011"] = '-';
	hashMap["1100"] = '*';
	hashMap["1101"] = '/';
}

char GeneEncode::decode(const char * szInputString) {
	char returnChar = 'f'; //Arbitrary non-dataset to check for init. //Arbitrary non-dataset to check for init.
	try {
		returnChar = hashMap.at(szInputString);
	} catch(std::out_of_range) {
		returnChar = '\0';
	}
	assert(returnChar != 'f');
	return returnChar;
}

/******************************************************************************
 *	Chromosome	
 *****************************************************************************/

std::default_random_engine Chromosome::rGen(time(NULL));
std::uniform_int_distribution<int> Chromosome::rDist(
		Chromosome::MIN_CHROMOSOME_SIZE,Chromosome::MAX_CHROMOSOME_SIZE);
std::uniform_real_distribution<double> Chromosome::rDistChance(0.0,1.0);

Chromosome::Chromosome() : 
	szParsedString(NULL),
	fitness(0),
	geneSize(4),
	expressionValue(-999)
	//rGen((time(NULL)),
	//rDist(16,64),
	//rDistChance(0.0,1.0)
{
	//TODO: Set the target number dynamically
	targetNumber = 64;
	chromosomeSize = rDist(rGen);	
	chromosomeSize -= (chromosomeSize % geneSize);

	szParsedString = new char[chromosomeSize / geneSize + 1];
	szBitString = new char[chromosomeSize + 1];
	int i = 0;
	for (i = 0; i < chromosomeSize; ++i) {
		szBitString[i] = '0' + (rDist(rGen) % 2);
	}
	szBitString[i] = '\0';
}

Chromosome::~Chromosome() {
	if(szParsedString) delete[] szParsedString;
	if(szBitString) delete[] szBitString;
	szParsedString = NULL;
	szBitString = NULL;
}

void Chromosome::parseResult() {
	//Parses the raw bit string into a raw string translation
	GeneEncode encoder;
	assert(!(strlen(szBitString) % geneSize));
	char * szGeneString = new char[geneSize + 1];
	int i;
	for (i = 0; i < chromosomeSize / 4; ++i) {
		strncpy(szGeneString,szBitString + i * geneSize,geneSize);
		*(szParsedString + i) = encoder.decode(szGeneString);
	}	
	*(szParsedString + i) = '\0';
	delete[] szGeneString;
}

void Chromosome::reduceExpression() {
	int initialExpSize = chromosomeSize / geneSize;
	char * szFinalExpression = new char[initialExpSize];
	bool bLastCharType = 1; //0 for digit, 1 for operator
	int j = 0; //Position of szFinalExpression
	int i = 0;
	char currentChar;
	for (i = 0; i < initialExpSize; ++i) {
		currentChar = szParsedString[i];
		if (currentChar != '\0' && ((currentChar >= '0' && currentChar <= '9' && bLastCharType == 1)
				|| (currentChar < '0' && bLastCharType == 0 && i < initialExpSize - 1)))
	       	{
			bLastCharType = !bLastCharType;
			//TODO: Find out why the expression size is erratic.

			assert(currentChar != '\0');
			calculator.add(currentChar);
			szFinalExpression[j++] = currentChar;
		}	
		assert(currentChar <= '9');
		//assert(currentChar >= '*');
	}
	//TODO: Add an idempotent or remove the last operator if it ends in an operator.
	szFinalExpression[j] = '\0';
	expressionSize = j;
	strcpy(szParsedString, szFinalExpression);
	delete[] szFinalExpression;
	//displayParsed();
}

int Chromosome::evaluateExpression() {

	if (expressionValue == -999) expressionValue = calculator.evaluate();
	return expressionValue;

	/*
	int operandLeft, operandRight;
	char expOperator = '+';
	operandLeft = operandRight = 0;
	for (int i = 0; i < expressionSize; ++i) {
		if (!(i % 2)) //operand
		{
			//compute stored op, store in left
			//First iteration with default values is 0 + 0
			//operandLeft = operandRight;
			operandRight = szParsedString[i] - '0';
			operandLeft = useOperator(operandLeft, expOperator, operandRight);
		}
		else
			expOperator=szParsedString[i];
	}

	return operandLeft;
	*/
}

int Chromosome::useOperator(int left, char op, int right) {
	switch(op) {
		case('+'):
			return left + right;
			break;
		case('-'):
			return left - right;
			break;
		case('*'):
			return left * right;
			break;
		case('/'):
			if (!right) return 0;
			return left / right;
			break;
		default:
			assert(true);
			return 0;
	}
}

/*
char r_reduceExpression(char * szExpressionString, int i, int length) {
	if(i >= length) return '\0';
	else return 'l';
	
	if (isOperator(szExpressionString[i])) {
		//recurse left
		//save char to string
		//recurse right
		//save char to string
	}
}
*/

Chromosome & Chromosome::combine(Chromosome & other) {
	crossover(other);
	mutate();
	other.mutate();
	expressionValue = -999;
	return *this;
}

Chromosome & Chromosome::crossover(Chromosome & other) {
	double randChance = rDistChance(rGen);		
	int randIndex =
	       	std::uniform_int_distribution<int>(0,geneSize-1)(rGen);
	char tempBit;
	assert(geneSize == other.geneSize);
	if (randChance <= CROSSOVER_RATE) {
		for (int i = randIndex; i < geneSize && i < other.geneSize; ++i) {
			tempBit = szBitString[i];
			szBitString[i] = other.szBitString[i];
			other.szBitString[i] = tempBit;
		}
	}
	return *this;
}

Chromosome & Chromosome::mutate() {
	double randChance = 0.0;
	for (int i = 0; i < geneSize; ++i) {
		randChance = rDistChance(rGen);	
		if (randChance <= MUTATION_RATE)
			szBitString[i] = !szBitString[i];
	}
	return *this;
}

double Chromosome::evaluateFitness() {
	int targetDifference = targetNumber - evaluateExpression();
	if (targetDifference < 0) targetDifference *= -1;
	//TODO: Better handle the case where we are an exact match
	else if (targetDifference == 0) { fitness = 999.0; return fitness;}
	else fitness = 1/double(targetDifference);
			
	return fitness;

}

void Chromosome::displayEquation() {
	using namespace std;
	cout << "Reduced equation: ";
	displayParsed();
	cout << "Expression Value: " << evaluateExpression() << endl;
	cout << "Fitness Value: " << fitness << endl << endl;
}

/*******************************************************************************
 * Genepool
 ******************************************************************************/

Genepool::Genepool() {
	poolSize = POOL_SIZE;
	poolArray = new Chromosome[poolSize];
	updatePool();
}


Genepool::~Genepool() {
	delete[] poolArray;
	poolArray = NULL;
}

void Genepool::updatePool() {
	assert(poolArray != NULL);
	if (!poolArray) return;
	
	for (int i = 0; i < poolSize; ++i) {
		poolArray[i].parseResult();
		poolArray[i].reduceExpression();
		poolArray[i].evaluateFitness();
	}
}

void Genepool::displayAll() {
	for (int i = 0; i < poolSize; ++i) {
		poolArray[i].displayEquation();
		std::cout << std::endl;
	}
}

void Genepool::incrementGeneration() {
	Chromosome * pPickOne, pPickTwo;
	Chromosome ** newPool = new Chromosome *[poolSize];
	for (int i = 0; i < poolSize; ++i) {
		selectTwo(pPickOne, pPickTwo);
		//poolArray[i].combine(

	}
}
