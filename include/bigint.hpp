#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

class bigint
{
    static constexpr int64_t m_iBase = 1ll * 1000 * 1000 * 1000; // * 1000 * 1000;              // Multiplication brokes with base > 1 000 000 000
    std::vector<int64_t> m_vDigits;
    bool m_bSign;

public:

    bigint();
    bigint(long long);
    bigint(const bigint &obj);

    void normalize(void);
    std::string to_string() const;
    void debug(void) const;

    bigint& operator= (const bigint&);

    const bigint operator+ (const bigint&) const;
    const bigint operator- (const bigint&) const;
    const bigint operator* (const bigint&) const;

    bool operator<  (const bigint&) const;
    bool operator>  (const bigint&) const;
    bool operator== (const bigint&) const;
    bool operator!= (const bigint&) const;
    bool operator>= (const bigint&) const;
    bool operator<= (const bigint&) const;

    bigint operator- () const;

    friend std::ostream& operator<<(std::ostream& os, const bigint& i);
};
