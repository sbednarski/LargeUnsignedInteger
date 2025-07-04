
#include "LargeUnsignedInteger.h"
#include <exception>
#include <utility>
#include <ostream>
#include <string>
#include <iomanip>


// Static constants
const ull_t LargeUnsignedInteger::NUM_ONE_TENTH_INIT = 0xCCCC'CCCC'CCCC'CCCCull;
const ull_t LargeUnsignedInteger::NUM_ONE_TENTH = 0xCCCC'CCCDull;
//const ull_t LargeUnsignedInteger::NUM_ONE_TENTH = 3'435'973'837ull;
const ull_t LargeUnsignedInteger::DEN_POW_ONE_TENTH = 35ull;
const unsigned int LargeUnsignedInteger::UINT_BITS = sizeof(unsigned int) * 8;
const unsigned int LargeUnsignedInteger::ULL_BITS = sizeof(ull_t) * 8;


// Default Constructor
LargeUnsignedInteger::LargeUnsignedInteger() :
		num_segments	{1},
		arr				{new ull_t[1]}
{
	refresh_arr_half();

	arr[0] = 0;
}


// Scalar Initializing Constructor
LargeUnsignedInteger::LargeUnsignedInteger(ull_t num) :
		num_segments	{1},
		arr				{new ull_t[1]}
{
	refresh_arr_half();

	arr[0] = num;
}


// Array Initializing Constructor
LargeUnsignedInteger::LargeUnsignedInteger(unsigned int len, const ull_t* num) :
		num_segments	{len},
		arr				{new ull_t[len]}
{
	refresh_arr_half();

	for(;  len > 0;  --len)
		arr[len-1] = num[len-1];

	trim();
}


// String Initializing Constructor
LargeUnsignedInteger::LargeUnsignedInteger(const std::string& str) :
		LargeUnsignedInteger()	// call default constructor
{
	// Parse string
	set(str);
}


// Copy Constructor
LargeUnsignedInteger::LargeUnsignedInteger(const LargeUnsignedInteger& rhs) :
		num_segments	{rhs.num_segments},
		arr				{new ull_t[rhs.num_segments]}
{
	refresh_arr_half();

	// Copy rhs array to this array
	for(unsigned int i = 0;  i < num_segments;  ++i)
		this->arr[i] = rhs.arr[i];
}


// Move Constructor
LargeUnsignedInteger::LargeUnsignedInteger(LargeUnsignedInteger&& rhs) :
		num_segments	{rhs.num_segments},
		arr				{rhs.arr}
{
	refresh_arr_half();

	// Nullify rhs
	rhs.num_segments = 0;
	rhs.arr = nullptr;
	rhs.refresh_arr_half();
}


// Destructor
LargeUnsignedInteger::~LargeUnsignedInteger() {
	delete[] arr;
}


// Return array size
unsigned int LargeUnsignedInteger::get_size() const {
	return num_segments;
}


// Check if value is zero
bool LargeUnsignedInteger::is_zero() const {
	return num_segments == 1 && arr[0] == 0;
}


// Set array to zero
void LargeUnsignedInteger::reset() {
	// Resize array
	if(num_segments > 1) {
		assign_arr(new ull_t[1]);
		num_segments = 1;
	}

	// Set value to zero
	arr[0] = 0;
}


// Set array to value
void LargeUnsignedInteger::set(ull_t num) {
	// Resize array
	if(num_segments > 1) {
		assign_arr(new ull_t[1]);
		num_segments = 1;
	}

	// Set value
	arr[0] = num;
}


// Set array to array value
void LargeUnsignedInteger::set(unsigned int len, const ull_t* nums) {
	// Resize array
	if(num_segments != len) {
		assign_arr(new ull_t[len]);
		num_segments = len;
	}

	// Set values
	for(;  len > 0;  --len)
		arr[len-1] = nums[len-1];

	// Trim object
	trim();
}


// Set array to string value
void LargeUnsignedInteger::set(const std::string& str) {
	// Reset array
	reset();

	// Throw error for empty string
	if(str.empty())
		throw std::invalid_argument("String argument is empty.");

	// Special case for input 0
	if(str.length() == 1  &&  str[0] == '0')
		return;

	std::string::const_iterator itr = str.cbegin();
	std::string::const_iterator itr_end = str.cend();

	// Check for integer literal prefix
	if(*itr == '0') {
		// Number is hexadecimal
		if(*(itr+1) == 'X' || *(itr+1) == 'x') {
			itr += 2;
			construct_string_hex(itr, itr_end);
		}

		// Number is binary
		else if(*(itr+1) == 'B' || *(itr+1) == 'b') {
			// Increment past prefix
			itr += 2;
			construct_string_bin(itr, itr_end);
		}

		// Number is octal
		else {
			// Increment past prefix
			++itr;
			construct_string_oct(itr, itr_end);
		}
	}

	// Number is decimal
	else {
		construct_string_dec(itr, itr_end);
	}

	trim();
}


// Return low word from arr
ull_t LargeUnsignedInteger::get_low_word() const {
	return arr[0];
}


// Return the sum of two LargeUnsignedInteger objects as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator+(const LargeUnsignedInteger& rhs) const {
	unsigned int min_segments;
	const LargeUnsignedInteger* max_op;

	// Determine which operand is smaller / larger
	if(this->num_segments < rhs.num_segments) {
		min_segments = this->num_segments;
		max_op = &rhs;
	} else {
		min_segments = rhs.num_segments;
		max_op = this;
	}

	// Initialize return object with larger number of segments
	LargeUnsignedInteger rtn{};
	rtn.resize(max_op->num_segments);

	// Half adder sum. Needed to check for carry out
	ull_t half_sum;

	// Adder carry bit
	ull_t carry = 0;

	unsigned int i = 0;

	// Full Adder
	for(;  i < min_segments;  ++i) {
		half_sum = this->arr[i] + rhs.arr[i];
		rtn.arr[i] = half_sum + carry;
		carry = (this->arr[i] > ULL_MAX - rhs.arr[i])  ||  (half_sum > ULL_MAX - carry);
	}

	// Half Adder to propagate carry
	for(;  i < max_op->num_segments;  ++i) {
		rtn.arr[i] = max_op->arr[i] + carry;
		carry = max_op->arr[i] > ULL_MAX - carry;
	}

	// Add additional word for carry out
	if(carry) {
		rtn.resize(rtn.num_segments+1);
		rtn.arr[rtn.num_segments-1] = 1;
	}

	return rtn;
}


// Return the sum of a LargeUnsignedInteger object with a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator+(const ull_t& rhs) const {
	// Initialize return object
	LargeUnsignedInteger rtn{};
	rtn.resize(this->num_segments);

	// Adder carry bit
	ull_t carry;

	// Add rhs
	rtn.arr[0] = this->arr[0] + rhs;
	carry = this->arr[0] > ULL_MAX - rhs;

	// Half Adder to propagate carry
	for(unsigned int i = 1;  i < this->num_segments;  ++i) {
		rtn.arr[i] = this->arr[i] + carry;
		carry = this->arr[i] > ULL_MAX - carry;
	}

	// Add additional word for carry out
	if(carry) {
		rtn.resize(rtn.num_segments+1);
		rtn.arr[rtn.num_segments-1] = 1;
	}

	return rtn;
}


// Return the difference of two LargeUnsignedInteger objects as a new object
// Behavior is undefined if LHS < RHS
LargeUnsignedInteger LargeUnsignedInteger::operator-(const LargeUnsignedInteger& rhs) const {
	unsigned int min_segments;
	const LargeUnsignedInteger* max_op;

	// Determine which operand is smaller / larger
	if(this->num_segments < rhs.num_segments) {
		min_segments = this->num_segments;
		max_op = &rhs;
	} else {
		min_segments = rhs.num_segments;
		max_op = this;
	}

	// Initialize return object with larger number of segments
	LargeUnsignedInteger rtn{};
	rtn.resize(max_op->num_segments);

	// Half subtracter difference. Needed to check for borrow in
	ull_t half_diff;

	// Subtracter borrow bit
	ull_t borrow = 0;

	unsigned int i = 0;

	// Full Subtracter
	for(;  i < min_segments;  ++i) {
		half_diff = this->arr[i] - rhs.arr[i];
		rtn.arr[i] = half_diff - borrow;
		borrow = (this->arr[i] < rhs.arr[i]) || (half_diff < borrow);
	}

	// This is larger array. Half Subtracter to propagate borrow
	if(this == max_op) {
		for(;  i < max_op->num_segments;  ++i) {
			rtn.arr[i] = this->arr[i] - borrow;
			borrow = this->arr[i] < borrow;
		}
	}
	// This is smaller array. Full Subtracter for remaining RHS segments
	else {
		for(;  i < max_op->num_segments;  ++i) {
			half_diff = -rhs.arr[i];
			rtn.arr[i] = half_diff - borrow;
			borrow = (0 < rhs.arr[i]) || (half_diff < borrow);
		}
	}

	// Trim return object
	rtn.trim();

	return rtn;
}

// Return the difference of a LargeUnsignedInteger object with a ull as a new object
// Behavior is undefined if LHS < RHS
LargeUnsignedInteger LargeUnsignedInteger::operator-(const ull_t& rhs) const {
	// Initialize return object
	LargeUnsignedInteger rtn{};
	rtn.resize(this->num_segments);

	// Subtracter borrow bits
	ull_t borrow;

	// Subtract RHS operand
	rtn.arr[0] = this->arr[0] - rhs;
	borrow = this->arr[0] < rhs;

	// Half Subtracter to propagate borrow
	for(unsigned int i = 1;  i < this->num_segments;  ++i) {
		rtn.arr[i] = this->arr[i] - borrow;
		borrow = this->arr[i] < borrow;
	}

	// Trim return object
	rtn.trim();

	return rtn;
}


// Return the product of two LargeUnsignedInteger objects as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator*(const LargeUnsignedInteger& rhs) const {
	// Initialize return object
	LargeUnsignedInteger rtn;
	rtn.resize(this->num_segments + rhs.num_segments);

	// Temporary object to accumulate full-word product
	LargeUnsignedInteger prod;
	prod.resize(this->num_segments + rhs.num_segments);

	ull_t prod_ull;	// temporary variable to calculate full-word product
	unsigned int k;	// product index

	// Iterate through half-words of this
	for(unsigned int ih = 0;  ih < this->num_segments * 2;  ++ih) {

		// Iterate through half-words of rhs
		for(unsigned int jh = 0;  jh < rhs.num_segments * 2;  ++jh) {
			// Multiply half-words
			prod_ull = static_cast<ull_t>(this->arr_half[ih]) * rhs.arr_half[jh];

			// Skip if result is 0
			if(prod_ull != 0) {
				// Product index
				k = ih + jh;

				// Set product object
				prod.arr_half[k] = static_cast<unsigned int>(prod_ull);
				prod.arr_half[k+1] = static_cast<unsigned int>(prod_ull >> UINT_BITS);

				// Accumulate product into return object
				rtn += prod;

				// Reset product object
				prod.arr_half[k] = 0;
				prod.arr_half[k+1] = 0;
			}
		}
	}

	// Trim return object
	rtn.trim();

	return rtn;
}


// Return the product of a LargeUnsignedInteger object with a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator*(const ull_t& rhs) const {
	// Initialize return object
	LargeUnsignedInteger rtn;
	rtn.resize(this->num_segments + 1);

	// Temporary object to accumulate full-word product
	LargeUnsignedInteger prod;
	prod.resize(this->num_segments + 1);

	ull_t prod_ull;		// temporary variable to calculate full-word product
	unsigned int carry;	// carry bit
	bool prod_nonzero;	// flag that product is non-zero

	// Iterate through half-words of this
	for(unsigned int ih = 0;  ih < this->num_segments * 2;  ++ih) {
		// Reset flag
		prod_nonzero = false;

		// Multiply half-words using rhs lower-half
		prod_ull = static_cast<ull_t>(this->arr_half[ih]) * static_cast<unsigned int>(rhs);

		// Skip if result is 0
		if(prod_ull != 0) {
			// Set flag
			prod_nonzero = true;

			// Set product object
			prod.arr_half[ih] = static_cast<unsigned int>(prod_ull);
			prod.arr_half[ih+1] = static_cast<unsigned int>(prod_ull >> UINT_BITS);
		}

		// Multiply half-words using rhs upper-half
		prod_ull = static_cast<ull_t>(this->arr_half[ih]) * static_cast<unsigned int>(rhs >> UINT_BITS);

		// Skip if result is 0
		if(prod_ull != 0) {
			// Set flag
			prod_nonzero = true;

			// Accumulate product object
			carry = prod.arr_half[ih+1] > UINT_MAX - static_cast<unsigned int>(prod_ull);
			prod.arr_half[ih+1] += static_cast<unsigned int>(prod_ull);
			prod.arr_half[ih+2] = static_cast<unsigned int>(prod_ull >> UINT_BITS) + carry;
		}

		// Skip if product is zero
		if(prod_nonzero) {
			// Accumulate product into return object
			rtn += prod;

			// Reset product object
			prod.arr_half[ih] = 0;
			prod.arr_half[ih+1] = 0;
			prod.arr_half[ih+2] = 0;
		}
	}

	// Trim return object
	rtn.trim();

	return rtn;
}


quot_rem LargeUnsignedInteger::div_mod(const LargeUnsignedInteger& rhs) const {
	LargeUnsignedInteger quot;	// quotient
	LargeUnsignedInteger rem;	// remainder

	// Divisor is larger, so quotient is 0 and remainder is dividend
	if(this->num_segments < rhs.num_segments)
		rem = *this;

	// Perform division
	else {
		quot.resize(this->num_segments);

		// Reverse-iterate through words
		for(unsigned int i = this->num_segments-1;  i < this->num_segments;  --i)
			// Reverse-iterate through bits
			for(unsigned int j = ULL_BITS-1;  j < ULL_BITS;  --j) {
				// Append next-highest bit to remainder
				rem <<= 1ull;
				rem.arr[0] |= this->arr[i] >> j & 1;

				// Subtract divisor
				if(rem >= rhs) {
					// Track division in quotient
					quot.arr[i] |= 1ull << j;

					// Update remainder
					rem -= rhs;
				}
			}

		// Trim quotient
		quot.trim();
	}

	// Return quotient & remainder as pair
	return std::make_pair(quot, rem);
}


quot_rem LargeUnsignedInteger::div_mod(const ull_t& rhs) const {
	LargeUnsignedInteger quot;	// quotient
	LargeUnsignedInteger rem;	// remainder
	ull_t rem_ull = 0ull;		// ull_t remainder
	bool rem_ovf;				// flag to track remainder overflow

	// Resize quotient
	quot.resize(this->num_segments);

	// Reverse-iterate through words
	for(unsigned int i = this->num_segments-1;  i < this->num_segments;  --i)
		// Reverse-iterate through bits
		for(unsigned int j = ULL_BITS-1;  j < ULL_BITS;  --j) {
			// Track overflow
			rem_ovf = rem_ull & (1ull << 63);

			// Append next-highest bit to remainder
			rem_ull <<= 1;
			rem_ull |= this->arr[i] >> j & 1;

			// Subtract divisor
			if(rem_ull >= rhs || rem_ovf) {
				// Track division in quotient
				quot.arr[i] |= 1ull << j;

				// Update remainder
				rem_ull -= rhs;
			}
		}

	// Trim quotient
	quot.trim();

	// Copy ull_t remainder to object
	rem.arr[0] = rem_ull;

	// Return quotient & remainder as pair
	return std::make_pair(quot, rem);
}


// Return the quotient of one LargeUnsignedInteger object divided by another as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator/(const LargeUnsignedInteger& rhs) const {
	quot_rem res = this->div_mod(rhs);
	return res.first;
}


// Return the quotient of a LargeUnsignedInteger object divided by a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator/(const ull_t& rhs) const {
	quot_rem res = this->div_mod(rhs);
	return res.first;
}


// Return the remainder of one LargeUnsignedInteger object divided by another as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator%(const LargeUnsignedInteger& rhs) const {
	quot_rem res = this->div_mod(rhs);
	return res.second;
}


// Return the remainder of a LargeUnsignedInteger object divided by a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator%(const ull_t& rhs) const {
	quot_rem res = this->div_mod(rhs);
	return res.second;
}


// Return the left-shift of a LargeUnsignedInteger object by a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator<<(const ull_t& rhs) const {
	ull_t shift_cycles = rhs / ULL_BITS;			// Number of times the bit-shift will wrap
	ull_t shift_bits = rhs % ULL_BITS;				// Remaining bit-shift
	ull_t shift_bits_rev = ULL_BITS - shift_bits;	// Bits for reverse-shift

	// Construct return object
	LargeUnsignedInteger rtn;

	// Resize object to append extra bit-shift cycles and potential overflow
	rtn.resize(this->num_segments + shift_cycles + 1);

	// Reverse-iterate through words
	for(unsigned int i = this->num_segments-1;  i < this->num_segments;  --i) {
		// Catch 64-bit shift error
		if(shift_bits > 0)
			// Append high-portion of word
			rtn.arr[i+1+shift_cycles] |= this->arr[i] >> shift_bits_rev;

		// Shift low-portion of word
		rtn.arr[i+shift_cycles] = this->arr[i] << shift_bits;
	}

	// Trim return object
	rtn.trim();

	return rtn;
}


// Return the right-shift of a LargeUnsignedInteger object by a ull as a new object
LargeUnsignedInteger LargeUnsignedInteger::operator>>(const ull_t& rhs) const {
	ull_t shift_cycles = rhs / ULL_BITS;			// Number of times the bit-shift will wrap
	ull_t shift_bits = rhs % ULL_BITS;				// Remaining bit-shift
	ull_t shift_bits_rev = ULL_BITS - shift_bits;	// Bits for reverse-shift

	// Construct return object
	LargeUnsignedInteger rtn;

	// Skip if all words are shifted out
	if(shift_cycles < this->num_segments) {
		// Resize object to remove cleared bit-shift cycles
		rtn.resize(this->num_segments - shift_cycles);

		// Iterate through words
		for(unsigned int i = shift_cycles;  i < this->num_segments-1;  ++i) {
			// Shift high-portion of low-word
			rtn.arr[i-shift_cycles] = this->arr[i] >> shift_bits;

			// Catch 64-bit shift error
			if(shift_bits > 0)
				// Append low-portion of high-word
				rtn.arr[i-shift_cycles] |= this->arr[i+1] << shift_bits_rev;
		}

		// Shift low-portion of highest word
		rtn.arr[rtn.num_segments-1] = this->arr[this->num_segments-1] >> shift_bits;

		// Trim object
		rtn.trim();
	}

	return rtn;
}


// Check if a LargeUnsignedInteger object is less than another object
bool LargeUnsignedInteger::operator<(const LargeUnsignedInteger& rhs) const {
	// Compare array sizes
	if(this->num_segments < rhs.num_segments)
		return true;
	if(this->num_segments > rhs.num_segments)
		return false;

	// Reverse-iterate through array words
	for(unsigned int i = this->num_segments-1;  i < this->num_segments;  --i) {
		if(this->arr[i] < rhs.arr[i])
			return true;
		if(this->arr[i] > rhs.arr[i])
			return false;
	}

	// Numbers are equal
	return false;
}


// Check if a LargeUnsignedInteger object is less than a ull
bool LargeUnsignedInteger::operator<(const ull_t& rhs) const {
	// Check if array is multiple words
	if(this->num_segments > 1)
		return false;

	// Compare directly
	return this->arr[0] < rhs;
}


// Check if a LargeUnsignedInteger object is greater than another object
bool LargeUnsignedInteger::operator>(const LargeUnsignedInteger& rhs) const {
	return rhs < *this;
}


// Check if a LargeUnsignedInteger object is greater than a ull
bool LargeUnsignedInteger::operator>(const ull_t& rhs) const {
	// Check if array is multiple words
	if(this->num_segments > 1)
		return true;

	// Compare directly
	return this->arr[0] > rhs;
}


// Check if a LargeUnsignedInteger object is less than or equal to another object
bool LargeUnsignedInteger::operator<=(const LargeUnsignedInteger& rhs) const {
	return !(rhs < *this);
}


// Check if a LargeUnsignedInteger object is less than or equal to a ull
bool LargeUnsignedInteger::operator<=(const ull_t& rhs) const {
	return !(*this > rhs);
}


// Check if a LargeUnsignedInteger object is greater than or equal to another object
bool LargeUnsignedInteger::operator>=(const LargeUnsignedInteger& rhs) const {
	return !(*this < rhs);
}


// Check if a LargeUnsignedInteger object is greater than or equal to a ull
bool LargeUnsignedInteger::operator>=(const ull_t& rhs) const {
	return !(*this < rhs);
}


// Check if a LargeUnsignedInteger object is equal to another object
bool LargeUnsignedInteger::operator==(const LargeUnsignedInteger& rhs) const {
	// Compare array sizes
	if(this->num_segments != rhs.num_segments)
		return false;

	// Iterate through array words
	for(unsigned int i = 0;  i < this->num_segments;  ++i) {
		if(this->arr[i] != rhs.arr[i])
			return false;
	}

	// Numbers are equal
	return true;
}


// Check if a LargeUnsignedInteger object is equal to a ull
bool LargeUnsignedInteger::operator==(const ull_t& rhs) const {
	// Check if array is multiple words
	if(this->num_segments > 1)
		return false;

	// Compare directly
	return this->arr[0] == rhs;
}


// Check if a LargeUnsignedInteger object is not equal to another object
bool LargeUnsignedInteger::operator!=(const LargeUnsignedInteger& rhs) const {
	return !(*this == rhs);
}


// Check if a LargeUnsignedInteger object is not equal to a ull
bool LargeUnsignedInteger::operator!=(const ull_t& rhs) const {
	return !(*this == rhs);
}


// Accumulate the sum of two LargeUnsignedInteger objects into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator+=(const LargeUnsignedInteger& rhs) {
	// Get min segments
	unsigned int min_segments = MIN(this->num_segments, rhs.num_segments);

	// Resize this
	this->resize(MAX(this->num_segments, rhs.num_segments));

	// Half adder sum. Needed to check for carry out
	ull_t half_sum;

	// Adder carry bit
	ull_t carry = 0;

	unsigned int i = 0;

	// Full Adder through min_segments
	for(;  i < min_segments;  ++i) {
		half_sum = rhs.arr[i] + carry;
		carry = (this->arr[i] > ULL_MAX - half_sum)  ||  (rhs.arr[i] > ULL_MAX - carry);
		this->arr[i] += half_sum;
	}

	// This is smaller array. Full Adder for remaining RHS segments
	if(rhs.num_segments > min_segments) {
		// Process remaining segments
		for(;  i < rhs.num_segments;  ++i) {
			this->arr[i] += rhs.arr[i] + carry;
			carry = rhs.arr[i] > ULL_MAX - carry;
		}
	}
	// This is larger array. Half Adder to propagate carry
	else {
		// Process remaining segments
		for(;  i < this->num_segments && carry;  ++i) {
			half_sum = carry;
			carry = this->arr[i] > ULL_MAX - half_sum;
			this->arr[i] += half_sum;
		}
	}

	// Add additional word for carry out
	if(carry) {
		this->resize(this->num_segments+1);
		this->arr[this->num_segments-1] = 1;
	}

	return *this;
}


// Accumulate the sum of a LargeUnsignedInteger object with a ull into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator+=(const ull_t& rhs) {
	// Half adder sum. Needed to check for carry out
	ull_t half_sum;

	// Adder carry bit
	ull_t carry;

	// Add RHS operand
	carry = this->arr[0] > ULL_MAX - rhs;
	this->arr[0] += rhs;

	// Half Adder to propagate carry
	for(unsigned int i = 1;  i < this->num_segments && carry;  ++i) {
		half_sum = carry;
		carry = this->arr[i] > ULL_MAX - carry;
		this->arr[i] += half_sum;
	}

	// If LHS operand is dynamic and carry out, resize to propagate carry
	if(carry) {
		this->resize(this->num_segments+1);
		this->arr[this->num_segments-1] = 1;
	}

	return *this;
}


// Accumulate the difference of two LargeUnsignedInteger objects into the LHS
// Behavior is undefined if LHS < RHS
LargeUnsignedInteger& LargeUnsignedInteger::operator-=(const LargeUnsignedInteger& rhs) {
	// Get smaller number of segments for initial accumulation
	unsigned int min_segments = MIN(this->num_segments, rhs.num_segments);

	// Resize this
	this->resize(MAX(this->num_segments, rhs.num_segments));

	// Half subtracter sum. Needed to check for borrow in
	ull_t half_sum;

	// Subtracter borrow bits
	ull_t borrow = 0;

	unsigned int i = 0;

	// Full Subtracter
	for(;  i < min_segments;  ++i) {
		half_sum = rhs.arr[i] + borrow;
		borrow = (rhs.arr[i] > ULL_MAX - borrow) || (this->arr[i] < half_sum);
		this->arr[i] -= half_sum;
	}

	// This is smaller array. Full Subtracter for remaining RHS segments.
	if(rhs.num_segments > min_segments) {
		for(;  i < rhs.num_segments;  ++i) {
			half_sum = rhs.arr[i] + borrow;
			borrow = (rhs.arr[i] > ULL_MAX - borrow) || (this->arr[i] < half_sum);
			this->arr[i] -= half_sum;
		}
	}
	// This is larger array. Full Subtracter to propagate borrow
	else {
		for(;  i < this->num_segments && borrow;  ++i) {
			half_sum = borrow;
			borrow = this->arr[i] < borrow;
			this->arr[i] -= half_sum;
		}
	}

	// Trim this
	this->trim();

	return *this;
}


// Accumulate the difference of a LargeUnsignedInteger object with a ull into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator-=(const ull_t& rhs) {
	// Half subtracter sum. Needed to check for borrow in
	ull_t half_sum;

	// Subtracter borrow bit
	ull_t borrow;

	// Subtract RHS operand
	borrow = this->arr[0] < rhs;
	this->arr[0] -= rhs;

	// Full Subtracter to propagate borrow
	for(unsigned int i = 1;  i < this->num_segments && borrow;  ++i) {
		half_sum = borrow;
		borrow = this->arr[i] < borrow;
		this->arr[i] -= half_sum;
	}

	// Trim this
	this->trim();

	return *this;
}


// Accumulate the product of two LargeUnsignedInteger objects into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator*=(const LargeUnsignedInteger& rhs) {
	// Resize this
	this->resize(this->num_segments + rhs.num_segments);

	// Temporary object to accumulate full-word product
	LargeUnsignedInteger prod;
	prod.resize(this->num_segments + rhs.num_segments);

	ull_t prod_ull;	// temporary variable to calculate full-word product
	unsigned int k;	// product index

	// Reverse-iterate through half-words of this
	for(unsigned int ih = (this->num_segments * 2) - 1;  ih < this->num_segments * 2;  --ih) {

		// Reverse-iterate through half-words of rhs
		for(unsigned int jh = (rhs.num_segments * 2) - 1;  jh < rhs.num_segments * 2;  --jh) {
			// Multiply half-words
			prod_ull = static_cast<ull_t>(this->arr_half[ih]) * rhs.arr_half[jh];

			// Clear half-word multiplicand from this, so that product can be accumulated
			if(jh == 0)
				this->arr_half[ih] = 0;

			// Skip if result is 0
			if(prod_ull != 0) {
				// Product index
				k = ih + jh;

				// Set product object
				prod.arr_half[k] = static_cast<unsigned int>(prod_ull);
				prod.arr_half[k+1] = static_cast<unsigned int>(prod_ull >> UINT_BITS);

				// Accumulate product into this
				*this += prod;

				// Reset product object
				prod.arr_half[k] = 0;
				prod.arr_half[k+1] = 0;
			}
		}
	}

	// Trim object
	this->trim();

	return *this;
}


// Accumulate the product of a LargeUnsignedInteger object with a ull into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator*=(const ull_t& rhs) {
	// Resize this
	this->resize(this->num_segments + 1);

	// Temporary object to accumulate full-word product
	LargeUnsignedInteger prod;
	prod.resize(this->num_segments);

	ull_t prod_ull;		// temporary variable to calculate full-word product
	unsigned int carry;	// carry bit
	bool prod_nonzero;	// flag that product is non-zero

	// Reverse-iterate through half-words of this
	for(unsigned int ih = (this->num_segments * 2) - 3;  ih < this->num_segments * 2;  --ih) {
		// Reset flag
		prod_nonzero = false;

		// Multiply half-words using rhs upper-half
		prod_ull = static_cast<ull_t>(this->arr_half[ih]) * static_cast<unsigned int>(rhs >> UINT_BITS);

		// Skip if result is 0
		if(prod_ull != 0) {
			// Set flag
			prod_nonzero = true;

			// Set product object
			prod.arr_half[ih+1] = static_cast<unsigned int>(prod_ull);
			prod.arr_half[ih+2] = static_cast<unsigned int>(prod_ull >> UINT_BITS);
		}

		// Multiply half-words using rhs lower-half
		prod_ull = static_cast<ull_t>(this->arr_half[ih]) * static_cast<unsigned int>(rhs);

		// Skip if result is 0
		if(prod_ull != 0) {
			// Set flag
			prod_nonzero = true;

			// Accumulate product object
			prod.arr_half[ih] = static_cast<unsigned int>(prod_ull);

			carry = prod.arr_half[ih+1] > UINT_MAX - static_cast<unsigned int>(prod_ull >> UINT_BITS);
			prod.arr_half[ih+1] += static_cast<unsigned int>(prod_ull >> UINT_BITS);
			prod.arr_half[ih+2] += carry;
		}

		// Clear half-word multiplicand from this, so that product can be accumulated
		this->arr_half[ih] = 0;

		// Skip if product is zero
		if(prod_nonzero) {
			// Accumulate product in return object
			*this += prod;

			// Reset product object
			prod.arr_half[ih] = 0;
			prod.arr_half[ih+1] = 0;
			prod.arr_half[ih+2] = 0;
		}
	}

	// Trim return object
	this->trim();

	return *this;
}


// Accumulate the quotient of one LargeUnsignedInteger object divided by another into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator/=(const LargeUnsignedInteger& rhs) {
	quot_rem res = this->div_mod(rhs);
	*this = std::move(res.first);
	return *this;
}


// Accumulate the quotient of one LargeUnsignedInteger object divided by a ull the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator/=(const ull_t& rhs) {
	quot_rem res = this->div_mod(rhs);
	*this = std::move(res.first);
	return *this;
}


// Accumulate the remainder of one LargeUnsignedInteger object divided by another into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator%=(const LargeUnsignedInteger& rhs) {
	quot_rem res = this->div_mod(rhs);
	*this = std::move(res.second);
	return *this;
}


// Accumulate the remainder of one LargeUnsignedInteger object divided by a ull the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator%=(const ull_t& rhs) {
	quot_rem res = this->div_mod(rhs);
	*this = std::move(res.second);
	return *this;
}


// Accumulate the left-shift of a LargeUnsignedInteger object by a ull into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator<<=(const ull_t& rhs) {
	ull_t shift_cycles = rhs / ULL_BITS;			// Number of times the bit-shift will wrap
	ull_t shift_bits = rhs % ULL_BITS;				// Remaining bit-shift
	ull_t shift_bits_rev = ULL_BITS - shift_bits;	// Bits for reverse-shift

	// Resize object to append extra bit-shift cycles and potential overflow
	this->resize(this->num_segments + shift_cycles + 1);

	// Reverse-iterate through words
	for(unsigned int i = this->num_segments-2-shift_cycles;  i < this->num_segments;  --i) {
		// Catch 64-bit shift error
		if(shift_bits > 0)
			// Append high-portion of word
			this->arr[i+1+shift_cycles] |= static_cast<ull_t>(this->arr[i]) >> shift_bits_rev;

		// Shift low-portion of word
		this->arr[i+shift_cycles] = this->arr[i] << shift_bits;
	}

	// Clear cycled words
	for(unsigned int i = 0;  i < shift_cycles;  ++i)
		this->arr[i] = 0ull;

	// Trim object
	this->trim();

	return *this;
}


// Accumulate the right-shift of a LargeUnsignedInteger object by a ull into the LHS
LargeUnsignedInteger& LargeUnsignedInteger::operator>>=(const ull_t& rhs) {
	ull_t shift_cycles = rhs / ULL_BITS;			// Number of times the bit-shift will wrap
	ull_t shift_bits = rhs % ULL_BITS;				// Remaining bit-shift
	ull_t shift_bits_rev = ULL_BITS - shift_bits;	// Bits for reverse-shift

	// Reset this if all words are shifted out
	if(shift_cycles >= this->num_segments)
		this->reset();

	// Do right-shift
	else {
		// Iterate through words
		for(unsigned int i = shift_cycles;  i < this->num_segments-1;  ++i) {
			// Shift high-portion of low-word
			this->arr[i-shift_cycles] = this->arr[i] >> shift_bits;

			// Catch 64-bit shift error
			if(shift_bits > 0)
				// Append low-portion of high-word
				this->arr[i-shift_cycles] |= this->arr[i+1] << shift_bits_rev;
		}

		// Shift low-portion of highest word
		this->arr[this->num_segments-1-shift_cycles] = this->arr[this->num_segments-1] >> shift_bits;

		// Clear cycled words
		for(unsigned int i = this->num_segments-shift_cycles;  i < this->num_segments;  ++i)
			this->arr[i] = 0;

		// Trim object
		this->trim();
	}

	return *this;
}


// Prefix increment
LargeUnsignedInteger& LargeUnsignedInteger::operator++() {
	// Increment words until no more overflow
	for(unsigned int i = 0;  i == 0 || (i < this->num_segments && this->arr[i-1] == 0);  ++i)
		++this->arr[i];

	// Add additional word for overflow
	if(this->arr[this->num_segments-1] == 0) {
		this->resize(this->num_segments+1);
		this->arr[this->num_segments-1] = 1;
	}

	return *this;
}


// Prefix decrement
LargeUnsignedInteger& LargeUnsignedInteger::operator--() {
	// Decrement words until no more underflow
	for(unsigned int i = 0;  i == 0 || (i < this->num_segments && this->arr[i-1] == ULL_MAX);  ++i)
		--this->arr[i];

	// Trim this
	this->trim();

	return *this;
}


// Postfix increment
LargeUnsignedInteger LargeUnsignedInteger::operator++(int) {
	// Save copy of this before increment
	LargeUnsignedInteger this_old = *this;

	// Increment words until no more overflow
	for(unsigned int i = 0;  i == 0 || (i < this->num_segments && this->arr[i-1] == 0);  ++i)
		this->arr[i]++;

	// Add additional word for overflow
	if(this->arr[this->num_segments-1] == 0) {
		this->resize(this->num_segments+1);
		this->arr[this->num_segments-1] = 1;
	}

	return this_old;
}


// Postfix decrement
LargeUnsignedInteger LargeUnsignedInteger::operator--(int) {
	// Save copy of this before decrement
	LargeUnsignedInteger this_old = *this;

	// Decrement words until no more underflow
	for(unsigned int i = 0;  i == 0 || (i < this->num_segments && this->arr[i-1] == ULL_MAX);  ++i)
		this->arr[i]--;

	// Trim this
	this->trim();

	return this_old;
}


// Copy assignment
// Copy values of RHS array into this array
LargeUnsignedInteger& LargeUnsignedInteger::operator=(const LargeUnsignedInteger& rhs) {
	// Do nothing if self-assignment
	if(this != &rhs) {
		// Resize if needed
		if(this->num_segments != rhs.num_segments) {
			this->assign_arr(new ull_t[rhs.num_segments]);
			this->num_segments = rhs.num_segments;
		}

		// Copy rhs array to this array
		for(unsigned int i = 0; i < this->num_segments; ++i)
			this->arr[i] = rhs.arr[i];
	}

	return *this;
}


// Copy assignment scalar
// Copy value of RHS into this array
LargeUnsignedInteger& LargeUnsignedInteger::operator=(const ull_t& rhs) {
	this->set(rhs);
	return *this;
}


// Move assignment
// Move values of RHS array into this array
LargeUnsignedInteger& LargeUnsignedInteger::operator=(LargeUnsignedInteger&& rhs) {
	// Do nothing if self-assignment
	if(this != &rhs) {
		// Reassign array
		this->assign_arr(rhs.arr);
		this->num_segments = rhs.num_segments;

		// Nullify rhs
		rhs.num_segments = 0;
		rhs.arr = nullptr;
		rhs.refresh_arr_half();
	}

	return *this;
}


// Move assignment scalar
// Move value of RHS into this array
LargeUnsignedInteger& LargeUnsignedInteger::operator=(ull_t&& rhs) {
	this->set(rhs);
	rhs = 0;
	return *this;
}


// Return quotient & remainder of object divided by ten
// Adapted from Hacker's Delight, 2nd Edition, Section 10-8
unsigned int LargeUnsignedInteger::div_mod_ten(const LargeUnsignedInteger& num, const ull_t& den_pow) {
	LargeUnsignedInteger temp = *this;	// dividend
	LargeUnsignedInteger quot_ten;		// quotient * 10
	unsigned int rem;

	// Compute quotient
	*this = *this * num >> den_pow;

	// Get remainder
	quot_ten = *this * 10ull;
	rem = static_cast<unsigned int>(temp.arr[0] - quot_ten.arr[0]);

	return rem;
}


// Output stream
std::ostream& operator<<(std::ostream& os, const LargeUnsignedInteger& rhs) {
	LargeUnsignedInteger temp = rhs;
	std::string dec_digits;

	// Method using div_mod_ten(). Much slower for large numbers.
	if(rhs.num_segments <= 5) {
		unsigned int num_segments_old = rhs.num_segments;	// number of segments before division

		LargeUnsignedInteger num_one_tenth;		// numerator
		num_one_tenth.resize(rhs.num_segments);

		// Fill numerator segments
		for(unsigned int i = 0;  i < num_one_tenth.num_segments;  ++i)
			num_one_tenth.arr[i] = LargeUnsignedInteger::NUM_ONE_TENTH_INIT;

		// Finalize numerator
		num_one_tenth.arr[0] |= 1ull;

		ull_t den_pow_one_tenth = LargeUnsignedInteger::ULL_BITS * num_segments_old + 3;

		// Iterate through digits
		while(!temp.is_zero()) {
			// Divide by 10, and store remainder digit as character
			dec_digits.push_back(temp.div_mod_ten(num_one_tenth, den_pow_one_tenth) + 0x30);

			if(temp.num_segments < num_segments_old) {
				--num_segments_old;
				num_one_tenth.resize(num_segments_old);
				den_pow_one_tenth -= LargeUnsignedInteger::ULL_BITS;
			}
		}
	}

	// Method using standard division
	else {
		LargeUnsignedInteger digit;

		// Iterate through digits
		while(!temp.is_zero()) {
			digit = temp % 10ull;
			dec_digits.push_back(digit.arr[0] + 0x30);
			temp /= 10ull;
		}
	}

	// Reverse-iterate through digits to output in correct order
	for(std::string::const_reverse_iterator itr = dec_digits.crbegin();  itr != dec_digits.crend();  ++itr)
		os << *itr;

	return os;
}


// Print object debugging info
void LargeUnsignedInteger::print_debug(const std::string name) const {
	std::cout << "LargeUnsignedInteger:   " << name << "\n";

	std::cout << "Object Address:         " << this << "\n";

	std::cout << "Array Address:          " << arr << "\n";

	std::cout << "Array UINT Address:     " << arr_half << "\n";

	std::cout << "Array Data (reversed):  [";
	std::cout << std::hex;
	for(unsigned int i = num_segments-1; i < num_segments; --i) {
		std::cout << "0x";
		std::cout << std::setw(16) << std::setfill('0');
		std::cout << arr[i];
		if(i > 0)
			std::cout << ", ";
	}
	std::cout << "]\n";
	std::cout << std::dec;

	std::cout << "num_segments:           " << num_segments << "\n";

	std::cout << std::endl;
}


bool LargeUnsignedInteger::is_separating_char(char c) {
	return c == '\'' || c == ' ' || c == ',' || c == '.';
}


// Binary string constructor
void LargeUnsignedInteger::construct_string_bin(std::string::const_iterator& itr, std::string::const_iterator& itr_end) {
	char d;

	// Iterate through characters
	for(;  itr != itr_end;  ++itr) {
		// Extract character
		d = *itr;

		// Skip separating characters
		if(!is_separating_char(d)) {
			// Convert to digit
			d -= 0x30;

			// Throw error if character is non-octal
			if(d > 1)
				throw std::invalid_argument("Binary string argument contains invalid non-binary characters.");

			// Append digit
			*this <<= 1ull;
			arr[0] |= static_cast<ull_t>(d);
		}
	}
}


// Octal string constructor
void LargeUnsignedInteger::construct_string_oct(std::string::const_iterator& itr, std::string::const_iterator& itr_end) {
	char d;

	// Iterate through characters
	for(;  itr != itr_end;  ++itr) {
		// Extract character
		d = *itr;

		// Skip separating characters
		if(!is_separating_char(d)) {
			// Convert to digit
			d -= 0x30;

			// Throw error if character is non-octal
			if(d > 7)
				throw std::invalid_argument("Octal string argument contains invalid non-octal characters.");

			// Append digit
			*this <<= 3ull;
			arr[0] |= static_cast<ull_t>(d);
		}
	}
}


// Decimal string constructor
void LargeUnsignedInteger::construct_string_dec(std::string::const_iterator& itr, std::string::const_iterator& itr_end) {
	char d;

	// Iterate through characters
	for(;  itr != itr_end;  ++itr) {
		// Extract character
		d = *itr;

		// Skip separating characters
		if(!is_separating_char(d)) {
			// Convert to digit
			d -= 0x30;

			// Throw error if character is non-decimal
			if(d > 9)
				throw std::invalid_argument("Decimal string argument contains invalid non-decimal characters.");

			// Append digit
			*this *= 10ull;
			*this += static_cast<ull_t>(d);
		}
	}
}


// Hexadecimal string constructor
void LargeUnsignedInteger::construct_string_hex(std::string::const_iterator& itr, std::string::const_iterator& itr_end) {
	char d;

	// Iterate through characters
	for(;  itr != itr_end;  ++itr) {
		// Extract character
		d = *itr;

		// Skip separating characters
		if(!is_separating_char(d)) {
			// Convert character to value
			if(d - 0x30 <= 9)		// digits 0-9
				d -= 0x30;
			else if(d - 0x41 <= 5)	// digits A-F
				d -= 55;
			else if(d - 0x61 <= 5)	// digits a-f
				d -= 87;
			// Throw error if character is non-hexadecimal
			else
				throw std::invalid_argument("Hexadecimal string argument contains invalid non-hexadecimal characters.");

			// Append digit
			*this <<= 4ull;
			arr[0] |= static_cast<ull_t>(d);
		}
	}
}


// Reassign arr to new pointer, and refress arr_half
void LargeUnsignedInteger::assign_arr(ull_t* arr_new) {
	// Delete existing array
	delete[] arr;

	// Assign new array
	arr = arr_new;

	// Refresh arr_half
	refresh_arr_half();
}


// Refresh arr_half to reference arr
void LargeUnsignedInteger::refresh_arr_half() {
	arr_half = reinterpret_cast<unsigned int*>(arr);
}


// Resize
void LargeUnsignedInteger::resize(unsigned int len) {
	// Skip if size is unchanged
	if(len != num_segments) {
		ull_t* new_arr = new ull_t[len];
		unsigned int i = 0;

		// Get smaller number of segments for initial copy
		unsigned int min_segments = MIN(len, num_segments);

		// Copy old array to new array
		for(; i < min_segments; ++i)
			new_arr[i] = arr[i];

		// Fill remaining elements with zeros
		for(; i < len; ++i)
			new_arr[i] = 0;

		// Reset members
		assign_arr(new_arr);
		num_segments = len;
	}
}


// Resize to minimum needed words
void LargeUnsignedInteger::trim() {
	// Get minimum needed words
	unsigned int min_segments = num_segments;
	while(arr[--min_segments] == 0ull && min_segments > 0);

	// Resize
	resize(MAX(min_segments+1, 1));
}
