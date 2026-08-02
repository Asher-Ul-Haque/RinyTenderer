/**
 * @file test_vector.cpp
 * @brief Comprehensive unit tests for vector.hpp using testManager.hpp
 */

#include <math/vector.hpp>
#include <utils/testManager.hpp>
#include <iostream>

using namespace utils;

// -----------------------------------------------------------------------------
// 1. Constructors & Assignment
// -----------------------------------------------------------------------------

u8 test_default_constructor()
{
    Vector<f32, 3> v;
    EXPECT_TO_BE(3, v.size());
    return PASS_TEST;
}

u8 test_initializer_list_constructor()
{
    Vector<f32, 3> v({1.0f, 2.0f, 3.0f});
    EXPECT_FLOAT_TO_BE(1.0f, v[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(2.0f, v[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(3.0f, v[2], 1e-5f);
    return PASS_TEST;
}

u8 test_copy_constructor()
{
    Vector<f32, 3> original({10.0f, 20.0f, 30.0f});
    Vector<f32, 3> copy(original);

    EXPECT_TO_BE_TRUE(original == copy);
    EXPECT_FLOAT_TO_BE(10.0f, copy[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(20.0f, copy[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(30.0f, copy[2], 1e-5f);
    return PASS_TEST;
}

u8 test_fill_value_constructor()
{
    Vector<i32, 4> v(42);
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        EXPECT_TO_BE(42, v[i]);
    }
    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// 2. Element Access & Iterators
// -----------------------------------------------------------------------------

u8 test_subscript_operator()
{
    Vector<f64, 3> v({1.5, 2.5, 3.5});
    
    // Non-const write & read
    v[1] = 9.9;
    EXPECT_FLOAT_TO_BE(9.9, v[1], 1e-5);

    // Const read
    const Vector<f64, 3>& constRef = v;
    EXPECT_FLOAT_TO_BE(1.5, constRef[0], 1e-5);
    EXPECT_FLOAT_TO_BE(9.9, constRef[1], 1e-5);
    EXPECT_FLOAT_TO_BE(3.5, constRef[2], 1e-5);

    return PASS_TEST;
}

u8 test_iterators()
{
    Vector<i32, 4> v({10, 20, 30, 40});

    // Range-based for loop (non-const)
    i32 sum = 0;
    for (auto& val : v)
    {
        val += 1;
        sum += val;
    }
    // Values become 11, 21, 31, 41 => sum = 104
    EXPECT_TO_BE(104, sum);

    // Const iterators
    const Vector<i32, 4>& constV = v;
    i32 constSum = 0;
    for (auto it = constV.begin(); it != constV.end(); ++it)
    {
        constSum += *it;
    }
    EXPECT_TO_BE(104, constSum);

    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// 3. Equality & Arithmetic Operators
// -----------------------------------------------------------------------------

u8 test_equality_operators()
{
    Vector<f32, 3> v1({1.0f, 2.0f, 3.0f});
    Vector<f32, 3> v2({1.0f, 2.0f, 3.0f});
    Vector<f32, 3> v3({1.0f, 2.0f, 4.0f});

    EXPECT_TO_BE_TRUE(v1 == v2);
    EXPECT_TO_BE_FALSE(v1 == v3);
    return PASS_TEST;
}

u8 test_addition()
{
    Vector<f32, 3> a({1.0f, 2.0f, 3.0f});
    Vector<f32, 3> b({4.0f, 5.0f, 6.0f});

    Vector<f32, 3> res = a + b;
    EXPECT_FLOAT_TO_BE(5.0f, res[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(7.0f, res[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(9.0f, res[2], 1e-5f);

    a += b;
    EXPECT_TO_BE_TRUE(a == res);
    return PASS_TEST;
}

u8 test_subtraction()
{
    Vector<f32, 3> a({5.0f, 7.0f, 9.0f});
    Vector<f32, 3> b({4.0f, 5.0f, 6.0f});

    Vector<f32, 3> res = a - b;
    EXPECT_FLOAT_TO_BE(1.0f, res[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(2.0f, res[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(3.0f, res[2], 1e-5f);

    a -= b;
    EXPECT_TO_BE_TRUE(a == res);
    return PASS_TEST;
}

u8 test_dot_product()
{
    Vector<f32, 3> a({1.0f, 3.0f, -5.0f});
    Vector<f32, 3> b({4.0f, -2.0f, -1.0f});

    // 1*4 + 3*(-2) + (-5)*(-1) = 4 - 6 + 5 = 3
    f32 dot = a * b;
    EXPECT_FLOAT_TO_BE(3.0f, dot, 1e-5f);
    return PASS_TEST;
}

u8 test_scalar_multiplication()
{
    Vector<f32, 3> a({1.0f, -2.0f, 3.0f});
    f32 scalar = 3.0f;

    Vector<f32, 3> res = a * scalar;
    EXPECT_FLOAT_TO_BE(3.0f, res[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(-6.0f, res[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(9.0f, res[2], 1e-5f);

    a *= scalar;
    EXPECT_TO_BE_TRUE(a == res);
    return PASS_TEST;
}

u8 test_scalar_division()
{
    Vector<f32, 3> a({6.0f, -12.0f, 18.0f});
    f32 scalar = 3.0f;

    Vector<f32, 3> res = a / scalar;
    EXPECT_FLOAT_TO_BE(2.0f, res[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(-4.0f, res[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(6.0f, res[2], 1e-5f);

    a /= scalar;
    EXPECT_TO_BE_TRUE(a == res);
    return PASS_TEST;
}

u8 test_cross_product()
{
    Vector<f32, 3> x({1.0f, 0.0f, 0.0f});
    Vector<f32, 3> y({0.0f, 1.0f, 0.0f});

    // i x j = k
    Vector<f32, 3> z = x.cross(y);
    EXPECT_FLOAT_TO_BE(0.0f, z[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(0.0f, z[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(1.0f, z[2], 1e-5f);

    // Arbitrary 3D cross product test
    Vector<f32, 3> a({2.0f, 3.0f, 4.0f});
    Vector<f32, 3> b({5.0f, 6.0f, 7.0f});
    // Cross product: (3*7 - 4*6, 4*5 - 2*7, 2*6 - 3*5) = (-3, 6, -3)
    Vector<f32, 3> crossRes = a.cross(b);
    EXPECT_FLOAT_TO_BE(-3.0f, crossRes[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(6.0f, crossRes[1], 1e-5f);
    EXPECT_FLOAT_TO_BE(-3.0f, crossRes[2], 1e-5f);

    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// 4. Magnitudes, Distance, Normalization & Angles
// -----------------------------------------------------------------------------

u8 test_magnitude_and_distance()
{
    Vector<f64, 3> v({3.0, 4.0, 0.0});

    EXPECT_FLOAT_TO_BE(25.0, v.magnitude2(), 1e-5);
    EXPECT_FLOAT_TO_BE(5.0, v.magnitude(), 1e-5);

    Vector<f64, 3> u({0.0, 0.0, 0.0});
    EXPECT_FLOAT_TO_BE(25.0, v.distance2(u), 1e-5);
    EXPECT_FLOAT_TO_BE(5.0, v.distance(u), 1e-5);

    return PASS_TEST;
}

u8 test_normalization()
{
    Vector<f64, 3> v({0.0, 3.0, 4.0});

    // Test normalizedCopy
    Vector<f64, 3> normCopy = v.normalizedCopy();
    EXPECT_FLOAT_TO_BE(1.0, normCopy.magnitude(), 1e-5);
    EXPECT_FLOAT_TO_BE(0.0, normCopy[0], 1e-5);
    EXPECT_FLOAT_TO_BE(0.6, normCopy[1], 1e-5);
    EXPECT_FLOAT_TO_BE(0.8, normCopy[2], 1e-5);

    // Test in-place normalize
    v.normalize();
    EXPECT_FLOAT_TO_BE(1.0, v.magnitude(), 1e-5);
    EXPECT_TO_BE_TRUE(v == normCopy);

    return PASS_TEST;
}

u8 test_angles()
{
    Vector<f64, 2> a({1.0, 0.0});
    Vector<f64, 2> b({0.0, 1.0});

    // 90 degrees perpendicular test
    f64 rad = a.angleRadians(b);
    f64 deg = a.angleDegrees(b);

    EXPECT_FLOAT_TO_BE(std::numbers::pi / 2.0, rad, 1e-5);
    EXPECT_FLOAT_TO_BE(90.0, deg, 1e-5);

    // Parallel vector test (0 degrees)
    Vector<f64, 2> c({2.0, 0.0});
    EXPECT_FLOAT_TO_BE(0.0, a.angleDegrees(c), 1e-5);

    return PASS_TEST;
}

u8 test_lerp()
{
    Vector<f64, 3> start({0.0, 10.0, 20.0});
    Vector<f64, 3> end({10.0, 20.0, 30.0});

    // t = 0 -> start
    Vector<f64, 3> res0 = start.lerp(end, 0.0);
    EXPECT_TO_BE_TRUE(res0 == start);

    // t = 1 -> end
    Vector<f64, 3> res1 = start.lerp(end, 1.0);
    EXPECT_TO_BE_TRUE(res1 == end);

    // t = 0.5 -> midpoint
    Vector<f64, 3> mid = start.lerp(end, 0.5);
    EXPECT_FLOAT_TO_BE(5.0,  mid[0], 1e-5);
    EXPECT_FLOAT_TO_BE(15.0, mid[1], 1e-5);
    EXPECT_FLOAT_TO_BE(25.0, mid[2], 1e-5);

    // Arbitrary interpolation
    Vector<f64, 3> quarter = start.lerp(end, 0.25);
    EXPECT_FLOAT_TO_BE(2.5,  quarter[0], 1e-5);
    EXPECT_FLOAT_TO_BE(12.5, quarter[1], 1e-5);
    EXPECT_FLOAT_TO_BE(22.5, quarter[2], 1e-5);

    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// Main Execution
// -----------------------------------------------------------------------------

int main()
{
    // Constructors
    registerTest(test_default_constructor, "Default Constructor", 0);
    registerTest(test_initializer_list_constructor, "Initializer List Constructor", 0);
    registerTest(test_copy_constructor, "Copy Constructor", 0);
    registerTest(test_fill_value_constructor, "Fill Value Constructor", 0);

    // Accessors & Iterators
    registerTest(test_subscript_operator, "Subscript Operator Read/Write", 1);
    registerTest(test_iterators, "Range-based and Const Iterators", 1);

    // Arithmetic
    registerTest(test_equality_operators, "Vector Equality Operator", 2);
    registerTest(test_addition, "Vector Addition (+ and +=)", 2);
    registerTest(test_subtraction, "Vector Subtraction (- and -=)", 2);
    registerTest(test_dot_product, "Vector Dot Product (*)", 2);
    registerTest(test_scalar_multiplication, "Scalar Multiplication (* and *=)", 2);
    registerTest(test_scalar_division, "Scalar Division (/ and /=)", 2);
    registerTest(test_cross_product, "3D Vector Cross Product", 2);

    // Math Functions
    registerTest(test_magnitude_and_distance, "Magnitude and Distance Calculations", 3);
    registerTest(test_normalization, "Vector Normalization", 3);
    registerTest(test_angles, "Vector Angle Calculations (Rad/Deg)", 3);
    registerTest(test_lerp, "Vector linear interpolation (Lerp)", 3);

    // Execute all registered tests
    runTests();

    return 0;
}
