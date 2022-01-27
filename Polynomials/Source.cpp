#include "polynomial.hpp"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <cctype>
#include <sstream>
#include <string>
using namespace std;

/********************************************************************************************************************/
/******************************************POLY CONSTRUCTORS*********************************************************/
//Constructor with no values
Polynomial::Polynomial() {
	Head = nullptr;
}

//Copy Constructor
Polynomial::Polynomial(const Polynomial& other) :Head{ nullptr } {

	if (other.Head == nullptr) {
		this->Head = nullptr;
	}
	Node* cur;
	for (cur = other.Head; cur != nullptr; cur = cur->next)
	{
		addTerm(cur->term);
	}


}

//Assignment Operator
Polynomial& Polynomial::operator =(const Polynomial& other) {

	clear();
	Head = nullptr;
	Node* cur;
	for (cur = other.Head; cur != nullptr; cur = cur->next)
	{
		addTerm(cur->term);
	}

	return *this;
}

//Move Constructor
Polynomial::Polynomial(Polynomial&& other)noexcept {
	this->Head = other.Head;
	other.Head = nullptr;
}

//Move Assignment
Polynomial& Polynomial::operator=(Polynomial&& other) noexcept {

	if (this != &other) {
		clear();
		this->Head = other.Head;
		other.Head = nullptr;
	}
	return *this;
}

//Conversion Constructor Double to Poly
Polynomial::Polynomial(double d) {
	Term t = { (d) };
	this->addTerm(t);
}

//Conversion Constructor Term to Poly
Polynomial::Polynomial(Term t) {
	this->addTerm(t);
}

//Conversion Operator Poly to Term
Polynomial::operator Term() const {
	Term x;
	x.exponent = degree();
	x.coefficient = coefficientFor(x.exponent);
	return x;
}

//Conversion Operator Poly to Double
Polynomial::operator double() const
{
	double coeff = coefficientFor(degree());
	return coeff;
}

//Destructor
Polynomial::~Polynomial() {
	clear();
}

//Default constructor to Term
Term::Term()
{
	coefficient = 0;
	exponent = 0;
}

//Term Constructor
Term::Term(double coeff, int exp) {
	coefficient = coeff;
	exponent = exp;
}

/********************************************************************************************************************/
/***************************************COMPARE FUNCTIONS************************************************************/
//compare polynomial
Compare Polynomial::compare(Polynomial p)
{
	Polynomial a(*this);
	//finding the greatest degree from both Polynomials
	unsigned int exp = max(a.degree(), p.degree());

	//Now we going down in the Polynomial to find the respective coeffient at the greatest degree 
	//I want to stop when they are not equal
	while (exp > 0 && a[exp] == p[exp]) {
		//counting down in the Polynomial (move to the smaller polynomial)
		--exp;
	}
	//getting the values for less, greater and equal
	if (p[exp] < a[exp]) {
		//less
		return Compare::less;
	}
	if (p[exp] > a[exp]) {
		//greater
		return Compare::greater;
	}
	else {
		//equal
		return Compare::equal;
	}
}
//compare terms
Compare Polynomial::compareTerm(Term t)
{
	Term termToCompare = Term(*this);
	int exp = max(termToCompare.exponent, t.exponent);
	// t =-2x^4   2x^2    2x^5 -2x^2  x exp=2
	//getting the values for less, greater and equal
	while (exp > 0 && termToCompare.coefficient == t.coefficient) {
		//counting down in the Polynomial (move to the smaller polynomial)
		--exp;
	}
	if (t.coefficient < termToCompare.coefficient) {
		//less
		return Compare::less;
	}
	if (t.coefficient > termToCompare.coefficient) {
		//greater
		return Compare::greater;
	}
	else {
		//equal
		return Compare::equal;
	}

}
//compare doubles
Compare Polynomial::compareDoubles(double d)
{
	double doubleToCompare = double(*this);
	//getting the values for less, greater and equal
	if (d < doubleToCompare) {
		//less
		return Compare::less;
	}
	if (d > doubleToCompare) {
		//greater
		return Compare::greater;
	}
	else {
		//equal
		return Compare::equal;
	}
}

/********************************************************************************************************************/
/*****************************************POLY OPERATOR**************************************************************/
//Equality Operator
bool Polynomial::operator == (Polynomial p) {
	return p.compare(*this) == Compare::equal;
}

// Inequality Operator
bool Polynomial::operator !=(Polynomial p) {
	return p.compare(*this) != Compare::equal;
}

//Less Than Operator
bool Polynomial::operator < (Polynomial p) {
	return p.compare(*this) == Compare::less;
}

//Greater Than Operator
bool Polynomial:: operator > (Polynomial p) {
	return p.compare(*this) == Compare::greater;
}

//Less Than Or Equal Operator
bool Polynomial::operator <= (Polynomial p) {
	//Everything but greater
	return p.compare(*this) != Compare::greater;
}

//Greater Than or Equal Operator
bool Polynomial:: operator >= (Polynomial p) {
	//Everything except less (avoiding calling two functions and make the program inefficient
	return p.compare(*this) != Compare::less;
}

/********************************************************************************************************************/
/*****************************************TERM OPERATOR**************************************************************/
//Equality Operator
bool Polynomial::operator == (Term t) {
	return compareTerm(t) == Compare::equal;
}

// Inequality Operator
bool Polynomial::operator !=(Term t) {
	return compareTerm(t) != Compare::equal;
}

//Less Than Operator
bool Polynomial::operator < (Term t) {
	return compareTerm(t) == Compare::less;
}

//Greater Than Operator
bool Polynomial:: operator > (Term t) {
	return compareTerm(t) == Compare::greater;
}

//Less Than Or Equal Operator
bool Polynomial::operator <= (Term t) {
	//Everything but greater
	return compareTerm(t) != Compare::greater;
}

//Greater Than or Equal Operator
bool Polynomial:: operator >= (Term t) {
	//Everything except less (avoiding calling two functions and make the program inefficient
	return compareTerm(t) != Compare::less;
}

/*********************************************************************************************************************************/
/*****************************************DOUBLE OPERATOR*************************************************************************/
//Equality Operator
bool Polynomial::operator == (double d) {
	return compareDoubles(d) == Compare::equal;
}

//Inequality Operators
bool Polynomial::operator !=(double d) {
	return compareDoubles(d) != Compare::equal;

}

//Less Than Operator
bool Polynomial::operator < (double d) {
	return compareDoubles(d) == Compare::less;
}

//Less Than Or Equal Operator
bool Polynomial::operator <= (double d) {
	return compareDoubles(d) == Compare::greater;
}

//Greater Than Operator
bool Polynomial::operator > (double d) {
	return compareDoubles(d) != Compare::greater;
}

//Greater Than or Equal Operator
bool Polynomial::operator >= (double d) {
	return compareDoubles(d) != Compare::less;
}


/********************************************************************************************************************/
/**********************************************OPERATORS EXTRA*******************************************************/
//Negative Operator
Polynomial Polynomial::operator - () {
	Polynomial copy(*this);
	this->clear();
	Node* cur;
	for (cur = copy.Head; cur != nullptr; cur = cur->next)
	{
		cur->term.coefficient *= -1;
		addTerm(cur->term);
	}
	return *this;
}

//Getting Value of Polynomial
double Polynomial::operator() (int x) {
	Polynomial copy(*this);
	unsigned int exp = this->degree();
	double total = 0;
	while (exp > 0) {
		total += copy[exp] * (pow(x, exp));
		--exp;
	}
	return total;
}

//Getting the Coefficient 
double Polynomial::operator[](unsigned int exp)
{
	//first we need to loop and find where in the poly has the exponent we are passing
	Node* cur;

	for (cur = this->Head; cur != nullptr; cur = cur->next)
	{
		if (cur->term.exponent == exp) {
			return cur->term.coefficient;

		}
	}
	return 0;
}
/********************************************************************************************************************/
/**********************************************MAIN FUNCTIONS********************************************************/
//Getting Degree Function
unsigned int Polynomial::degree() const {
	if (Head != nullptr) {
		return this->Head->term.exponent;
	}
	else {
		return 0;
	}
}
//Adding a term to a polynomial
bool Polynomial::addTerm(Term t) {
	//Allocating memory
	Node* newP = new Node;

	//Failed to allocate memory 
	if (newP == NULL) {
		return false;
	}

	//Populate the Polynomial by setting the value of data into the term (node). 
	newP->term.coefficient = t.coefficient;
	newP->term.exponent = t.exponent;
	newP->next = NULL;
	newP->prev = NULL;

	//The new pointer (nemP) will point to what head is pointing to 
	if (this->Head == NULL) {
		this->Head = newP;
		return true;
	}

	//Inserting the terms in the Polynomial. First loop throughout the polynomial and finding the exact location to insert
	else
	{
		Node* cur;
		for (cur = this->Head; cur != NULL; cur = cur->next)
		{

			/***********************************************First Case Insert*************************************************************/
			   //Inserts Term (node) at the beginning of the list
			if (cur->prev == NULL && newP->term.exponent >= cur->term.exponent)
			{
				//First Case exponents are same and b is next
				if (cur->term.exponent == newP->term.exponent) {
					double a, b;
					a = cur->term.coefficient;
					b = newP->term.coefficient;
					if ((a + b) == 0) {
						Delete(cur->term, *this);
						delete newP;
						return true;
					}
					else {
						cur->term.coefficient = cur->term.coefficient + newP->term.coefficient;
						delete newP;
						return true;
					}

				}

				//
				if (newP->term.coefficient == cur->term.coefficient) {
					//Case for when the greatest coefficient 
					if (newP->term.coefficient >= cur->term.coefficient) {
						cur->prev = newP;
						newP->next = cur;
						this->Head = newP;
						return true;
					}

					if (newP->term.coefficient <= cur->term.coefficient) {
						cur->next = newP;
						newP->prev = cur;
						return true;
					}
				}
				else {
					cur->prev = newP;
					newP->next = cur;
					this->Head = newP;
					return true;
				}
			}
			/***********************************************Second Case Insert************************************************************/
			   //Inserts Node at the End of the List
			if (cur->next == NULL && newP->term.exponent <= cur->term.exponent)
			{
				//Second Case exponents are same and b is next
				if (cur->term.exponent == newP->term.exponent) {
					double a, b;
					a = cur->term.coefficient;
					b = newP->term.coefficient;
					if ((a + b) == 0) {
						Delete(cur->term, *this);
						delete newP;
						return true;
					}
					else {
						cur->term.coefficient = cur->term.coefficient + newP->term.coefficient;
						delete newP;
						return true;
					}

				}
				if (newP->term.coefficient == cur->term.coefficient) {
					//If the coefficient is lesser
					if (newP->term.coefficient <= cur->term.coefficient) {
						cur->next = newP;
						newP->prev = cur;
						return true;
					}
					//if the coefficient is greater
					if (newP->term.coefficient >= cur->term.coefficient) {
						cur->prev = newP;
						newP->next = cur;
						this->Head = newP;
						return true;
					}
				}
				else {
					cur->next = newP;
					newP->prev = cur;
					return true;
				}
			}
			//middle case
			if (cur->term.exponent <= newP->term.exponent)
			{
				//First Case exponents are same and b is next
				if (cur->term.exponent == newP->term.exponent) {
					double a, b;
					a = cur->term.coefficient;
					b = newP->term.coefficient;
					if ((a + b) == 0) {
						Delete(cur->term, *this);
						delete newP;
						return true;
					}
					else {
						cur->term.coefficient = cur->term.coefficient + newP->term.coefficient;
						delete newP;
						return true;
					}

				}
				newP->next = cur;
				newP->prev = cur->prev;
				cur->prev = newP;
				newP->prev->next = newP;
				return true;

			}


		}
		return true;

	}
}

//Printing Operator
void polyPrint(Polynomial const& p, std::ostream& out) {
	bool zero = false;
	for (Node* ptr = p.Head; ptr != NULL; ptr = ptr->next)
	{

		unsigned int exponent = ptr->term.exponent;
		double coefficient = ptr->term.coefficient;
		/*	if (exponent == this->term.exponent && coefficient == >term.coefficient)*/
			//if the term we are ouputing is the first term of the Polynomial and it is positive
			//we dont output the operator +, but we output -
		if (exponent == p.degree()) {
			//if exponent is 0 you just output the coefficient
			if (exponent == 0) {
				if (coefficient == 0) {
					out << 0 << " ";

				}
				else if (coefficient > 0) {
					out << coefficient << " ";

				}
				else {
					out << "- " << coefficient * -1 << " ";

				}
				zero = true;
			}
			//if exponent is  1 you output-> (+,-) + coefficient + 'x^' + exponent
			if (exponent == 1) {
				if (coefficient == 1) {
					out << "x" << " ";
				}
				else if (coefficient > 0) {
					out << coefficient << "x" << " ";
				}

				else {
					out << "-" << coefficient * -1 << "x" << " ";
				}
				zero = true;
			}
			//if exponent is greater than 1 you output-> (+,-) + coefficient + 'x^' + exponent
			if (exponent > 1) {
				if (coefficient == 1) {
					out << "x^" << exponent << " ";
				}
				else if (coefficient > 0) {
					out << " " << coefficient << "x^" << exponent << " ";

				}
				else if (coefficient == 0) {
					zero = true;
				}
				else {
					out << "-" << " " << coefficient * -1 << "x^" << exponent << " ";
				}
				zero = true;
			}

		}
		else {
			//if exponent is 0 you just output the coefficient
			if (exponent == 0) {
				if (coefficient == 1) {
					out << coefficient << " ";
				}
				else if (coefficient > 0) {
					out << "+ " << coefficient << " ";

				}

				else {
					out << "- " << coefficient * -1 << " ";

				}
				zero = true;
			}
			//if exponent is  1 you output-> (+,-) + coefficient + 'x^' + exponent
			if (exponent == 1) {
				if (coefficient == 1) {
					out << "+" << " " << "x" << " ";

				}
				else if (coefficient > 0) {
					out << "+" << " " << coefficient << "x" << " ";
				}
				else {
					out << "-" << " " << coefficient * -1 << "x" << " ";
				}
				zero = true;

			}
			//if exponent is greater than 1 you output-> (+,-) + coefficient + 'x^' + exponent
			if (exponent > 1) {
				if (coefficient == 1) {
					out << "+" << " " << "x^" << exponent << " ";

				}
				else if (coefficient > 0) {
					out << "+" << " " << coefficient << "x^" << exponent << " ";
				}
				else {
					out << "-" << " " << coefficient * -1 << "x^" << exponent << " ";
				}
				zero = true;
			}
		}

	}
	if (!zero) {
		out << "0";
	}
	std::cout << endl;
}

//Clear Polynomial
void Polynomial::clear() {
	Node* Cur = Head;
	while (Cur != NULL)
	{
		Head = Head->next;
		delete Cur;
		Cur = Head;
	}
}
//delete one term function
bool Delete(Term  t, Polynomial& q)
{
	Node* cur;
	for (cur = q.Head; cur != NULL; cur = cur->next)
	{
		//Deletes Node at the beginning
		if (cur->term.exponent == t.exponent && cur->term.coefficient == t.coefficient && cur->prev == NULL)
		{
			//If there is just one item on the list
			if (cur->next != NULL)
			{
				cur = q.Head->next;
				delete(q.Head);
				q.Head = cur;
				q.Head->prev = NULL;
			}
			else {
				delete cur;
				q.Head = NULL;
			}

			return true;

		}
		//Deletes Node at the middle of the list
		else if (cur->term.exponent == t.exponent && cur->term.coefficient == t.coefficient && cur->prev != NULL && cur->next != NULL)
		{
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;

			delete cur;

			return true;

		}
		//Deletes node at the end of the list
		else if (cur->term.exponent == t.exponent && cur->term.coefficient == t.coefficient && cur->next == NULL)
		{
			cur->prev->next = NULL;

			delete cur;
			return true;
		}

	}
	return false;
}

//Reading Polynomial
void Polynomial::polyRead(Polynomial& polynomial, std::istream& in) {
	Term term;
	if (!scanTerm(in, false, term)) {
		// Error if we don't read at least one term
		in.setstate(std::ios::failbit);
	}
	else {
		polynomial.clear();
		polynomial.addTerm(term);

		while (scanTerm(in, true, term)) {
			polynomial.addTerm(term);
		}
	}
}

/********************************************************************************************************************/
/**********************************************OPERATORS STREAM******************************************************/
//Printing Operator
std::ostream& operator<<(std::ostream& out, const Polynomial& p)
{
	polyPrint(p, out);
	return out;
}

//Input Operator (reads in a string) 
std::istream& operator>>(std::istream& in, Polynomial& p)
{
	p.polyRead(p, in);
	return in;
}

/********************************************************************************************************************/
/*********************************************PROFESSOR CODE*********************************************************/

////////////////////////////////////////////////////////////////////////////////
// You shouldn't need to change anything below here ***** GLOBAL FUNCTIONS *****
////////////////////////////////////////////////////////////////////////////////

//Reading whitespace
void shipWhitespace(std::istream& in) {
	int c = in.peek();
	while (std::isspace(c)) {
		in.get();
		c = in.peek();
	}
	in.clear();
}

// Try to get specified character from the stream
bool scanChar(std::istream& in, char required) {
	shipWhitespace(in);
	int c = in.peek();
	if (c == required) {
		in.get();
		return true;
	}
	else {
		in.clear();
		return false;
	}
}

// Try to get an operator (+/-) from the stream
bool scanOperator(std::istream& in, char& op) {
	if (scanChar(in, '-')) {
		op = '-';
		return true;
	}
	else if (scanChar(in, '+')) {
		op = '+';
		return true;
	}
	else {
		return false;
	}
}

//Reading dots
bool scanNumString(std::istream& in, std::string& numstr, bool allowDot) {
	shipWhitespace(in);
	numstr.clear();
	int c = in.peek();
	while (std::isdigit(c) || allowDot && c == '.') {
		numstr += c;
		if (c == '.') {
			allowDot = false;
		}
		in.get();
		c = in.peek();
	}
	if (numstr == ".") {
		in.setstate(std::ios::failbit);
		return false;
	}
	in.clear();
	return true;
}

// Try to get the coefficient from the stream
bool scanCoefficient(std::istream& in, double& coefficient, bool& implicit) {
	bool neg = scanChar(in, '-');
	bool dot = false;

	std::string numstr;
	if (!scanNumString(in, numstr, true)) return false;

	if (numstr.size() > 0) {
		implicit = false;
		coefficient = std::stod(numstr);
	}
	else {
		implicit = true;
		coefficient = 1;
	}
	if (neg) {
		coefficient = -coefficient;
	}
	return true;
}

// Try to get variable + exponent from the stream
bool scanExp(std::istream& in, int& exponent, bool required) {
	if (scanChar(in, 'x')) {
		if (scanChar(in, '^')) {
			// If there's a caret, there must be an exponent
			std::string numstr;
			if (!scanNumString(in, numstr, true)) return false;

			if (numstr.size() > 0) {
				exponent = std::stoi(numstr);
			}
			else {
				in.setstate(std::ios::failbit);
				return false;
			}
		}
		else {
			// No caret
			exponent = 1;
		}
	}
	else if (required) {
		in.setstate(std::ios::failbit);
		return false;
	}
	else {
		// No variable
		exponent = 0;
	}
	return true;
}

//Try to get a term from the stream
//@param hasOp = is there an operator (+/-) before this term?
bool scanTerm(std::istream& in, bool hasOp, Term& term) {
	bool implicitCoefficient;
	char op = '+';
	if (hasOp) {
		if (!scanOperator(in, op)) return false;
	}

	if (!scanCoefficient(in, term.coefficient, implicitCoefficient)) return false;
	if (op == '-') {
		term.coefficient = -term.coefficient;
	}

	if (!scanExp(in, term.exponent, implicitCoefficient)) return false;

	return true;
}


/*********************************************PROFESSOR CODE***********************************************************/

/*********************************************GLOBAL OPERATORS ***********************************************************/

// Sum Operator
Polynomial operator + (Polynomial _this, Polynomial p)
{
	int exp;
	Polynomial sum;
	if (_this.degree() != NULL) {
		exp = max(_this.degree(), p.degree());
	}
	else {
		exp = p.degree();
	}

	while (exp >= 0) {
		Term x;
		x.exponent = exp;
		x.coefficient = _this.coefficientFor(exp) + p.coefficientFor(exp);

		if (x.coefficient != 0) {
			sum.addTerm(x);
		}

		--exp;

	}

	return sum;
}

//Substraction Operator
Polynomial operator - (Polynomial _this, Polynomial p)
{
	//3x 2x
	Polynomial sum;
	int exp = max(_this.degree(), p.degree());

	while (exp >= 0) {
		Term x;
		x.exponent = exp;
		x.coefficient = _this.coefficientFor(exp) - p.coefficientFor(exp);

		if (x.coefficient != 0) {
			sum.addTerm(x);
		}
		--exp;
	}
	return sum;
}

// + = Operator
Polynomial operator += (Polynomial _this, Polynomial p)
{


	Polynomial temp;
	temp = _this + p;
	_this = std::move(temp);
	return _this;
}

// - = Operator
Polynomial operator -= (Polynomial _this, Polynomial p)
{
	Polynomial temp;
	temp = _this - p;
	_this = std::move(temp);
	return _this;
}
