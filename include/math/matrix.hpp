/**
 * @file matrix.hpp 
 * @brief: provides matrix classes for math 
 *
 * Uses a template to create matrix classes at compile time as requested 
 * for any math operation.
 *
 * Comes with common math operations defined
*/

#pragma once 
#include <utils/profiler.hpp>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <numbers>
#include <utils/dataTypes.hpp>
#include <utils/asserts.hpp>
#include <math/vector.hpp>
#include <vector>
using namespace utils;

namespace math 
{
  /**
   * @brief: A Math Matrix implementation in C++ 
   * @param typename TYPE: the datatype of the matrix 
   * @param u8 SIZE: the size of the matrix
   * @warning: since the size is a u8, no matrix can be bigger than 255 elements 
   * @param array: data the internal array storing the matrix 
  */
  template<typename TYPE, std::size_t SIZE>
  class SquareMatrix
  {
    COMPILE_TIME_ASSERT(SIZE <= 255 && SIZE > 0, "[SQUARE MATRIX] : Size must be in (0, 255]");

  private:
    std::array<Vector<TYPE, SIZE>, SIZE> data;

  public:

    // - - - Constructors - - - 

    /**
     * @brief Standard constructor returning an Identity Matrix
     * @warning: returns an identity matrix 
     * @return: an identity matrix
     */
    constexpr SquareMatrix(void)
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        for (std::size_t j = 0; j < SIZE; ++j)
        {
          data[i][j] = (i == j) ? static_cast<TYPE>(1) : static_cast<TYPE>(0);
        }
      }
    }

    /**
     * @brief Constructs a matrix from an std::vector of math vectors
     * @param std::vector<Vector<TYPE, SIZE>>: VECTORS: std::vector of Math vectors 
     * @warning: the vector must have the same size as SIZE
     * @return A Sqaure Matrix
     */
    constexpr SquareMatrix(const std::vector<Vector<TYPE, SIZE>>& VECTORS)
    {
      RUNTIME_ASSERT_DEBUG(VECTORS.size() == SIZE, "[SQUARE MATRIX] : VECTORS must have size matching matrix dimension");
      for (std::size_t i = 0; i < SIZE; ++i) data[i] = VECTORS[i];
    }

    /**
     * @brief Constructs a matrix from a nested std::vector
     * @param std::vector<std::vector<TYPE>> : a vector of vectors of the type 
     * @warning: all vectors must have the size as SIZE 
     * @warning: there must be SIZE number of vectors 
     * @return: A Square Matrix
    */
    constexpr SquareMatrix(const std::vector<std::vector<TYPE>>& VECTORS)
    {
      RUNTIME_ASSERT_DEBUG(VECTORS.size() == SIZE, "[SQUARE MATRIX] : Outer vector size must match matrix dimension");
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        RUNTIME_ASSERT_DEBUG(VECTORS[i].size() == SIZE, "[SQUARE MATRIX] : Inner vector size must match matrix dimension");
        for (std::size_t j = 0; j < SIZE; ++j)
        {
          data[i][j] = VECTORS[i][j];
        }
      }
    }

    /**
     * @brief Constructs a matrix from an initializer list of Vector
     * @param std::initializer_list<Vector<TYPE, SIZE>>: the vectors 
     * @warning : the list must have the same size as SIZE
     * @return : A Square Matrix
    */
    constexpr SquareMatrix(std::initializer_list<Vector<TYPE, SIZE>> VECTORS)
    {
      RUNTIME_ASSERT_DEBUG(VECTORS.size() == SIZE, "[SQUARE MATRIX] : Initializer list size must match matrix dimension");
      std::size_t i = 0;
      for (const auto& vec : VECTORS)
      {
        data[i++] = vec;
      }
    }

    /// @brief Copy constructor
    constexpr SquareMatrix(const SquareMatrix& OTHER) = default;

    /**
     * @brief Initializes every element of the matrix with INIT_VALUE
     * @param TYPE: INIT_VALUE: The start value 
     * @return: A Sqaure Matrix
    */
    constexpr explicit SquareMatrix(TYPE INIT_VALUE)
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        for (std::size_t j = 0; j < SIZE; ++j)
        {
          data[i][j] = INIT_VALUE;
        }
      }
    }


    // - - - Element Access - - - 

    constexpr Vector<TYPE, SIZE>& operator[] (std::size_t INDEX)
    {
      RUNTIME_ASSERT_DEBUG(INDEX < SIZE, "[SQUARE MATRIX]: Index out of bounds error");
      return data[INDEX];
    }

    constexpr const Vector<TYPE, SIZE>& operator[] (std::size_t INDEX) const
    {
      RUNTIME_ASSERT_DEBUG(INDEX < SIZE, "[SQUARE MATRIX]: Index out of bounds error");
      return data[INDEX];
    }


    // - - - Math Operations - - - 

    /**
     * @brief: adds this matrix to another matrix and returns the result 
     * @param SquareMatrix: OTHER , the other matrix 
     * @return: the sum of these matrices
     */
    constexpr SquareMatrix operator+ (const SquareMatrix& OTHER) const
    {
      SquareMatrix<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        result[i] = data[i] + OTHER[i];
      }
      return result;
    }

    /**
     * @brief: subtracts this matrix by another matrix and returns the result 
     * @param SquareMatrix: OTHER , the other matrix 
     * @return: the diff of these matrices
     */
    constexpr SquareMatrix operator- (const SquareMatrix& OTHER) const
    {
      SquareMatrix<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        result[i] = data[i] - OTHER[i];
      }
      return result;
    }


    /**
     * @brief Unrolled and optimized matrix multiplication for 2x2, 3x3, 4x4
     * @param SquareMatrix: OTHER, the other matrix 
     * @return : the product of these two
     */
    constexpr SquareMatrix operator* (const SquareMatrix& OTHER) const
    {
      SquareMatrix<TYPE, SIZE> result(static_cast<TYPE>(0));

      if constexpr (SIZE == 2)
      {
        result[0][0] = (data[0][0] * OTHER[0][0]) + (data[0][1] * OTHER[1][0]);
        result[0][1] = (data[0][0] * OTHER[0][1]) + (data[0][1] * OTHER[1][1]);
        result[1][0] = (data[1][0] * OTHER[0][0]) + (data[1][1] * OTHER[1][0]);
        result[1][1] = (data[1][0] * OTHER[0][1]) + (data[1][1] * OTHER[1][1]);
      }
      else if constexpr (SIZE == 3)
      {
        for (std::size_t i = 0; i < 3; ++i)
        {
          result[i][0] = (data[i][0] * OTHER[0][0]) + (data[i][1] * OTHER[1][0]) + (data[i][2] * OTHER[2][0]);
          result[i][1] = (data[i][0] * OTHER[0][1]) + (data[i][1] * OTHER[1][1]) + (data[i][2] * OTHER[2][1]);
          result[i][2] = (data[i][0] * OTHER[0][2]) + (data[i][1] * OTHER[1][2]) + (data[i][2] * OTHER[2][2]);
        }
      }
      else if constexpr (SIZE == 4)
      {
        for (std::size_t i = 0; i < 4; ++i)
        {
          result[i][0] = (data[i][0] * OTHER[0][0]) + (data[i][1]*OTHER[1][0] + data[i][2]*OTHER[2][0] + data[i][3] * OTHER[3][0]);
          result[i][1] = (data[i][0] * OTHER[0][1]) + (data[i][1]*OTHER[1][1] + data[i][2]*OTHER[2][1] + data[i][3] * OTHER[3][1]);
          result[i][2] = (data[i][0] * OTHER[0][2]) + (data[i][1]*OTHER[1][2] + data[i][2]*OTHER[2][2] + data[i][3] * OTHER[3][2]);
          result[i][3] = (data[i][0] * OTHER[0][3]) + (data[i][1]*OTHER[1][3] + data[i][2]*OTHER[2][3] + data[i][3] * OTHER[3][3]);
        }
      }
      else
      {
        for (std::size_t i = 0; i < SIZE; ++i) 
        {
          for (std::size_t k = 0; k < SIZE; ++k) 
          {
            TYPE r = data[i][k];
            for (std::size_t j = 0; j < SIZE; ++j) 
            {
              result[i][j] += r * OTHER[k][j];
            }
          }
        }
      }

      return result;
    }

    /**
     * @brief: Transform vector by matrix (Vector-Matrix multiplication)
     * @param Vector<TYPE, SIZE>&: VEC, the vector to be multiplied with 
     * @return: a vector with the product of the two
    */
    constexpr Vector<TYPE, SIZE> operator* (const Vector<TYPE, SIZE>& VEC) const
    {
      Vector<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        result[i] = data[i] * VEC; 
      }
      return result;
    }

    /**
     * @brief: Scales the amtrix with a factor 
     * @param TYPE: FACTOR, the factor to scale the matrix by 
     * @return : A square matrix with the prodct
     */
    constexpr SquareMatrix operator* (TYPE FACTOR) const
    {
      SquareMatrix<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        result[i] = data[i] * FACTOR;
      }
      return result;
    }

    /**
     * @brief: Scales the amtrix with a factor 
     * @param TYPE: FACTOR, the factor to scale the matrix by 
     * @warning : FACTOR cannot be 0
     * @return : A square matrix with the prodct
     */
    constexpr SquareMatrix operator/ (TYPE FACTOR) const
    {
      RUNTIME_ASSERT_DEBUG(FACTOR != static_cast<TYPE>(0), "[SQUARE MATRIX] : Division by zero");
      SquareMatrix<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        result[i] = data[i] / FACTOR;
      }
      return result;
    }

    /**
     * @brief: equality check between this and another matrix 
     * @param SquareMatrix OTHER: another matrix 
     * @return true if equal, else false
     */
    constexpr bool operator== (const SquareMatrix& OTHER) const
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        if (data[i] != OTHER[i]) return false;
      }
      return true;
    }

    /**
     * @brief: In place addition of this matrix with another
     * @param SquareMatrix: OTHER, the other matrix 
    */
    constexpr SquareMatrix& operator+= (const SquareMatrix& OTHER)
    {
      for (std::size_t i = 0; i < SIZE; ++i) data[i] += OTHER[i];
      return *this;
    }
    
    /**
     * @brief: In place subtraction of this matrix with another
     * @param SquareMatrix: OTHER, the other matrix 
    */
    constexpr SquareMatrix& operator-= (const SquareMatrix& OTHER)
    {
      for (std::size_t i = 0; i < SIZE; ++i) data[i] -= OTHER[i];
      return *this;
    }
    
    /**
     * @brief: In place multiplication of this matrix with another
     * @param SquareMatrix: OTHER, the other matrix 
    */
    constexpr SquareMatrix& operator*= (const SquareMatrix& OTHER)
    {
      *this = *this * OTHER;
      return *this;
    }

    /**
     * @brief: In place multiplication of this matrix a scaling factor 
     * @param TYPE: FACTOR, the scaling factor 
    */
    constexpr SquareMatrix& operator*= (TYPE FACTOR)
    {
      for (std::size_t i = 0; i < SIZE; ++i) data[i] *= FACTOR;
      return *this;
    }

    /**
     * @brief: In place diviston of this matrix with a scaling factor 
     * @param TYPE: FACTOR, the sacling factor 
     * @warning: FACTOR must not be 0
    */
    constexpr SquareMatrix& operator/= (TYPE FACTOR)
    {
      RUNTIME_ASSERT_DEBUG(FACTOR != static_cast<TYPE>(0), "[SQUARE MATRIX] : Division by zero");
      for (std::size_t i = 0; i < SIZE; ++i) data[i] /= FACTOR;
      return *this;
    }


    // - - - Special Operations - - - 

    /// @brief: transposes this matrix in place
    constexpr void transpose(void)
    {
      for (std::size_t i = 0; i < SIZE; ++i) 
      {
        for (std::size_t j = i + 1; j < SIZE; ++j) 
        {
          std::swap(data[i][j], data[j][i]);
        }
      }
    }

    /**
     * @brief: Makes a transposed copy of this matrix 
     * @return SquareMatrix: a transposed copy
     */
    constexpr SquareMatrix transposed(void) const
    {
      SquareMatrix<TYPE, SIZE> result(static_cast<TYPE>(0));
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        for (std::size_t j = 0; j < SIZE; ++j)
        {
          result[j][i] = data[i][j];
        }
      }
      return result;
    }

    /**
     * @brief Computes Matrix Determinant using closed analytical unrolling for N=1..4,
     * @warning: no code for size greater than 4 
     * @return f64: the determinant
     */
    f64 determinant(void) const
    {
      PROFILE

      if constexpr (SIZE == 1)
      {
        return static_cast<f64>(data[0][0]);
      }
      else if constexpr (SIZE == 2)
      {
        return static_cast<f64>(data[0][0] * data[1][1] - data[0][1] * data[1][0]);
      }
      else if constexpr (SIZE == 3)
      {
        return static_cast<f64>(
          data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
          data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
          data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0])
        );
      }
      else if constexpr (SIZE == 4)
      {
        f64 s0 = static_cast<f64>(data[0][0] * data[1][1] - data[0][1] * data[1][0]);
        f64 s1 = static_cast<f64>(data[0][0] * data[1][2] - data[0][2] * data[1][0]);
        f64 s2 = static_cast<f64>(data[0][0] * data[1][3] - data[0][3] * data[1][0]);
        f64 s3 = static_cast<f64>(data[0][1] * data[1][2] - data[0][2] * data[1][1]);
        f64 s4 = static_cast<f64>(data[0][1] * data[1][3] - data[0][3] * data[1][1]);
        f64 s5 = static_cast<f64>(data[0][2] * data[1][3] - data[0][3] * data[1][2]);

        f64 c5 = static_cast<f64>(data[2][2] * data[3][3] - data[2][3] * data[3][2]);
        f64 c4 = static_cast<f64>(data[2][1] * data[3][3] - data[2][3] * data[3][1]);
        f64 c3 = static_cast<f64>(data[2][1] * data[3][2] - data[2][2] * data[3][1]);
        f64 c2 = static_cast<f64>(data[2][0] * data[3][3] - data[2][3] * data[3][0]);
        f64 c1 = static_cast<f64>(data[2][0] * data[3][2] - data[2][2] * data[3][0]);
        f64 c0 = static_cast<f64>(data[2][0] * data[3][1] - data[2][1] * data[3][0]);

        return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
      }
      else TODO_COMMENT("Will do if necessary");
    }

    /**
     * @brief Computes inverted matrix (copy) using optimized direct analytic cofactor/adjugate
     * inversion for 2x2, 3x3, and 4x4
     * @warning : no code exists for SIZE greater than 4 
     * @param SquareMatrix: the inverted copy of the matrix
     */
    SquareMatrix inverted(void) const
    {
      PROFILE
      f64 det = determinant();
      RUNTIME_ASSERT_DEBUG(std::abs(det) > 1e-12, "[SQUARE MATRIX] : Matrix is non-invertible (Determinant is 0)");

      SquareMatrix<TYPE, SIZE> result;
      f64 invDet = 1.0 / det;

      if constexpr (SIZE == 2)
      {
        result[0][0] = static_cast<TYPE>( data[1][1] * invDet);
        result[0][1] = static_cast<TYPE>(-data[0][1] * invDet);
        result[1][0] = static_cast<TYPE>(-data[1][0] * invDet);
        result[1][1] = static_cast<TYPE>( data[0][0] * invDet);
      }
      else if constexpr (SIZE == 3)
      {
        result[0][0] = static_cast<TYPE>((data[1][1] * data[2][2] - data[1][2] * data[2][1]) * invDet);
        result[0][1] = static_cast<TYPE>((data[0][2] * data[2][1] - data[0][1] * data[2][2]) * invDet);
        result[0][2] = static_cast<TYPE>((data[0][1] * data[1][2] - data[0][2] * data[1][1]) * invDet);

        result[1][0] = static_cast<TYPE>((data[1][2] * data[2][0] - data[1][0] * data[2][2]) * invDet);
        result[1][1] = static_cast<TYPE>((data[0][0] * data[2][2] - data[0][2] * data[2][0]) * invDet);
        result[1][2] = static_cast<TYPE>((data[0][2] * data[1][0] - data[0][0] * data[1][2]) * invDet);

        result[2][0] = static_cast<TYPE>((data[1][0] * data[2][1] - data[1][1] * data[2][0]) * invDet);
        result[2][1] = static_cast<TYPE>((data[0][1] * data[2][0] - data[0][0] * data[2][1]) * invDet);
        result[2][2] = static_cast<TYPE>((data[0][0] * data[1][1] - data[0][1] * data[1][0]) * invDet);
      }
      else if constexpr (SIZE == 4)
      {
        f64 s0 = static_cast<f64>(data[0][0] * data[1][1] - data[0][1] * data[1][0]);
        f64 s1 = static_cast<f64>(data[0][0] * data[1][2] - data[0][2] * data[1][0]);
        f64 s2 = static_cast<f64>(data[0][0] * data[1][3] - data[0][3] * data[1][0]);
        f64 s3 = static_cast<f64>(data[0][1] * data[1][2] - data[0][2] * data[1][1]);
        f64 s4 = static_cast<f64>(data[0][1] * data[1][3] - data[0][3] * data[1][1]);
        f64 s5 = static_cast<f64>(data[0][2] * data[1][3] - data[0][3] * data[1][2]);

        f64 c5 = static_cast<f64>(data[2][2] * data[3][3] - data[2][3] * data[3][2]);
        f64 c4 = static_cast<f64>(data[2][1] * data[3][3] - data[2][3] * data[3][1]);
        f64 c3 = static_cast<f64>(data[2][1] * data[3][2] - data[2][2] * data[3][1]);
        f64 c2 = static_cast<f64>(data[2][0] * data[3][3] - data[2][3] * data[3][0]);
        f64 c1 = static_cast<f64>(data[2][0] * data[3][2] - data[2][2] * data[3][0]);
        f64 c0 = static_cast<f64>(data[2][0] * data[3][1] - data[2][1] * data[3][0]);

        result[0][0] = static_cast<TYPE>(( data[1][1] * c5 - data[1][2] * c4 + data[1][3] * c3) * invDet);
        result[0][1] = static_cast<TYPE>((-data[0][1] * c5 + data[0][2] * c4 - data[0][3] * c3) * invDet);
        result[0][2] = static_cast<TYPE>(( data[3][1] * s5 - data[3][2] * s4 + data[3][3] * s3) * invDet);
        result[0][3] = static_cast<TYPE>((-data[2][1] * s5 + data[2][2] * s4 - data[2][3] * s3) * invDet);

        result[1][0] = static_cast<TYPE>((-data[1][0] * c5 + data[1][2] * c2 - data[1][3] * c1) * invDet);
        result[1][1] = static_cast<TYPE>(( data[0][0] * c5 - data[0][2] * c2 + data[0][3] * c1) * invDet);
        result[1][2] = static_cast<TYPE>((-data[3][0] * s5 + data[3][2] * s2 - data[3][3] * s1) * invDet);
        result[1][3] = static_cast<TYPE>(( data[2][0] * s5 - data[2][2] * s2 + data[2][3] * s1) * invDet);

        result[2][0] = static_cast<TYPE>(( data[1][0] * c4 - data[1][1] * c2 + data[1][3] * c0) * invDet);
        result[2][1] = static_cast<TYPE>((-data[0][0] * c4 + data[0][1] * c2 - data[0][3] * c0) * invDet);
        result[2][2] = static_cast<TYPE>(( data[3][0] * s4 - data[3][1] * s2 + data[3][3] * s0) * invDet);
        result[2][3] = static_cast<TYPE>((-data[2][0] * s4 + data[2][1] * s2 - data[2][3] * s0) * invDet);

        result[3][0] = static_cast<TYPE>((-data[1][0] * c3 + data[1][1] * c1 - data[1][2] * c0) * invDet);
        result[3][1] = static_cast<TYPE>(( data[0][0] * c3 - data[0][1] * c1 + data[0][2] * c0) * invDet);
        result[3][2] = static_cast<TYPE>((-data[3][0] * s3 + data[3][1] * s1 - data[3][2] * s0) * invDet);
        result[3][3] = static_cast<TYPE>(( data[2][0] * s3 - data[2][1] * s1 + data[2][2] * s0) * invDet);
      }
      else TODO_COMMENT("Will do if necessary")

      return result;
    }

    /// @brief: Invert this matrix
    void invert(void)
    {
      *this = inverted();
    }


    // - - - Iterators - - - 

    /// @brief: Read / Write start iterator 
    constexpr auto begin(void) noexcept       { return data.begin(); }

    /// @brief: Read / Write end iterator 
    constexpr auto end(void) noexcept         { return data.end(); }

    /// @brief: Read only start iterator 
    constexpr auto begin(void) const noexcept { return data.begin(); }

    /// @brief: Read only end iterator 
    constexpr auto end(void) const noexcept   { return data.end(); }

    constexpr u8 size(void) const 
    { 
      return static_cast<u8>(SIZE); 
    }
  };
}
