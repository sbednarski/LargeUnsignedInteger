
#ifndef LARGEUNSIGNEDINTEGER_H_
#define LARGEUNSIGNEDINTEGER_H_


#include <iostream>
#include <string>
#include <utility>


#define ULL_MAX 0xFFFF'FFFF'FFFF'FFFFull

#define PRINT_DEBUG(obj) obj.print_debug(#obj)

#define MIN(a, b) (a < b) ? (a) : (b)
#define MAX(a, b) (a > b) ? (a) : (b)

class LargeUnsignedInteger;

using ull_t = unsigned long long;
using quot_rem = std::pair<LargeUnsignedInteger, LargeUnsignedInteger>;



class LargeUnsignedInteger {
private:
	unsigned int num_segments;
	ull_t* arr;	// little-endian
	unsigned int* arr_half;

	bool is_separating_char(char c);
	void construct_string_bin(std::string::const_iterator& itr, std::string::const_iterator& itr_end);
	void construct_string_oct(std::string::const_iterator& itr, std::string::const_iterator& itr_end);
	void construct_string_dec(std::string::const_iterator& itr, std::string::const_iterator& itr_end);
	void construct_string_hex(std::string::const_iterator& itr, std::string::const_iterator& itr_end);

	void assign_arr(ull_t* arr_new);
	void refresh_arr_half();

	void resize(unsigned int len);
	void trim();

	static const ull_t NUM_ONE_TENTH_INIT;
	static const ull_t NUM_ONE_TENTH;
	static const ull_t DEN_POW_ONE_TENTH;
	unsigned int div_mod_ten(const LargeUnsignedInteger& num, const ull_t& den_pow);


public:
	static const unsigned int UINT_BITS;
	static const unsigned int ULL_BITS;

	LargeUnsignedInteger();
	LargeUnsignedInteger(ull_t num);
	LargeUnsignedInteger(unsigned int len, const ull_t* nums);
	LargeUnsignedInteger(const std::string& str);
	LargeUnsignedInteger(const LargeUnsignedInteger& rhs);	// copy constructor
	LargeUnsignedInteger(LargeUnsignedInteger&& rhs);		// move constructor
	~LargeUnsignedInteger();								// destructor

	unsigned int get_size() const;

	bool is_zero() const;

	void reset();
	void set(ull_t num);
	void set(unsigned int len, const ull_t* nums);
	void set(const std::string& str);

	ull_t get_low_word() const;

	LargeUnsignedInteger operator+(const LargeUnsignedInteger& rhs) const;
	LargeUnsignedInteger operator+(const ull_t& rhs) const;

	LargeUnsignedInteger operator-(const LargeUnsignedInteger& rhs) const;
	LargeUnsignedInteger operator-(const ull_t& rhs) const;

	LargeUnsignedInteger operator*(const LargeUnsignedInteger& rhs) const;
	LargeUnsignedInteger operator*(const ull_t& rhs) const;

	quot_rem div_mod(const LargeUnsignedInteger& rhs) const;
	quot_rem div_mod(const ull_t& rhs) const;

	LargeUnsignedInteger operator/(const LargeUnsignedInteger& rhs) const;
	LargeUnsignedInteger operator/(const ull_t& rhs) const;

	LargeUnsignedInteger operator%(const LargeUnsignedInteger& rhs) const;
	LargeUnsignedInteger operator%(const ull_t& rhs) const;

	LargeUnsignedInteger operator<<(const ull_t& rhs) const;
	LargeUnsignedInteger operator>>(const ull_t& rhs) const;

	bool operator<(const LargeUnsignedInteger& rhs) const;
	bool operator<(const ull_t& rhs) const;

	bool operator>(const LargeUnsignedInteger& rhs) const;
	bool operator>(const ull_t& rhs) const;

	bool operator<=(const LargeUnsignedInteger& rhs) const;
	bool operator<=(const ull_t& rhs) const;

	bool operator>=(const LargeUnsignedInteger& rhs) const;
	bool operator>=(const ull_t& rhs) const;

	bool operator==(const LargeUnsignedInteger& rhs) const;
	bool operator==(const ull_t& rhs) const;

	bool operator!=(const LargeUnsignedInteger& rhs) const;
	bool operator!=(const ull_t& rhs) const;

	LargeUnsignedInteger& operator+=(const LargeUnsignedInteger& rhs);
	LargeUnsignedInteger& operator+=(const ull_t& rhs);

	LargeUnsignedInteger& operator-=(const LargeUnsignedInteger& rhs);
	LargeUnsignedInteger& operator-=(const ull_t& rhs);

	LargeUnsignedInteger& operator*=(const LargeUnsignedInteger& rhs);
	LargeUnsignedInteger& operator*=(const ull_t& rhs);

	LargeUnsignedInteger& operator/=(const LargeUnsignedInteger& rhs);
	LargeUnsignedInteger& operator/=(const ull_t& rhs);

	LargeUnsignedInteger& operator%=(const LargeUnsignedInteger& rhs);
	LargeUnsignedInteger& operator%=(const ull_t& rhs);

	LargeUnsignedInteger& operator<<=(const ull_t& rhs);
	LargeUnsignedInteger& operator>>=(const ull_t& rhs);

	LargeUnsignedInteger& operator++();	// prefix increment
	LargeUnsignedInteger& operator--();	// prefix decrement

	LargeUnsignedInteger operator++(int);	// postfix increment
	LargeUnsignedInteger operator--(int);	// postfix decrement

	LargeUnsignedInteger& operator=(const LargeUnsignedInteger& rhs);	// copy assignment
	LargeUnsignedInteger& operator=(const ull_t& rhs);					// copy assignment scalar

	LargeUnsignedInteger& operator=(LargeUnsignedInteger&& rhs);		// move assignment
	LargeUnsignedInteger& operator=(ull_t&& rhs);						// move assignment scalar

	friend std::ostream& operator<<(std::ostream& os, const LargeUnsignedInteger& rhs);
	void print_debug(const std::string name) const;
};


#endif /* LARGEUNSIGNEDINTEGER_H_ */
