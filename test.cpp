
#include "LargeUnsignedInteger.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <utility>
#include <chrono>

#define TEST_FUNC(func) test_wrapper(&func, #func)

using namespace std;


void test_wrapper(void (*func)(void), const string& name) {
	cout << name << "\n" << endl;
	func();
	cout << endl;
}


void test_default_constructor() {
	LargeUnsignedInteger a;
	PRINT_DEBUG(a);
}


void test_scalar_constructor() {
	LargeUnsignedInteger a{ULL_MAX};
	PRINT_DEBUG(a);
}


void test_array_constructor() {
	constexpr unsigned int a_len = 5;
	ull_t a_arr[a_len] = {0ull, 1ull, 2ull, 3ull, 0ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);
}


void test_string_constructor() {
	string str = "18'446'744,073,709,551,615,000.000 000";
	LargeUnsignedInteger a{str};
	cout << str << endl;
	PRINT_DEBUG(a);

	str = "0b01'10000111 10100111.00101101,00010110";
	LargeUnsignedInteger b{str};
	cout << str << endl;
	PRINT_DEBUG(b);

	str = "0071230'564712350067'501320570123'560075126470 123570005621,322567556120.357745062321";
	LargeUnsignedInteger c{str};
	cout << str << endl;
	PRINT_DEBUG(c);

	str = "0x02'abcdef26'ABCDEF12 3302d032,5E489794.502f6547";
	LargeUnsignedInteger d{str};
	cout << str << endl;
	PRINT_DEBUG(d);
}

void test_copy_constructor() {
	LargeUnsignedInteger a{0x123ull};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b{a};
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
}

void test_move_constructor() {
	LargeUnsignedInteger a{0x123ull};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b{std::move(a)};
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
}


void test_get_size() {
	constexpr unsigned int a_len = 5;
	ull_t a_arr[a_len] = {0ull, 1ull, 2ull, 3ull, 0ull};
	LargeUnsignedInteger a{a_len, a_arr};
	cout << a.get_size() << endl;
}


void test_is_zero() {
	LargeUnsignedInteger a{0ull};
	cout << a.is_zero() << endl;

	LargeUnsignedInteger b{1ull};
	cout << b.is_zero() << endl;

	constexpr unsigned int c_len = 2;
	ull_t c_arr[c_len] = {1ull, 2ull};
	LargeUnsignedInteger c{c_len, c_arr};
	cout << c.is_zero() << endl;
}


void test_reset() {
	LargeUnsignedInteger a{1ull};
	PRINT_DEBUG(a);
	a.reset();
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 2ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);
	b.reset();
	PRINT_DEBUG(b);
}


void test_set() {
	LargeUnsignedInteger a{1ull};
	PRINT_DEBUG(a);
	a.set(2ull);
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 2ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);
	b.set(ULL_MAX);
	PRINT_DEBUG(b);
}


void test_set_array() {
	LargeUnsignedInteger a{0x123ull};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 2ull};
	a.set(b_len, b_arr);
	PRINT_DEBUG(a);

	b_arr[0] = 0ull;
	b_arr[1] = ULL_MAX;
	a.set(b_len, b_arr);
	PRINT_DEBUG(a);
}


void test_set_string() {
	LargeUnsignedInteger a{ULL_MAX};
	PRINT_DEBUG(a);

	string str = "18'446'744,073,709,551,615,000.000 000";
	a.set(str);
	cout << str << endl;
	PRINT_DEBUG(a);

	str = "0b01'10000111 10100111.00101101,00010110";
	a.set(str);
	cout << str << endl;
	PRINT_DEBUG(a);

	str = "0071230'564712350067'501320570123'560075126470 123570005621,322567556120.357745062321";
	a.set(str);
	cout << str << endl;
	PRINT_DEBUG(a);

	str = "0x02'abcdef26'ABCDEF12 3302d032,5E489794.502f6547";
	a.set(str);
	cout << str << endl;
	PRINT_DEBUG(a);
}


void test_get_low_word() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {1ull, 2ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = a.get_low_word();
	cout << "0x" << std::hex << b << endl;
}


void test_addition_object() {
	constexpr unsigned int a_len = 5;
	ull_t a_arr[a_len] = {ULL_MAX>>1, 1ull, 0ull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {(ULL_MAX>>1)+1, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger b{b_len, b_arr};

	LargeUnsignedInteger c = a + b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(c);

	c = b + a;
	PRINT_DEBUG(c);
}


void test_addition_ull() {
	LargeUnsignedInteger a{0x45600ull};
	ull_t b = 0x123ull;
	LargeUnsignedInteger c = a + b;
	PRINT_DEBUG(a);
	cout << std::hex << "0x" << b << endl;
	PRINT_DEBUG(c);

	constexpr unsigned int d_len = 2;
	ull_t d_arr[d_len] = {ULL_MAX, ULL_MAX};
	LargeUnsignedInteger d{d_len, d_arr};
	LargeUnsignedInteger e = d + b;
	PRINT_DEBUG(d);
	cout << std::hex << "0x" << b << endl;
	PRINT_DEBUG(e);
}


void test_subtraction_object() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0x123ull, 0x456ull};
	LargeUnsignedInteger a{a_len, a_arr};

	LargeUnsignedInteger b = a;

	LargeUnsignedInteger c = a - b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(c);

	b = ULL_MAX;
	c = a - b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(c);

	c = b - a;
	PRINT_DEBUG(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(c);
}


void test_subtraction_ull() {
	LargeUnsignedInteger a{ULL_MAX>>1};
	ull_t b = 0x123ull;
	LargeUnsignedInteger c = a - b;
	PRINT_DEBUG(a);
	cout << std::hex << "0x" << b << endl;
	PRINT_DEBUG(c);

	b = ULL_MAX;
	c = a - b;
	PRINT_DEBUG(a);
	cout << std::hex << "0x" << b << endl;
	PRINT_DEBUG(c);

	constexpr unsigned int d_len = 2;
	ull_t d_arr[d_len] = {0x123ull, 0x456ull};
	LargeUnsignedInteger d{d_len, d_arr};
	LargeUnsignedInteger e = d - b;
	PRINT_DEBUG(d);
	cout << std::hex << "0x" << b << endl;
	PRINT_DEBUG(e);
}


void test_multiplication_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0000000800000001ull, 0, 0x8000000000000001ull};
	LargeUnsignedInteger a{a_len, a_arr};

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 0x1000000000000001ull};
	LargeUnsignedInteger b{b_len, b_arr};

	LargeUnsignedInteger c = a * b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(c);
}


void test_multiplication_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {1ull, 0x7000000000000001ull};
	LargeUnsignedInteger a{a_len, a_arr};

	ull_t b = ULL_MAX;

	LargeUnsignedInteger c = a * b;
	PRINT_DEBUG(a);
	cout << "0x" << std::hex << b << endl;
	PRINT_DEBUG(c);
}


void test_div_mod_object() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};

	LargeUnsignedInteger b{1ull};

	quot_rem qr = a.div_mod(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);

	qr = b.div_mod(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);

	b = a;
	qr = a.div_mod(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);

	b = a + 1ull;
	qr = a.div_mod(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);

	a = 0x1548723a'6f98c024ull;
	b = 0x12345ull;
	qr = a.div_mod(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);
}


void test_div_mod_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};

	ull_t b = 1ull;

	quot_rem qr = a.div_mod(b);
	PRINT_DEBUG(a);
	cout << "0x" << std::hex << b << endl;
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);

	a = 0x1548723a'6f98c024ull;
	b = 0x12345ull;
	qr = a.div_mod(b);
	PRINT_DEBUG(a);
	cout << "0x" << std::hex << b << endl;
	PRINT_DEBUG(qr.first);
	PRINT_DEBUG(qr.second);
}


void test_division_modulus_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0123456789abcdefull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};

	LargeUnsignedInteger c = a / b;
	LargeUnsignedInteger d = a % b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	PRINT_DEBUG(c);
	PRINT_DEBUG(d);

	LargeUnsignedInteger e = c * b + d;
	PRINT_DEBUG(e);
}


void test_division_modulus_ull() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0123456789abcdefull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};

	ull_t b = 0xfedcba9876543210ull;

	LargeUnsignedInteger c = a / b;
	LargeUnsignedInteger d = a % b;
	PRINT_DEBUG(a);
	cout << "0x" << std::hex << b << endl;
	PRINT_DEBUG(c);
	PRINT_DEBUG(d);

	LargeUnsignedInteger e = c * b + d;
	PRINT_DEBUG(e);
}


void test_left_shift() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {ULL_MAX, 0x00000000F7310248ull};
	LargeUnsignedInteger a{a_len, a_arr};

	ull_t shift_bits = 16ull;
	LargeUnsignedInteger b = a << shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);

	shift_bits = 48ull;
	b = a << shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);

	shift_bits = 128ull + 48ull;
	b = a << shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);
}


void test_right_shift() {
	constexpr unsigned int a_len = 5;
	ull_t a_arr[a_len] = {ULL_MAX, 0ull, 0x00000000F7310248ull, ULL_MAX, 0x00000000FFFFFFFFull};
	LargeUnsignedInteger a{a_len, a_arr};

	ull_t shift_bits = 16ull;
	LargeUnsignedInteger b = a >> shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);

	shift_bits = 32ull;
	b = a >> shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);

	shift_bits = 128ull + 32ull;
	b = a >> shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);

	shift_bits = 64ull * 5;
	b = a >> shift_bits;
	PRINT_DEBUG(a);
	cout << shift_bits << endl;
	PRINT_DEBUG(b);
}


void test_less_than_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;

	res = b < a;
	cout << "b < a" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;

	res = b < a;
	cout << "b < a" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;

	res = b < a;
	cout << "b < a" << endl;
	cout << res << endl;
}


void test_less_than_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX;
	cout << "0x" << std::hex << b << endl;

	bool res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;

	a.set(0ull);
	PRINT_DEBUG(a);

	res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a < b;
	cout << "a < b" << endl;
	cout << res << endl;
}


void test_greater_than_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;

	res = b > a;
	cout << "b > a" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;

	res = b > a;
	cout << "b > a" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;

	res = b > a;
	cout << "b > a" << endl;
	cout << res << endl;
}


void test_greater_than_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX-1;
	cout << "0x" << std::hex << b << endl;

	bool res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;

	a.set(0ull);
	PRINT_DEBUG(a);

	res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a > b;
	cout << "a > b" << endl;
	cout << res << endl;
}


void test_less_than_or_equal_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;

	res = b <= a;
	cout << "b <= a" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;

	res = b <= a;
	cout << "b <= a" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;

	res = b <= a;
	cout << "b <= a" << endl;
	cout << res << endl;
}


void test_less_than_or_equal_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX-1;
	cout << "0x" << std::hex << b << endl;

	bool res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;

	a.set(ULL_MAX-1);
	PRINT_DEBUG(a);

	res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a <= b;
	cout << "a <= b" << endl;
	cout << res << endl;
}


void test_greater_than_or_equal_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;

	res = b >= a;
	cout << "b >= a" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;

	res = b >= a;
	cout << "b >= a" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;

	res = b >= a;
	cout << "b >= a" << endl;
	cout << res << endl;
}


void test_greater_than_or_equal_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX;
	cout << "0x" << std::hex << b << endl;

	bool res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;

	a.set(ULL_MAX-1);
	PRINT_DEBUG(a);

	res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a >= b;
	cout << "a >= b" << endl;
	cout << res << endl;
}


void test_equal_to_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;
}


void test_equal_to_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX;
	cout << "0x" << std::hex << b << endl;

	bool res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;

	a.set(0ull);
	PRINT_DEBUG(a);

	res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a == b;
	cout << "a == b" << endl;
	cout << res << endl;
}


void test_not_equal_to_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {ULL_MAX, 1ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 0ull, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	bool res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;

	b_arr[1] = 1ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;

	b_arr[2] = 0ull;
	b.set(b_len, b_arr);
	PRINT_DEBUG(b);

	res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;
}


void test_not_equal_to_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX;
	cout << "0x" << std::hex << b << endl;

	bool res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;

	a.set(0ull);
	PRINT_DEBUG(a);

	res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;

	a.set(ULL_MAX);
	PRINT_DEBUG(a);

	res = a != b;
	cout << "a != b" << endl;
	cout << res << endl;
}


void test_addition_assign_object() {
	constexpr unsigned int a_len = 5;
	ull_t a_arr[a_len] = {ULL_MAX>>1, 1ull, 0ull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {(ULL_MAX>>1)+1, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger b{b_len, b_arr};

	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	a += b;
	PRINT_DEBUG(a);

	b += a;
	PRINT_DEBUG(b);
}


void test_addition_assign_ull() {
	LargeUnsignedInteger a{0x45600ull};
	ull_t b = 0x123ull;
	PRINT_DEBUG(a);
	cout << std::hex << "0x" << b << endl;

	a += b;
	PRINT_DEBUG(a);

	constexpr unsigned int c_len = 2;
	ull_t c_arr[c_len] = {ULL_MAX, ULL_MAX};
	LargeUnsignedInteger c{c_len, c_arr};
	PRINT_DEBUG(c);
	cout << std::hex << "0x" << b << endl;

	c += b;
	PRINT_DEBUG(c);
}


void test_subtraction_assign_object() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {0x123ull, 0x456ull};
	LargeUnsignedInteger a{a_len, a_arr};

	LargeUnsignedInteger b = a;

	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	a -= b;
	PRINT_DEBUG(a);

	a.set(a_len, a_arr);
	b = ULL_MAX;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);
	a -= b;
	PRINT_DEBUG(a);

	PRINT_DEBUG(b);
	PRINT_DEBUG(a);
	b -= a;
	PRINT_DEBUG(b);
}


void test_subtraction_assign_ull() {
	LargeUnsignedInteger a{ULL_MAX>>1};
	ull_t b = 0x123ull;
	PRINT_DEBUG(a);
	cout << std::hex << "0x" << b << endl;

	a -= b;
	cout << "a -= b" << endl;
	PRINT_DEBUG(a);

	b = ULL_MAX;
	cout << std::hex << "0x" << b << endl;

	a -= b;
	cout << "a -= b" << endl;
	PRINT_DEBUG(a);

	constexpr unsigned int d_len = 2;
	ull_t d_arr[d_len] = {0x123ull, 0x456ull};
	LargeUnsignedInteger d{d_len, d_arr};
	PRINT_DEBUG(d);
	cout << std::hex << "0x" << b << endl;

	d -= b;
	cout << "d -= b" << endl;
	PRINT_DEBUG(d);

	d_arr[0] = 0ull;
	d_arr[1] = 1ull;
	d.set(d_len, d_arr);
	PRINT_DEBUG(d);

	b = 1ull;
	cout << std::hex << "0x" << b << endl;

	d -= b;
	cout << "d -= b" << endl;
	PRINT_DEBUG(d);
}


void test_multiplication_assign_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0000000800000001ull, 0, 0x8000000000000001ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 0x1000000000000001ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	a *= b;
	cout << "a *= b" << endl;
	PRINT_DEBUG(a);
}


void test_multiplication_assign_ull() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {1ull, 0x8000000000000001ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = ULL_MAX;
	cout << "0x" << std::hex << b << endl;

	a *= b;
	cout << "a *= b" << endl;
	PRINT_DEBUG(a);
}


void test_division_modulus_assign_object() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0123456789abcdefull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 3;
	ull_t b_arr[b_len] = {ULL_MAX, 1ull};
	LargeUnsignedInteger b{b_len, b_arr};

	LargeUnsignedInteger c = a;
	a /= b;
	c %= b;

	PRINT_DEBUG(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(c);

	LargeUnsignedInteger d = a * b + c;
	PRINT_DEBUG(d);
}


void test_division_modulus_assign_ull() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0x0123456789abcdefull, ULL_MAX, ULL_MAX};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t b = 0xfedcba9876543210ull;

	LargeUnsignedInteger c = a;

	a /= b;
	c %= b;
	cout << "0x" << std::hex << b << endl;
	PRINT_DEBUG(a);
	PRINT_DEBUG(c);

	LargeUnsignedInteger d = a * b + c;
	PRINT_DEBUG(d);
}


void test_left_shift_assign() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {ULL_MAX, 0x00000000F7310248ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t shift_bits = 16ull;
	a <<= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);

	shift_bits = 32ull;
	a <<= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);

	shift_bits = 128ull + 16ull;
	a <<= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);
}


void test_right_shift_assign() {
	constexpr unsigned int a_len = 7;
	ull_t a_arr[a_len] = {ULL_MAX, ULL_MAX, ULL_MAX, 0ull, 0x00000000F7310248ull, ULL_MAX, 0x00000000FFFFFFFFull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	ull_t shift_bits = 16ull;
	a >>= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);

	shift_bits = 16ull;
	a >>= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);

	shift_bits = 128ull + 32ull;
	a >>= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);

	shift_bits = 64ull * 4;
	a >>= shift_bits;
	cout << shift_bits << endl;
	PRINT_DEBUG(a);
}


void test_prefix_increment() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {ULL_MAX, ULL_MAX-1,};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b = ++a;
	cout << "++a" << endl;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	++a;
	cout << "++a" << endl;
	PRINT_DEBUG(a);

	a_arr[1] = ULL_MAX;
	a.set(a_len, a_arr);
	PRINT_DEBUG(a);

	++a;
	cout << "++a" << endl;
	PRINT_DEBUG(a);
}


void test_prefix_decrement() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0ull, 0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b = --a;
	cout << "--a" << endl;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	--a;
	cout << "--a" << endl;
	PRINT_DEBUG(a);

	a_arr[0] = 0ull;
	a_arr[1] = ULL_MAX;
	a_arr[2] = 0ull;
	a.set(a_len, a_arr);
	PRINT_DEBUG(a);

	--a;
	cout << "--a" << endl;
	PRINT_DEBUG(a);
}


void test_postfix_increment() {
	constexpr unsigned int a_len = 2;
	ull_t a_arr[a_len] = {ULL_MAX, ULL_MAX-1,};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b = a++;
	cout << "a++" << endl;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	a++;
	cout << "a++" << endl;
	PRINT_DEBUG(a);

	a_arr[1] = ULL_MAX;
	a.set(a_len, a_arr);
	PRINT_DEBUG(a);

	a++;
	cout << "a++" << endl;
	PRINT_DEBUG(a);
}


void test_postfix_decrement() {
	constexpr unsigned int a_len = 3;
	ull_t a_arr[a_len] = {0ull, 0ull, 1ull};
	LargeUnsignedInteger a{a_len, a_arr};
	PRINT_DEBUG(a);

	LargeUnsignedInteger b = a--;
	cout << "a--" << endl;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	a--;
	cout << "a--" << endl;
	PRINT_DEBUG(a);

	a_arr[0] = 0ull;
	a_arr[1] = ULL_MAX;
	a_arr[2] = 0ull;
	a.set(a_len, a_arr);
	PRINT_DEBUG(a);

	a--;
	cout << "a--" << endl;
	PRINT_DEBUG(a);
}


void test_copy_assign_object() {
	LargeUnsignedInteger a;
	LargeUnsignedInteger b{0x123ull};
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	a = b;
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	constexpr unsigned int c_len = 2;
	ull_t c_arr[c_len] = {1ull, 2ull};
	LargeUnsignedInteger c{c_len, c_arr};
	PRINT_DEBUG(c);

	a = c;
	PRINT_DEBUG(a);
	PRINT_DEBUG(c);
}


void test_copy_assign_ull() {
	LargeUnsignedInteger a{1ull};
	PRINT_DEBUG(a);

	ull_t v = 0x123ull;
	a = v;
	cout << std::hex << "0x" << v << endl;
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 2ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	v = 0x456ull;
	b = v;
	cout << std::hex << "0x" << v << endl;
	PRINT_DEBUG(b);
}


void test_move_assign_object() {
	LargeUnsignedInteger a;
	LargeUnsignedInteger b{0x123ull};
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	a = std::move(b);
	PRINT_DEBUG(a);
	PRINT_DEBUG(b);

	constexpr unsigned int c_len = 2;
	ull_t c_arr[c_len] = {1ull, 2ull};
	LargeUnsignedInteger c{c_len, c_arr};
	PRINT_DEBUG(c);

	a = std::move(c);
	PRINT_DEBUG(a);
	PRINT_DEBUG(c);
}


void test_move_assign_ull() {
	LargeUnsignedInteger a{1ull};
	PRINT_DEBUG(a);

	a = 0x123ull;
	PRINT_DEBUG(a);

	constexpr unsigned int b_len = 2;
	ull_t b_arr[b_len] = {1ull, 2ull};
	LargeUnsignedInteger b{b_len, b_arr};
	PRINT_DEBUG(b);

	b = 0x456ull;
	PRINT_DEBUG(b);
}


void test_ostream() {
	LargeUnsignedInteger a{"1234567890'1234567890'1234567890"};
	PRINT_DEBUG(a);

	cout << "\n" << a << endl;
}


void time_ostream() {
//	LargeUnsignedInteger a{"1234567890'1234567890'1234567890"};
//	PRINT_DEBUG(a);
//
//	cout << "\n" << a << endl;


	string word = "ffffffffffffffff";
	LargeUnsignedInteger a;

	string a_str = "0x";

	a.set(a_str);

	constexpr unsigned int sets = 10;
	const unsigned int reps = 10;
	double time_sum;
	long long results[sets];

	chrono::time_point<chrono::high_resolution_clock> t_start, t_stop;
	chrono::duration<long long, chrono::microseconds::period> t_diff;

	for(unsigned int i = 0;  i < sets;  ++i) {
		a_str += word;
		a.set(a_str);

		time_sum = 0;

		for(unsigned int j = 0;  j < reps;  ++j) {
			t_start = chrono::high_resolution_clock::now();

			cout << a;

			t_stop = chrono::high_resolution_clock::now();
			t_diff = chrono::duration_cast<chrono::microseconds>(t_stop - t_start);

			time_sum += t_diff.count();
		}

		results[i] = time_sum / reps;
	}

	cout << "\n\n" << endl;

	for(unsigned int i = 0;  i < sets;  ++i) {
		cout << i << ":  " << results[i] / 1000 << ".";
		cout << setw(4) << setfill('0') << results[i] % 1000 << " ms" << endl;
	}
}


int main() {
//	TEST_FUNC(test_default_constructor);
//	TEST_FUNC(test_scalar_constructor);
//	TEST_FUNC(test_array_constructor);
//	TEST_FUNC(test_string_constructor);
//	TEST_FUNC(test_copy_constructor);
//	TEST_FUNC(test_move_constructor);

//	TEST_FUNC(test_get_size);
//	TEST_FUNC(test_is_zero);
//	TEST_FUNC(test_reset);
//	TEST_FUNC(test_set);
//	TEST_FUNC(test_set_array);
//	TEST_FUNC(test_set_string);

//	TEST_FUNC(test_get_low_word);

//	TEST_FUNC(test_addition_object);
//	TEST_FUNC(test_addition_ull);

//	TEST_FUNC(test_subtraction_object);
//	TEST_FUNC(test_subtraction_ull);

//	TEST_FUNC(test_multiplication_object);
//	TEST_FUNC(test_multiplication_ull);

//	TEST_FUNC(test_div_mod_object);
//	TEST_FUNC(test_div_mod_ull);
//	TEST_FUNC(test_division_modulus_object);
//	TEST_FUNC(test_division_modulus_ull);

//	TEST_FUNC(test_left_shift);
//	TEST_FUNC(test_right_shift);

//	TEST_FUNC(test_less_than_object);
//	TEST_FUNC(test_less_than_ull);

//	TEST_FUNC(test_greater_than_object);
//	TEST_FUNC(test_greater_than_ull);

//	TEST_FUNC(test_less_than_or_equal_object);
//	TEST_FUNC(test_less_than_or_equal_ull);

//	TEST_FUNC(test_greater_than_or_equal_object);
//	TEST_FUNC(test_greater_than_or_equal_ull);

//	TEST_FUNC(test_equal_to_object);
//	TEST_FUNC(test_equal_to_ull);

//	TEST_FUNC(test_not_equal_to_object);
//	TEST_FUNC(test_not_equal_to_ull);

//	TEST_FUNC(test_addition_assign_object);
//	TEST_FUNC(test_addition_assign_ull);

//	TEST_FUNC(test_subtraction_assign_object);
//	TEST_FUNC(test_subtraction_assign_ull);

//	TEST_FUNC(test_multiplication_assign_object);
//	TEST_FUNC(test_multiplication_assign_ull);

//	TEST_FUNC(test_division_modulus_assign_object);
//	TEST_FUNC(test_division_modulus_assign_ull);

//	TEST_FUNC(test_left_shift_assign);
//	TEST_FUNC(test_right_shift_assign);

//	TEST_FUNC(test_prefix_increment);
//	TEST_FUNC(test_prefix_decrement);

//	TEST_FUNC(test_postfix_increment);
//	TEST_FUNC(test_postfix_decrement);

//	TEST_FUNC(test_copy_assign_object);
//	TEST_FUNC(test_copy_assign_ull);
//	TEST_FUNC(test_move_assign_object);
//	TEST_FUNC(test_move_assign_ull);

	TEST_FUNC(test_ostream);
}
