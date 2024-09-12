#include "../include/bigint.hpp"

#define PLUS 0
#define MINUS 1

void bigint::normalize()
{
	for (size_t i = 0; i < this->m_vDigits.size(); i++)
	{
		if (this->m_vDigits[i] < this->m_iBase)
			continue;

		int64_t carry = this->m_vDigits[i] / this->m_iBase;
		this->m_vDigits[i] -= carry * this->m_iBase;

		if (i + 1 >= this->m_vDigits.size())
			this->m_vDigits.push_back(0);

		this->m_vDigits[i + 1] += carry;
	}

}

bigint::bigint()
{
	this->m_bSign = PLUS;
	this->m_vDigits = std::vector<int64_t>();
	this->m_vDigits.push_back(0);
}

bigint::bigint(long long n)
{
	this->m_bSign = PLUS;
	if (n <= 0)
	{
		this->m_bSign = MINUS;
		n = -n;
	}

	this->m_vDigits.push_back(n);
	normalize();
}

bigint::bigint(const bigint& obj)
{
	this->m_bSign = obj.m_bSign;
	this->m_vDigits = obj.m_vDigits;
}

std::string bigint::to_string() const
{
	std::string res = "";
	if (this->m_bSign == MINUS) res = "-";
	for (auto it = this->m_vDigits.rbegin(); it != this->m_vDigits.rend(); it++)
	{
		if (*it != 0)
		{
			std::string tmp = std::to_string(*it);

			if (it != this->m_vDigits.rbegin())
				while (tmp.size() != 9) tmp = "0" + tmp;

			res += tmp;
		}
		else
		{
			res += "000000000";
		}
	}

	return res;
}

void bigint::debug() const
{
	std::cout << (*this).to_string() << std::endl;
	std::cout << "Is negative? " << this->m_bSign << std::endl;
	std::cout << "vector( " << this->m_vDigits.size() << "): ";
	for (size_t i = 0; i < this->m_vDigits.size(); i++) std::cout << this->m_vDigits[i] << " ";
	std::cout << std::endl;
}

bigint& bigint::operator= (const bigint& obj)
{
	this->m_bSign = obj.m_bSign;
	this->m_vDigits = obj.m_vDigits;

	return *this;
}


const bigint bigint::operator+ (const bigint& other) const
{
	if (this->m_bSign == MINUS && other.m_bSign == PLUS)
		return other - -*this;
	else if (this->m_bSign == PLUS && other.m_bSign == MINUS)
		return *this - -other;

	bigint res = bigint();
	uint64_t carry = 0;

	for (size_t i = 0; (i < this->m_vDigits.size() || i < other.m_vDigits.size()) || carry; i++)
	{
		if (res.m_vDigits.size() == i)
			res.m_vDigits.push_back(0);

		uint64_t val = 0;

		if (i < other.m_vDigits.size())
			val = this->m_vDigits[i] + other.m_vDigits[i] + carry;
		else if (i < this->m_vDigits.size())
			val = this->m_vDigits[i] + carry;
		else
			val = carry;

		if (val == this->m_iBase)
			carry = 1;
		else
			carry = val / this->m_iBase;

		if (carry)
			val = val % this->m_iBase;

		res.m_vDigits[i] = val;

	}

	if (this->m_bSign == MINUS)
		res.m_bSign = MINUS;

	return res;
}
const bigint bigint::operator- (const bigint& other) const
{
	if (this->m_bSign == PLUS && other.m_bSign == MINUS)
		return *this + -other;
	else if (this->m_bSign == MINUS && other.m_bSign == MINUS)
		return -other - -*this;
	else if (this->m_bSign == MINUS && other.m_bSign == PLUS)
		return -(-*this + other);

	const bigint& a = *this >= other ? *this : other;
	const bigint& b = *this < other ? *this : other;

	bigint res = bigint(a);
	bool carry = 0;


	for (size_t i = 0; (i < b.m_vDigits.size()) || carry; i++)
	{
		if (i == res.m_vDigits.size())
			res.m_vDigits.push_back(0);

		if (i < b.m_vDigits.size())
			res.m_vDigits[i] -= b.m_vDigits[i] - carry;
		else
			res.m_vDigits[i] -= carry;

		carry = 0;

		if (res.m_vDigits[i] < 0)
		{
			carry = 1;
			res.m_vDigits[i] = m_iBase + res.m_vDigits[i];
		}
	}

	if (a != (*this))
		res.m_bSign = MINUS;

	return res;
}
const bigint bigint::operator* (const bigint& other) const
{
	bigint res = bigint();
	uint64_t carry = 0;

	const bigint& a = this->m_bSign == MINUS ? -*this : *this;
	const bigint& b = other.m_bSign == MINUS ? -other : other;

	res.m_vDigits = std::vector<int64_t>(a.m_vDigits.size() * b.m_vDigits.size() + 1, 0);

	for (size_t i = 0; i < a.m_vDigits.size(); i++)
	{
		for (size_t j = 0; j < b.m_vDigits.size() || carry; j++)
		{
			uint64_t var = j < b.m_vDigits.size() ?
				res.m_vDigits[i + j] + a.m_vDigits[i] * b.m_vDigits[j] + carry :
				res.m_vDigits[i + j] + carry;

			res.m_vDigits[i + j] = var % m_iBase;
			carry = var / m_iBase;
		}
	}
	while (res.m_vDigits.back() == 0)
		res.m_vDigits.pop_back();

	return res;
}

bigint bigint::operator- () const
{
	bigint res = bigint(*this);
	res.m_bSign = !res.m_bSign;
	return res;
}

bool bigint::operator<  (const bigint& other) const
{
	if (this->m_vDigits.size() != other.m_vDigits.size()) return this->m_vDigits.size() < other.m_vDigits.size();

	auto& a = this->m_vDigits.rbegin();
	auto& b = other.m_vDigits.rbegin();

	for (a, b; a != this->m_vDigits.rend(); a++, b++)
	{
		if (*a == *b) continue;
		return *a < *b;
	}

	return false;
}
bool bigint::operator>  (const bigint& other) const
{
	if (this->m_vDigits.size() != other.m_vDigits.size()) return this->m_vDigits.size() > other.m_vDigits.size();

	auto& a = this->m_vDigits.rbegin();
	auto& b = other.m_vDigits.rbegin();

	for (a, b; a != this->m_vDigits.rend(); a++, b++)
	{
		if (*a == *b) continue;
		return *a > *b;
	}

	return false;
}
bool bigint::operator== (const bigint& other) const
{
	if (this->m_vDigits.size() != other.m_vDigits.size()) false;

	auto& a = this->m_vDigits.rbegin();
	auto& b = other.m_vDigits.rbegin();

	for (a, b; a != this->m_vDigits.rend(); a++, b++)
		if (*a != *b) return false;

	return true;
}
bool bigint::operator!= (const bigint& other) const
{
	return !(*this == other);
}
bool bigint::operator>= (const bigint& other) const
{
	return (*this > other) || (*this == other);
}
bool bigint::operator<= (const bigint& other) const
{
	return (*this < other) || (*this == other);
}

std::ostream& operator<<(std::ostream& os, const bigint& i)
{
	os << i.to_string();
	return os;
}

#undef PLUS
#undef MINUS
