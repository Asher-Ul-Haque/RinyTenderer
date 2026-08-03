/**
 * @file vector.hpp
 * @brief provides vector classes for math 
 *
 * Uses a template to create vector classes at compile time as required
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
#include <vector>
using namespace utils;

namespace math 
{
  /**
  * @brief: A Math vector implementation in C++ 
  * @param typename TYPE: the dataype of the vector 
  * @param u8 SIZE: the size of the vector
  * @warning: since the size is a u8, no vector can be bigger than 255 elements
  * @param arrat data: the internal array storing the vector
  */
  template<typename TYPE, std::size_t SIZE>
  class Vector 
  {
    COMPILE_TIME_ASSERT(SIZE <= 255 && SIZE > 0, "[VECTOR] : Size must be in [0, 255]");

  private:
    std::array<TYPE, SIZE> data;

  public:

    // - - - Constructors - - - 

    /// @brief default constructor 
    constexpr Vector(void) = default;

    /**
    * @brief Constructor like so Vector<u8, 3> v(some_vector)
    * @param INIT_LIST std::vector<TYPE> a std::vector of the same type
    * @warning INIT_LIST must be the same size as SIZE
    * @return a Vector object
    */
    constexpr Vector(std::vector<TYPE> INIT_LIST)
    {
      RUNTIME_ASSERT_DEBUG(INIT_LIST.size() == SIZE, "[Vector] : Initializer list must contain exactly SIZE elements.");
      std::copy(
        INIT_LIST.begin(), 
        INIT_LIST.end(), 
        data.begin());
    }

    /**
    * @brief Constructor like so Vector<u8, 3> v({1, 2, 3})
    * @param INIT_LIST std::initializer_list<TYPE> the initializer list 
    * @warning INIT_LIST must be the same size as SIZE
    * @return a Vector object
    */
    constexpr explicit Vector(std::initializer_list<TYPE> INIT_LIST)
    {
      RUNTIME_ASSERT_DEBUG(INIT_LIST.size() == SIZE, "[Vector] : Initializer list must contain exactly SIZE elements.");
      std::copy(
        INIT_LIST.begin(), 
        INIT_LIST.end(), 
        data.begin());
    }

/**
    * @brief Copy constructor 
    * @param OTHER: another vector 
    * @return a Vector object
    */
    constexpr Vector(const Vector& OTHER) = default;

    /**
    * @brief Copy assignment operator
    * @param OTHER: another vector
    * @return Reference to this vector
    */
    constexpr Vector& operator=(const Vector& OTHER) = default;

    /**
    * @brief Initializes the entire vector with this value 
    * @param INIT_VALUE TYPE : The value to be filled in the vector
    * @retrn a Vector object
    */
    constexpr explicit Vector(TYPE INIT_VALUE)
    { data.fill(INIT_VALUE); }


    // - - - Element Access - - -  

    /**
    * @brief: Provides access to an element at the given index 
    * @param INDEX u8 : The index 
    * @return TYPE : the value at the index
    * @warning: allows writing
    */
    constexpr TYPE& operator[](std::size_t INDEX)
    {
      RUNTIME_ASSERT_DEBUG(INDEX < SIZE, "[Vector] : Index out of bounds error, greater");
      return data[INDEX];
    }

    /**
    * @brief: Provides access to an element at the given index 
    * @param INDEX u8 : The index 
    * @warning: allows writing
    * @return TYPE: the value at the index 
    */
    constexpr const TYPE& operator[](std::size_t INDEX) const 
    {
      RUNTIME_ASSERT_DEBUG(INDEX < SIZE, "[Vector] : Index out of bounds error, greater");
      return data[INDEX];
    }


    // - - - Arithmetic - - - 
    
    bool operator== (const Vector& OTHER) const
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      {
        if (data[i] != OTHER[i]) return false;
      }
      return true;
    }

    /**
    * @brief: Creates another vector as the sum of this and a given vector 
    * @param: OTHER another vector 
    * @return Vector: the sum of this and the OTHER vector
    */
    Vector operator+ (const Vector& OTHER) const 
    {
      Vector<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      { result[i] = data[i] + OTHER[i]; }
      return result;
    }

    /**
    * @brief: Creates another vector as the diff of this and a given vector 
    * @param: OTHER another vector 
    * @return Vector: the diff of this and the OTHER vector
    */
    Vector operator- (const Vector& OTHER) const
    {
      Vector<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      { result[i] = data[i] - OTHER[i]; }
      return result;
    }

    /**
    * @brief: Dot product
    * @param: OTHER another vector 
    * @return TYPE: doct product 
    */
    TYPE operator* (const Vector& OTHER) const 
    {
      TYPE answer = 0;
      for (std::size_t i = 0; i < SIZE; ++i)
      { answer += data[i] * OTHER[i]; }
      return answer;
    }

    /**
    * @brief: Creates another vector as the product of this and a given vector 
    * @param: OTHER another vector 
    * @return Vector: the product of this and the OTHER vector
    */
    Vector operator* (const TYPE& SCALAR) const 
    {
      Vector<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      { result[i] = data[i] * SCALAR; }
      return result;
    }

    /**
    * @brief: Creates another vector as the div of this and a given vector 
    * @param: OTHER another vector 
    * @return Vector: the div of this and the OTHER vector
    */
    Vector operator/ (const TYPE& SCALAR) const
    {
      RUNTIME_ASSERT_DEBUG(SCALAR != 0, "[Vector] : Divide by zero error");

      Vector<TYPE, SIZE> result;
      for (std::size_t i = 0; i < SIZE; ++i)
      { result[i] = data[i] / SCALAR; }
      return result;
    }

    /**
    * @brief: Cross product
    * @param: OTHER another vector 
    * @return Vector: the cross product 
    * @warning: The vectors must be 3 long 
    */
    Vector cross (const Vector& OTHER) const 
    {
      COMPILE_TIME_ASSERT(SIZE == 3);

      Vector<TYPE, 3> ans;
      ans[0] = (data[1] * OTHER[2]) - (data[2] * OTHER[1]);
      ans[1] = (data[2] * OTHER[0]) - (data[0] * OTHER[2]);
      ans[2] = (data[0] * OTHER[1]) - (data[1] * OTHER[0]);
      return ans;
    }

    /**
    * @brief: Adds another vector to this one
    * @param: OTHER another vector 
    */
    Vector& operator+= (const Vector& OTHER)
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      { data[i] += OTHER[i]; }
      return *this;
    }

    /**
    * @brief: Subtracts another vector to this one
    * @param: OTHER another vector 
    */
    Vector& operator-= (const Vector& OTHER)
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      { data[i] -= OTHER[i]; }
      return *this;
    }

    /**
    * @brief: Multiplies another vector to this one
    * @param: OTHER another vector 
    */
    Vector& operator*= (const TYPE& SCALAR)
    {
      for (std::size_t i = 0; i < SIZE; ++i)
      { data[i] *= SCALAR; }
      return *this;
    }

    /**
    * @brief: Divides another vector to this one
    * @param: OTHER another vector 
    */
    Vector& operator/= (const TYPE& SCALAR) 
    {
      RUNTIME_ASSERT_DEBUG(SCALAR != 0, "[Vector] : Divide by zero error");

      for (std::size_t i = 0; i < SIZE; ++i)
      { data[i] /= SCALAR; }

      return *this;
    }


    // - - - Sizes - - - 

    /**
    * @brief returns the size of the vector 
    * @return the size of the vector
    */
    constexpr u8 size(void) const 
    { return SIZE; }

    /**
    * @brief returns the magnitude of the vector squared 
    * @return TYPE: The magnitude of this vector squared
    */
    TYPE magnitude2 (void) const
    {
      TYPE magnitude2 = 0;
      for (std::size_t i = 0; i < SIZE; ++i)
      { magnitude2 += (data[i] * data[i]); }
      return magnitude2;
    }

    /**
    * @brief returns the magnitude of the vector
    * @return TYPE: The magnitude of this vector
    * @warning: sqrt is a costly operation: try using magnitude2 if possible
    * @see: magnitude2
    */
    f64 magnitude (void) const
    {
      PROFILE
      return std::sqrt(magnitude2()); 
    }


    // - - - Normalization - - - 

    /// @brief Normalizes this vector
    void normalize(void)
    { *this /= magnitude(); }

    /**
    * @brief Returns a normalized copy of this vector 
    * @return Vector: a normalized copy
    */
    Vector normalizedCopy(void) const
    {
      Vector<TYPE, SIZE> copy(*this);
      copy /= magnitude();
      return copy;
    }


    // - - - Distance - - - 

    /**
    * @brief Calculates the distance squared between this and another vector
    * @return f64: the distance squared as a double
    */
    f64 distance2(const Vector& OTHER) const
    {
      f64 dist2 = 0;
      for (size_t i = 0; i < SIZE; ++i)
      {
        dist2 += ((OTHER[i] - data[i]) * (OTHER[i] - data[i]));
      }
      return dist2;
    }

    /**
    * @brief: Calculates the distance between between this and another vector 
    * @return f64: the distance as a double
    * @warning: sqrt is a costly operation, try to use distance2 if possible 
    * @see distance2
    */
    f64 distance(const Vector& OTHER) const
    {
      PROFILE
      return std::sqrt(distance2(OTHER));
    }


    // - - - Angles - - - 

    /**
    * @brief: Returns the angle between two vectors in radians 
    * @param: Vector OTHER: the other vector 
    * @warning: the angle is in radians 
    * @return: f64 the angle
    */
    f64 angleRadians(const Vector& OTHER) const
    {
      PROFILE

      f64 dot        = *this * OTHER;
      f64 lengthMine = magnitude();
      f64 lenthOther = OTHER.magnitude();

      return std::acos(dot / (lengthMine * lenthOther));
    }

    /**
    * @brief: Returns the angle between two vectors in degrees 
    * @param: Vector OTHER: the other vector 
    * @warning: the angle is in degrees 
    * @return f64: the angle
    */
    f64 angleDegrees(const Vector& OTHER) const
    {
      return (f64(180) / f64(std::numbers::pi)) * angleRadians(OTHER);
    }

    // - - - Lerp 

    /**
    * @brief: Returns a lerped vector between this and another vector 
    * @param OTHER: Vector another vector 
    * @param LERP_FACTOR: f64: a double representing a lerp factor 
    * @warning: LERP_FACTOR must be within [0.0f, 1.0f] lest an asser triggers
    */
    Vector lerp(const Vector& OTHER, f64 LERP_FACTOR) const
    {
      RUNTIME_ASSERT_DEBUG(
        LERP_FACTOR >= 0.0f && LERP_FACTOR <= 1.0f, "[VECTOR]: LERP_FACTOR Needs to be within 0 and 1 (inclusive)");
      Vector<TYPE, SIZE> solution(0);
      for (size_t i = 0; i < SIZE; ++i)
      {
        solution[i] = (((1 - LERP_FACTOR) * data[i]) + ((LERP_FACTOR) * OTHER[i]));
      }
      return solution;
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
  };
};
