#include <iostream>
#include "include/bigint.hpp"

int main()
{
	long long val = 100000000000000;
	bigint first = bigint(val);
	bigint second = first;

	for (int i = 0; i < 16; i++) first = first + first;
	second = first;
	first = second + second + second;

	std::cout << first << std::endl << second << std::endl << first * second << std::endl << std::endl;

	return 0;
}
