#pragma once
#include <ostream>
#pragma once

/*******************************************************
Operators Questions: P to P, P to T, P - D, T to P, D to P
Do we need to have a += , -= overload for T to P, D to P?
How many operators total?
Try to delete extra space at beginning of printing
Big number crashes the program
Exam Review 2. 1 Minute 7:30 Watch what he says about comparison methods.
*******************************************************/

struct Term {
	double coefficient;
	int exponent;
	Term();
	//Term Constructor
	Term(double coeff, int exp = 0);

};

struct Node {
	Term term;
	Node* next = nullptr;
	Node* prev = nullptr;
};

enum class Compare {
	less, equal, greater
};

class Polynomial {

public:
	/*****************************************************************************************************************************/
	/************************************************CONSTRUCTORS*****************************************************************/
	//Default constructor — initalizes to the polynomial 0
	Polynomial();
	////Copy constructor / copy assignment
	Polynomial(const Polynomial& other);
	// Assignment operator
	Polynomial& operator =(const Polynomial& other);
	////Move constructor
	Polynomial(Polynomial&& other)noexcept;
	//Move assignment
	Polynomial& operator=(Polynomial&& other) noexcept;
	//Destructor
	~Polynomial();
	//Conversion Constructor - Double to Poly - Implicit
	Polynomial(double d);
	//Polynomial( double d,unsigned int exp);
	//Conversion Constructor - Term to Poly - Implicit
	Polynomial(Term t);
	//Conversion Operator - Poly to Double - Explicit
	explicit operator double() const;
	//Conversion Operator - Poly to Term - Explicit 
	explicit operator Term() const;
	/*****************************************************************************************************************************/
	/***********************************************COMPARE FUNCTIONS*************************************************************/
	//Compare function is the main function to find the less, greater and equal polynomial
	Compare compare(Polynomial p);
	//Compare Term
	Compare compareTerm(Term t);
	//Compare Double
	Compare compareDoubles(double d);
	//clear everything
	void clear();
	/*****************************************************************************************************************************/
	/**************************************POLY TO POLY OPERATORS COMPARISION*************************************************************/
	//Equality Operator
	bool operator == (Polynomial p);
	//Inequality Operators
	bool operator !=(Polynomial p);
	//Less Than Operator
	bool operator < (Polynomial p);
	//Less Than Or Equal Operator
	bool operator <= (Polynomial p);
	//Greater Than Operator
	bool operator > (Polynomial p);
	//Greater Than or Equal Operator
	bool operator >= (Polynomial p);

	/******************************************************************************************************************************/
	/**************************************POLY TO TERM OPERATORS COMPARISON*************************************************************/
	bool operator == (Term t);
	//Inequality Operators
	bool operator !=(Term t);
	//Less Than Operator
	bool operator < (Term t);
	//Less Than Or Equal Operator
	bool operator <= (Term t);
	//Greater Than Operator
	bool operator > (Term t);
	//Greater Than or Equal Operator
	bool operator >= (Term t);
	/*****************************************************************************************************************************/
	/****************************************POLY TO DOUBLE OPERATORS COMPARISON*********************************************************/
	//Equality Operator
	bool operator == (double d);
	//Inequality Operators
	bool operator !=(double d);
	//Less Than Operator
	bool operator < (double d);
	//Less Than Or Equal Operator
	bool operator <= (double d);
	//Greater Than Operator
	bool operator > (double d);
	//Greater Than or Equal Operator
	bool operator >= (double d);

	/*****************************************************************************************************************************/
	/*********************************************OPERATORS EXTRA*****************************************************************/
	//Convert to Negative Operator
	Polynomial operator - ();
	//[] Operator to Return Coefficion given Degree
	double operator [](unsigned int exponent);
	//() To get Value of Polynomial given X
	double operator ()(int x);
	/*****************************************************************************************************************************/
	/*********************************************MAIN FUNCTIONS******************************************************************/
	//Adding a term
	bool addTerm(Term t);
	//Getting the degree of a Polynomial
	unsigned int degree() const;
	//Reading the Polynomial
	void polyRead(Polynomial& self, std::istream& in);
	//Print Function
	friend void polyPrint(Polynomial const& self, std::ostream& out);
	//Coefficient For 
	double coefficientFor(unsigned int exp) const {
		//first we need to loop and find where in the poly has the exponent we are passing
		Node* cur;

		for (cur = this->Head; cur != nullptr; cur = cur->next)
		{
			if (cur->term.exponent == exp) {
				return cur->term.coefficient;

			}
		}
		return 0;

		//Once we find it we should return it or return 0 if we do not find it.
	}
	friend bool Delete(Term t, Polynomial& p);
	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/
private:
	Node* Head;
};
/*********************************************************************************************************************************/
/**********************************************GLOBAL FUNCTIONS*******************************************************************/
//Print Operator
std::ostream& operator << (std::ostream& out, const Polynomial& p);
//Input Operator
std::istream& operator >> (std::istream& in, Polynomial& p); //by reference but not const because we have to change it
//Reading White Space
void shipWhitespace(std::istream& in);
//Getting specific character from the stream
bool scanChar(std::istream& in, char required);
//Getting an operator (+/-) from the stream
bool scanOperator(std::istream& in, char& op);
//Reading dots (.)
bool scanNumString(std::istream& in, std::string& numstr, bool allowDot);
//Getting the coefficient from the stream
bool scanCoefficient(std::istream& in, double& coefficient, bool& implicit);
//Try to get variable + exponent from the stream
bool scanExp(std::istream& in, int& exponent, bool required);
//Try to get a term from the stream
bool scanTerm(std::istream& in, bool hasOp, Term& term);

//MATH FUNCTIONS 
//sum Operator
Polynomial operator + (Polynomial _this, Polynomial p);
//Substraction operator
Polynomial operator - (Polynomial _this, Polynomial p);

// + = Operator
Polynomial operator += (Polynomial _this, Polynomial p);
// - = Operator
Polynomial operator -= (Polynomial _this, Polynomial p);

/*********************************************************************************************************************************/
/****************************************TERM TO POLY OPERATORS COMPARISON*********************************************************/
	//Equality Operator
inline bool operator == (Term x, Polynomial p) {
	return p.compareTerm(x) == Compare::equal;
}
//Inequality Operators
inline bool operator !=(Term x, Polynomial p) {
	return p.compareTerm(x) != Compare::equal;
}
//Less Than Operator
inline bool operator < (Term x, Polynomial p) {
	return p.compareTerm(x) == Compare::less;
}
//Less Than Or Equal Operator
inline bool operator <= (Term x, Polynomial p) {
	//Everything but greater
	return p.compareTerm(x) != Compare::greater;
}
//Greater Than Operator
inline bool operator > (Term x, Polynomial p) {
	return p.compareTerm(x) == Compare::greater;
}
//Greater Than or Equal Operator
inline bool operator >= (Term x, Polynomial p) {
	//Everything but greater
	return p.compareTerm(x) != Compare::less;
}
/****************************************DOUBLE TO POLY OPERATORS COMPARISON*********************************************************/
	//Equality Operator
inline bool operator == (double d, Polynomial p) {
	return p.compareDoubles(d) == Compare::equal;
}
//Inequality Operators
inline bool operator !=(double d, Polynomial p) {
	return p.compareDoubles(d) != Compare::equal;
}
//Less Than Operator
inline bool operator < (double d, Polynomial p) {
	return p.compareDoubles(d) == Compare::less;
}
//Less Than Or Equal Operator
inline bool operator <= (double d, Polynomial p) {
	//Everything but greater
	return p.compareDoubles(d) != Compare::greater;
}
//Greater Than Operator
inline bool operator > (double d, Polynomial p) {
	return p.compareDoubles(d) == Compare::greater;
}
//Greater Than or Equal Operator
inline bool operator >= (double d, Polynomial p) {
	//Everything but greater
	return p.compareDoubles(d) != Compare::less;
}
/*********************************************************************************************************************************/
struct Operations
{
	Polynomial sum;
	Polynomial substr;
};

