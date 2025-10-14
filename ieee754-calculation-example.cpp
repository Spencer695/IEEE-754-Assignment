#include <stdint.h>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

#define NUM_TESTS 10
#define MAX_VALUE 100
#define MIN_VALUE -100
uint8_t const table_width[] = {12, 12, 35, 12};

// IEEE 754 single-precision float constants
uint8_t const width = 32U;
uint8_t const exp_width = 8U;
uint8_t const mantissa_width = width - exp_width - 1;
uint8_t const bias = 127U;
uint32_t const MANTISSA_MASK = 0x007FFFFFU;
uint8_t const EXPONENT_ZERO = 0U;
uint8_t const EXPONENT_INFINITY_NAN = 255U;
/*
 * *** STUDENTS SHOULD WRITE CODE FOR THIS FUNCTION ***
 * Students should create or add any data structures needed.
 * Students should create or add any functions or classes they may need.
 */
uint32_t extract_sign(uint32_t const data) {
    return (data >> 31) & 0x1U;
}

uint32_t extract_exponent(uint32_t const data) {
    return (data >> 23) & 0xFFU;
}

uint32_t extract_mantissa(uint32_t const data) {
    return data & MANTISSA_MASK;
}

float calculate_mantissa_fraction(uint32_t const mantissa) {
    return static_cast<float>(mantissa) / static_cast<float>(1 << mantissa_width);
]
   
float apply_sign(float const value, uint32_t const sign_bit) {
    return sign_bit ? -value : value;
}

float ieee_754(uint32_t const data) {
    uint32_t const sign_bit = extract_sign(data);
    uint32_t const exponent = extract_exponent(data);
    uint32_t const mantissa = extract_mantissa(data);
    
    // Special case: Zero
    if (exponent == EXPONENT_ZERO && mantissa == 0) {
        return apply_sign(0.0f, sign_bit);
    }
    
    // Special case: Denormalized numbers
    if (exponent == EXPONENT_ZERO) {
        float const mantissa_fraction = calculate_mantissa_fraction(mantissa);
        float const result = mantissa_fraction * powf(2.0f, 1 - bias);
        return apply_sign(result, sign_bit);
    }
    
    // Special case: Infinity
    if (exponent == EXPONENT_INFINITY_NAN && mantissa == 0) {
        return apply_sign(numeric_limits<float>::infinity(), sign_bit);
    }
    
    // Special case: Not a number
    if (exponent == EXPONENT_INFINITY_NAN) {
        return numeric_limits<float>::quiet_NaN();
}
    
// Normal case
    int32_t const actual_exponent = static_cast<int32_t>(exponent) - bias;
    float const mantissa_fraction = calculate_mantissa_fraction(mantissa);
    float const mantissa_value = 1.0f + mantissa_fraction;
    float const result = mantissa_value * powf(2.0f, static_cast<float>(actual_exponent));
    
return apply_sign(result, sign_bit);
}
/*
 * *** STUDENTS SHOULD NOT NEED TO CHANGE THE CODE BELOW. IT IS A CUSTOM TEST HARNESS. ***
 */

void header()
{
    cout << left << setw(table_width[0]) << setfill(' ') << "pass/fail";
    cout << left << setw(table_width[1]) << setfill(' ') << "value";
    cout << left << setw(table_width[2]) << setfill(' ') << "bits";
    cout << left << setw(table_width[3]) << setfill(' ') << "IEEE-754" << endl;

    cout << left << setw(table_width[0]) << setfill(' ') << "--------";
    cout << left << setw(table_width[1]) << setfill(' ') << "--------";
    cout << left << setw(table_width[2]) << setfill(' ') << "--------";
    cout << left << setw(table_width[3]) << setfill(' ') << "--------" << endl;
}

void print_row(bool const test_success, float const rand_val, uint32_t const val_int, float const ieee_754_value)
{
    // print results
    string const pass_fail = test_success ? "PASS" : "FAIL";
    cout << left << setw(table_width[0]) << setfill(' ') << pass_fail;
    cout << left << setw(table_width[1]) << setfill(' ') << rand_val;
    cout << left << setw(table_width[2]) << setfill(' ') << bitset<width>(val_int);
    cout << left << setw(table_width[3]) << setfill(' ') << ieee_754_value << endl;
}

template <typename T>
T rand_min_max(T const min, T const max)
{
    T const rand_val =
        min + static_cast<double>(static_cast<double>(rand())) / (static_cast<double>(RAND_MAX / (max - min)));
    return rand_val;
}

bool test()
{
    // the union
    union float_uint
    {
        float val_float;
        uint32_t val_int;
    } data;

    // print header
    header();

    // seed the random number generator
    srand(time(NULL));

    bool success = true;
    uint16_t pass = 0;
    for (size_t i = 0; i < NUM_TESTS; i++)
    {
        // random value
        float const rand_val = rand_min_max<float>(MIN_VALUE, MAX_VALUE);

        data.val_float = rand_val;

        // calculate using ieee_754 function
        float ieee_754_value = ieee_754(data.val_int);

        // test the results
        float const epsilon = std::numeric_limits<float>::epsilon();
        bool test_success = (abs(ieee_754_value - rand_val) < epsilon);
        if (test_success)
        {
            pass += 1;
        }

        // print row
        print_row(test_success, rand_val, data.val_int, ieee_754_value);
    }

    // summarize results
    cout << "-------------------------------------------" << endl;
    if (pass == NUM_TESTS)
    {
        cout << "SUCCESS ";
    }
    else
    {
        cout << "FAILURE ";
    }
    cout << pass << "/" << NUM_TESTS << " passed" << endl;
    cout << "-------------------------------------------" << endl;

    return success;
}

int main()
{
    if (!test())
    {
        return -1;
    }
    return 0;
}
