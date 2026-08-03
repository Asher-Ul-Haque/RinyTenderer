/**
 * @file test_squareMatrix.cpp
 * @brief Comprehensive unit tests for squareMatrix.hpp using testManager.hpp
 */

#include <math/matrix.hpp>
#include <math/vector.hpp>
#include <utils/testManager.hpp>
#include <iostream>
#include <cmath>

using namespace utils;
using namespace math;

// -----------------------------------------------------------------------------
// 1. Constructors & Accessors
// -----------------------------------------------------------------------------

u8 test_default_constructor()
{
    // Default constructor should build an Identity Matrix
    SquareMatrix<f32, 3> m;
    
    EXPECT_TO_BE(3, m.size());
    
    // Check diagonal elements are 1 and off-diagonals are 0
    for (std::size_t i = 0; i < 3; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            f32 expected = (i == j) ? 1.0f : 0.0f;
            EXPECT_FLOAT_TO_BE(expected, m[i][j], 1e-5f);
        }
    }
    return PASS_TEST;
}

u8 test_fill_constructor()
{
    SquareMatrix<i32, 3> m(7);
    for (std::size_t i = 0; i < 3; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            EXPECT_TO_BE(7, m[i][j]);
        }
    }
    return PASS_TEST;
}

u8 test_initializer_list_constructor()
{
    Vector<f32, 2> row0({1.0f, 2.0f});
    Vector<f32, 2> row1({3.0f, 4.0f});
    
    SquareMatrix<f32, 2> m({row0, row1});
    
    EXPECT_FLOAT_TO_BE(1.0f, m[0][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(2.0f, m[0][1], 1e-5f);
    EXPECT_FLOAT_TO_BE(3.0f, m[1][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(4.0f, m[1][1], 1e-5f);
    
    return PASS_TEST;
}

u8 test_subscript_and_iterators()
{
    SquareMatrix<f64, 2> m(0.0);
    m[0][0] = 1.1;
    m[0][1] = 2.2;
    m[1][0] = 3.3;
    m[1][1] = 4.4;

    const auto& constM = m;
    EXPECT_FLOAT_TO_BE(2.2, constM[0][1], 1e-5);

    // Iterators test
    f64 sum = 0.0;
    for (const auto& row : constM)
    {
        for (std::size_t j = 0; j < constM.size(); ++j)
        {
            sum += row[j];
        }
    }
    EXPECT_FLOAT_TO_BE(11.0, sum, 1e-5);

    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// 2. Arithmetic & Matrix Multiplication
// -----------------------------------------------------------------------------

u8 test_addition_and_subtraction()
{
    SquareMatrix<f32, 2> a({Vector<f32, 2>({1.0f, 2.0f}), Vector<f32, 2>({3.0f, 4.0f})});
    SquareMatrix<f32, 2> b({Vector<f32, 2>({5.0f, 6.0f}), Vector<f32, 2>({7.0f, 8.0f})});

    SquareMatrix<f32, 2> sum = a + b;
    EXPECT_FLOAT_TO_BE(6.0f, sum[0][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(8.0f, sum[0][1], 1e-5f);
    EXPECT_FLOAT_TO_BE(10.0f, sum[1][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(12.0f, sum[1][1], 1e-5f);

    a += b;
    EXPECT_TO_BE_TRUE(a == sum);

    SquareMatrix<f32, 2> diff = a - b;
    EXPECT_FLOAT_TO_BE(1.0f, diff[0][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(2.0f, diff[0][1], 1e-5f);

    return PASS_TEST;
}

u8 test_scalar_operations()
{
    SquareMatrix<f32, 2> m({Vector<f32, 2>({2.0f, 4.0f}), Vector<f32, 2>({6.0f, 8.0f})});
    
    SquareMatrix<f32, 2> scaled = m * 2.0f;
    EXPECT_FLOAT_TO_BE(4.0f, scaled[0][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(16.0f, scaled[1][1], 1e-5f);

    SquareMatrix<f32, 2> divided = scaled / 2.0f;
    EXPECT_TO_BE_TRUE(divided == m);

    return PASS_TEST;
}

u8 test_matrix_multiplication_2x2()
{
    SquareMatrix<f32, 2> a({Vector<f32, 2>({1.0f, 2.0f}), Vector<f32, 2>({3.0f, 4.0f})});
    SquareMatrix<f32, 2> b({Vector<f32, 2>({2.0f, 0.0f}), Vector<f32, 2>({1.0f, 2.0f})});

    // [1 2] * [2 0] = [4 4]
    // [3 4]   [1 2]   [10 8]
    SquareMatrix<f32, 2> res = a * b;

    EXPECT_FLOAT_TO_BE(4.0f, res[0][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(4.0f, res[0][1], 1e-5f);
    EXPECT_FLOAT_TO_BE(10.0f, res[1][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(8.0f, res[1][1], 1e-5f);

    return PASS_TEST;
}

u8 test_matrix_multiplication_3x3()
{
    SquareMatrix<f32, 3> a({Vector<f32, 3>({1.0f, 2.0f, 3.0f}),
                            Vector<f32, 3>({4.0f, 5.0f, 6.0f}),
                            Vector<f32, 3>({7.0f, 8.0f, 9.0f})});
    
    SquareMatrix<f32, 3> identity; // Identity matrix
    
    SquareMatrix<f32, 3> res = a * identity;
    EXPECT_TO_BE_TRUE(a == res);

    return PASS_TEST;
}

u8 test_vector_transformation()
{
    // Scaling matrix 2x in X, 3x in Y
    SquareMatrix<f32, 2> scale(0.0f);
    scale[0][0] = 2.0f;
    scale[1][1] = 3.0f;

    Vector<f32, 2> v({5.0f, 10.0f});
    Vector<f32, 2> transformed = scale * v;

    EXPECT_FLOAT_TO_BE(10.0f, transformed[0], 1e-5f);
    EXPECT_FLOAT_TO_BE(30.0f, transformed[1], 1e-5f);

    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// 3. Transpose, Determinants & Inversions
// -----------------------------------------------------------------------------

u8 test_transpose()
{
    SquareMatrix<f32, 3> m({Vector<f32, 3>({1.0f, 2.0f, 3.0f}),
                            Vector<f32, 3>({4.0f, 5.0f, 6.0f}),
                            Vector<f32, 3>({7.0f, 8.0f, 9.0f})});

    SquareMatrix<f32, 3> t = m.transposed();
    EXPECT_FLOAT_TO_BE(2.0f, t[1][0], 1e-5f);
    EXPECT_FLOAT_TO_BE(4.0f, t[0][1], 1e-5f);
    EXPECT_FLOAT_TO_BE(7.0f, t[0][2], 1e-5f);

    m.transpose();
    EXPECT_TO_BE_TRUE(m == t);

    return PASS_TEST;
}

u8 test_determinant_specializations()
{
    // 2x2 Determinant: ad - bc = 1*4 - 2*3 = -2
    SquareMatrix<f64, 2> m2({Vector<f64, 2>({1.0, 2.0}), Vector<f64, 2>({3.0, 4.0})});
    EXPECT_FLOAT_TO_BE(-2.0, m2.determinant(), 1e-5);

    // 3x3 Determinant
    SquareMatrix<f64, 3> m3({Vector<f64, 3>({6.0, 1.0, 1.0}),
                             Vector<f64, 3>({4.0, -2.0, 5.0}),
                             Vector<f64, 3>({2.0, 8.0, 7.0})});
    // Det = 6(-14 - 40) - 1(28 - 10) + 1(32 - (-4)) = 6(-54) - 18 + 36 = -306
    EXPECT_FLOAT_TO_BE(-306.0, m3.determinant(), 1e-5);

    // 4x4 Determinant
    SquareMatrix<f64, 4> m4({Vector<f64, 4>({1.0, 0.0, 2.0, 0.0}),
                             Vector<f64, 4>({0.0, 3.0, 0.0, 4.0}),
                             Vector<f64, 4>({5.0, 0.0, 6.0, 0.0}),
                             Vector<f64, 4>({0.0, 7.0, 0.0, 8.0})});
    // Det = (1*6 - 2*5) * (3*8 - 4*7) = (-4) * (-4) = 16
    EXPECT_FLOAT_TO_BE(16.0, m4.determinant(), 1e-5);

    return PASS_TEST;
}

u8 test_inversion_2x2()
{
    SquareMatrix<f64, 2> m({Vector<f64, 2>({4.0, 7.0}), Vector<f64, 2>({2.0, 6.0})});
    SquareMatrix<f64, 2> inv = m.inverted();

    // M * M^-1 = Identity
    SquareMatrix<f64, 2> identity = m * inv;
    SquareMatrix<f64, 2> expectedIdentity;

    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            EXPECT_FLOAT_TO_BE(expectedIdentity[i][j], identity[i][j], 1e-5);
        }
    }
    return PASS_TEST;
}

u8 test_inversion_3x3()
{
    SquareMatrix<f64, 3> m({Vector<f64, 3>({1.0, 2.0, 3.0}),
                             Vector<f64, 3>({0.0, 1.0, 4.0}),
                             Vector<f64, 3>({5.0, 6.0, 0.0})});

    SquareMatrix<f64, 3> inv = m.inverted();
    SquareMatrix<f64, 3> identity = m * inv;
    SquareMatrix<f64, 3> expectedIdentity;

    for (std::size_t i = 0; i < 3; ++i)
    {
        for (std::size_t j = 0; j < 3; ++j)
        {
            EXPECT_FLOAT_TO_BE(expectedIdentity[i][j], identity[i][j], 1e-5);
        }
    }
    return PASS_TEST;
}

u8 test_inversion_4x4()
{
    SquareMatrix<f64, 4> m({Vector<f64, 4>({2.0, 0.0, 0.0, 0.0}),
                             Vector<f64, 4>({0.0, 1.0, 5.0, 0.0}),
                             Vector<f64, 4>({0.0, 0.0, 1.0, 0.0}),
                             Vector<f64, 4>({0.0, 0.0, 0.0, 4.0})});

    SquareMatrix<f64, 4> inv = m.inverted();
    SquareMatrix<f64, 4> identity = m * inv;
    SquareMatrix<f64, 4> expectedIdentity;

    for (std::size_t i = 0; i < 4; ++i)
    {
        for (std::size_t j = 0; j < 4; ++j)
        {
            EXPECT_FLOAT_TO_BE(expectedIdentity[i][j], identity[i][j], 1e-5);
        }
    }
    return PASS_TEST;
}

// -----------------------------------------------------------------------------
// Main Execution
// -----------------------------------------------------------------------------

int main()
{
    // Constructors & Accessors
    registerTest(test_default_constructor, "Default (Identity) Constructor", 0);
    registerTest(test_fill_constructor, "Fill Value Constructor", 0);
    registerTest(test_initializer_list_constructor, "Initializer List Constructor", 0);
    registerTest(test_subscript_and_iterators, "Subscript Access and Iterators", 1);

    // Arithmetic
    registerTest(test_addition_and_subtraction, "Matrix Addition & Subtraction", 2);
    registerTest(test_scalar_operations, "Scalar Operations (* and /)", 2);
    registerTest(test_matrix_multiplication_2x2, "2x2 Matrix Multiplication", 2);
    registerTest(test_matrix_multiplication_3x3, "3x3 Matrix Multiplication", 2);
    registerTest(test_vector_transformation, "Matrix-Vector Transformation (* Vector)", 2);

    // Transformations, Determinants, Inversions
    registerTest(test_transpose, "Matrix Transposition", 3);
    registerTest(test_determinant_specializations, "2x2, 3x3, and 4x4 Determinants", 3);
    registerTest(test_inversion_2x2, "2x2 Matrix Inversion", 3);
    registerTest(test_inversion_3x3, "3x3 Matrix Inversion", 3);
    registerTest(test_inversion_4x4, "4x4 Matrix Inversion", 3);

    // Execute tests
    runTests();

    return 0;
}
