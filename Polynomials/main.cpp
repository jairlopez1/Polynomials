//Polynomials
//Jair Lopez
//Denilson Morales

#include <iostream>
#include <string>
#include <sstream>
#include "Polynomial.h"


int main()
{
	std::string line;
	bool firstTime = true;

	Polynomial sum;
	initializer(sum);
	Polynomial max;
	initializer(max);
	Polynomial p;

	while (std::getline(std::cin, line))
	{
		std::istringstream strIn(line);

		if (!empty(strIn))
		{
			initializer(p);

			if (parsePolynomial(strIn, p))
			{
				if (firstTime)
				{
					for (Node* cur = p.Head; cur != nullptr; cur = cur->next)
					{
						insert(cur->term.coeff, cur->term.exp, sum);
						insert(cur->term.coeff, cur->term.exp, max);
					}

					firstTime = false;
				}
				else
				{
					addPolynomial(p, sum);
					findMax(p, max);
				}
			}
			else
			{
				std::cerr << "Error: " << line << std::endl;
			}

			destructor(p);
		}
	}

	print(sum); std::cout << std::endl;
	print(max); std::cout << std::endl;
}