#include <iostream>
#include <string>
#include <sstream>
#include "Polynomial.h"

//Inserts the inputted line into a polynomial and divide them by terms
void insert(double coeff, unsigned int exp, Polynomial& p)
{
	Node* newNode = new Node;
	newNode->prev = nullptr;
	newNode->term.coeff = coeff;
	newNode->term.exp = exp;
	newNode->next = nullptr;

	if (p.Head == nullptr)
	{
		p.Head = newNode;
	}
	else
	{
		for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
		{
			//At the front
			if (newNode->term.exp >= cur->term.exp && cur->prev == nullptr)
			{
				newNode->next = cur;
				cur->prev = newNode;
				newNode->prev = nullptr;
				p.Head = newNode;
				break;
			}
			//At the end
			else if (newNode->term.exp <= cur->term.exp && cur->next == nullptr)
			{
				cur->next = newNode;
				newNode->prev = cur;
				newNode->next = nullptr;
				break;
			}
			//In the middle
			else if (newNode->term.exp <= cur->term.exp && newNode->term.exp >= cur->next->term.exp)
			{
				newNode->next = cur->next;
				cur->next->prev = newNode;
				cur->next = newNode;
				newNode->prev = cur;
				break;
			}
		}
	}
}

//Prints the final result of the polynomial addition
void print(Polynomial p)
{
	bool firstTime = true;

	if (p.Head != nullptr)
	{
		for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
		{
			//signs
			if (firstTime && cur->term.coeff != 0)
			{
				if (cur->term.coeff < 0)
				{
					cur->term.coeff *= -1;
					std::cout << "-";
				}
				firstTime = false;
			}
			else if (!firstTime && cur->term.coeff != 0)
			{
				if (cur->term.coeff > 0)
					std::cout << "+ ";
				if (cur->term.coeff < 0)
					std::cout << "- ";
			}
			// coeff 0 and exp 0
			if (cur->term.coeff == 0 && cur->term.exp == 0)
			{
				continue;
			}
			else if (cur->term.coeff == 0)
			{
				continue;
			}
			// integer ^ 0
			else if (cur->term.coeff != 0 && cur->term.exp == 0)
			{
				if (cur->term.coeff < 0)
					cur->term.coeff *= -1;

				std::cout << cur->term.coeff << " ";
			}
			// coeff 1 and exp no 0 or 1
			else if (cur->term.coeff == 1 || cur->term.coeff == -1 && cur->term.exp > 1)
			{
				if (cur->term.coeff < 0)
					cur->term.coeff *= -1;

				std::cout << "x^" << cur->term.exp << " ";
			}
			//
			else if (cur->term.coeff == 1 || cur->term.coeff == -1 && cur->term.exp == 1)
			{
				if (cur->term.coeff < 0)
					cur->term.coeff *= -1;

				std::cout << "x ";
			}
			//exp 1
			else if (cur->term.exp == 1)
			{
				std::cout << cur->term.coeff << "x ";
			}
			// normal term
			else
			{
				if (cur->term.coeff < 0)
					cur->term.coeff *= -1;
				std::cout << cur->term.coeff << "x^" << cur->term.exp << " ";
			}
		}
	}
	else
	{
		std::cout << "0" << std::endl;
	}
}

//Initializes or creates the polynomial
void initializer(Polynomial& p)
{
	p.Head = nullptr;
}

//Destroys or clears up a polynomial when no needed anymore
void destructor(Polynomial& p)
{
	Node* current = p.Head;
	Node* next;

	while (current != nullptr)
	{
		next = current->next;
		delete current;
		current = next;
	}
}

// Test whether there are any (non-whitespace) characters left in the stream
bool empty(std::istream& in)
{
	char c;
	if (in >> c)
	{
		in.putback(c);
		return false;
	}
	else
	{
		return true;
	}
}

// Try to get specified character from the stream
bool parseChar(std::istream& in, char required)
{
	char c;
	if (in >> c)
	{
		if (c == required)
			return true;
		in.putback(c);
	}
	return false;
}

// Try to get an operator (+/-) from the stream
bool parseOp(std::istream& in, char& op)
{
	if (parseChar(in, '-'))
	{
		op = '-';
		return true;
	}
	else if (parseChar(in, '+'))
	{
		op = '+';
		return true;
	}
	else
	{
		return false;
	}
}

// Try to get the coefficient from the stream
bool parseCoefficient(std::istream& in, double& coeff)
{
	bool neg = parseChar(in, '-');
	if (!(in >> coeff))
	{
		// Coefficient is optional
		coeff = 1.0;
		in.clear();
	}
	if (neg)
	{
		coeff = -coeff;
	}

	return true;
}

// Try to get variable + exponent from the stream
bool parseExp(std::istream& in, unsigned int& exp)
{
	if (parseChar(in, 'x'))
	{
		if (parseChar(in, '^'))
		{
			// If there's a caret, there must be an exponent
			if (!(in >> exp))
				return false;
		}
		else
		{
			// No caret
			exp = 1;
		}
	}
	else
	{
		// No variable
		exp = 0;
	}
	return true;
}

// Try to get a term from the stream
// @param hasOp = is there an operator (+/-) before this term?
bool parseTerm(std::istream& in, bool hasOp, Term& term)
{
	char op = '+';
	if (hasOp)
	{
		if (!parseOp(in, op)) return false;
	}

	if (!parseCoefficient(in, term.coeff))
		return false;

	if (op == '-')
	{
		term.coeff = -term.coeff;
	}

	if (!parseExp(in, term.exp))
		return false;

	return true;
}

// Try to get a polynomial from the stream
bool parsePolynomial(std::istream& in, Polynomial& p)
{
	bool hasOp = false;

	do
	{
		Term t;

		if (!parseTerm(in, hasOp, t))
			return false;
		insert(t.coeff, t.exp, p);
		hasOp = true;
	} while (!empty(in));

	return true;
}

// Add and stored the sum of the inserted polynomial
void addPolynomial(Polynomial p, Polynomial& sum)
{
	bool wasItFound;

	for (Node* cur1 = p.Head; cur1 != nullptr; cur1 = cur1->next)
	{
		wasItFound = false;

		for (Node* cur2 = sum.Head; cur2 != nullptr; cur2 = cur2->next)
		{
			if (cur1->term.exp == cur2->term.exp)
			{
				cur2->term.coeff = cur2->term.coeff + cur1->term.coeff;
				wasItFound = true;
			}
		}

		if (!wasItFound)
		{
			insert(cur1->term.coeff, cur1->term.exp, sum);
		}
	}
}

//Try to finds the polynomial with max exponential value
void findMax(Polynomial p, Polynomial& max)
{
	bool wasFound = false;

	Node* curP = p.Head;
	Node* curMax = max.Head;
	Node* headP = p.Head;
	Node* headMax = max.Head;

	while (wasFound == false)
	{
		if (headP->term.exp > headMax->term.exp)
		{
			destructor(max);
			initializer(max);

			for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
			{
				insert(cur->term.coeff, cur->term.exp, max);
			}
		}
		else if (headP->term.exp == headMax->term.exp)
		{
			if (curP->term.coeff > curMax->term.coeff)
			{
				if (curP->term.coeff > 0)
				{
					destructor(max);
					initializer(max);

					for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
					{
						insert(cur->term.coeff, cur->term.exp, max);
					}

					wasFound = true;
				}
				else
				{
					break;
				}
			}
			else if (curP->term.coeff < curMax->term.coeff)
			{
				if (curMax->term.coeff < 0)
				{
					destructor(max);
					initializer(max);

					for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
					{
						insert(cur->term.coeff, cur->term.exp, max);
					}

					wasFound = true;
				}
				else
				{
					break;
				}
			}
			else if (curP->term.exp == curMax->term.exp)
			{
				if (curP->term.coeff > curMax->term.coeff)
				{
					destructor(max);
					initializer(max);

					for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
					{
						insert(cur->term.coeff, cur->term.exp, max);
					}

					wasFound = true;
				}
			}

			curP = curP->next;
			curMax = curMax->next;
		}
		else
			break;
	}
}
