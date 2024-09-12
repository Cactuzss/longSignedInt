#include <iostream>
#include "include/bigint.hpp"

int main()
{
	long long val = 100000000000000;
	bigint first = bigint(val);
	bigint second = first;

	return 0;
}
