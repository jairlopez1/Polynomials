#pragma once

struct Term
{
	double coeff;
	unsigned int exp;
};

struct Node
{
	Term term;
	Node* next;
	Node* prev;
};

class Polynomial
{
public:
	Node* Head;
};


//Functions
void insert(double coeff, unsigned int exp, Polynomial& p);
void print(Polynomial p);
void initializer(Polynomial& p);
void destructor(Polynomial& p);
bool empty(std::istream& in);
bool parseChar(std::istream& in, char required);
bool parseOp(std::istream& in, char& op);
bool parseCoefficient(std::istream& in, double& coeff);
bool parseExp(std::istream& in, unsigned int& exp);
bool parseTerm(std::istream& in, bool hasOp, Term& term);
bool parsePolynomial(std::istream& in, Polynomial& p);
void addPolynomial(Polynomial p, Polynomial& sum);
void findMax(Polynomial p, Polynomial& max);


