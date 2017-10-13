#include "Postfix.h"
#include <unordered_map>
#include <cassert>
#include <random>
#include <cstring>
#include <iostream>
#include <ctime>

class Genepool {
	public:
		Genepool();
		//TODO: Implement Geneool copy constructor
		Genepool(const Genepool&);
		//TODO: Implement genepool assignment operator
		Genepool & operator=(const Genepool&);

		~Genepool();

		//Displays the best fitting equation so far
		void displayBest();
		//Displays all of the reduced equations in the pool
		void displayAll();
		//TODO: Implement incrementGeneration
		void incrementGeneration();
		void selectTwo(Chromosome *& pPickOne, Chromosome *& pPickTwo);

		protected:

		//Re-parses and re-reduces all elements chromosomes in pool
		void updatePool();
		int poolSize;

		class Chromosome * poolArray;
		static constexpr int POOL_SIZE = 32;

};

class Chromosome {
	public:
		Chromosome();
		//TODO: Implement Copy constructor
		Chromosome(const Chromosome &);
		//TODO: Implement assignment operator
		Chromosome & operator=(const Chromosome &);
		//TODO: Implement Chromosome destructor
		~Chromosome();
		void parseResult();
		void reduceExpression();
		double evaluateFitness();
		inline void displayParsed() { std::cout << szParsedString << std::endl; }
		void displayEquation();
		int evaluateExpression();
		Chromosome & combine(Chromosome & );

#ifdef DEBUG
		inline int testOperator(int left, char op, int right) { return useOperator(left, op, right); }
		inline void printExpressionSize() { std::cout << expressionSize << std::endl; return;}


#endif
		

	protected:
		//TODO: Replace char * with actual byte string
		char * szParsedString;
		char * szBitString;
		double fitness;
		int geneSize;
		int chromosomeSize;
		int expressionSize;
		int targetNumber;
		int expressionValue;

		static constexpr double MUTATION_RATE = 0.01;
		static constexpr double CROSSOVER_RATE = 0.7;
		static constexpr int MIN_CHROMOSOME_SIZE = 32;
		static constexpr int MAX_CHROMOSOME_SIZE = 64;

		Postfix calculator;

		static std::default_random_engine rGen;
		static std::uniform_int_distribution<int> rDist;
		static std::uniform_real_distribution<double> rDistChance;


		Chromosome & mutate();
		Chromosome & crossover(Chromosome &);
	

	private:
		int useOperator(int left, char op, int right);
		//char r_reduceExpression(char * expressionString, int i, int length);
};

class GeneEncode {
	public:
		GeneEncode();
		char decode(const char *);

	protected:
		std::unordered_map<std::string, char> hashMap;	
};
