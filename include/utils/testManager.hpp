/**
  * @file testManager.hpp
  * @brief Lightweight test harness for registering and running unit tests.
  *
  * Provides a minimal testing framework where tests are registered and
  * executed sequentially. Tests return a single byte indicating pass,
  * fail, or skip status.
  *
  * @note Adapted from Forge Library:
  * https://github.com/Asher-Ul-Haque/ForgeLibrary
*/

#pragma once

#include <defines.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <utils/dataTypes.hpp>
#include <utils/logger.hpp>
namespace utils
{
  /// @brief Enables compilation of test-related functionality.
  #define TESTING

  /**
    * @brief Function pointer type used for test functions.
    *
    * A test function must return a `u8` representing its result.
  */
  typedef u8 (*TEST)();

  /**
    * @brief Represents a single test entry.
    *
    * Stores the test function and a short description of what the
    * test validates.
  */
  typedef struct TestEntry
  {
    TEST func;                     ///< Test function.
    const std::string description; ///< Description of the test.
  } TestEntry;

  /// @brief Group of tests executed sequentially.
  typedef std::vector<TestEntry> TestGroup;


  // - - - Test registration API - - - 

  /**
    * @brief Registers a test for execution.
    *
    * @param FUNCTION Test function to execute.
    * @param DESCRIPTION Short description of the test.
    * @param GROUP_ID Identifier of the test group. Tests in the same group
    *        are executed sequentially.
  */
  void registerTest(
    TEST               FUNCTION,
    const std::string& DESCRIPTION,
    u8                 GROUP_ID = 0);

  /// @brief Executes all registered tests.
  void runTests();


  // - - - Expectation macros for tests - - - 

  /// @brief Verifies two values are equal.
  #define EXPECT_TO_BE(EXPECTED, ACTUAL)                                  \
    if ((EXPECTED) != (ACTUAL))                                           \
    {                                                                     \
      LOG_ERROR("EXPECT_TO_BE failed: " #EXPECTED " != " #ACTUAL);        \
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies two values are not equal.
  #define EXPECT_NOT_TO_BE(NOT_EXPECTED, ACTUAL)                          \
    if ((NOT_EXPECTED) == (ACTUAL))                                       \
    {                                                                     \
      LOG_ERROR("EXPECT_NOT_TO_BE failed: " #NOT_EXPECTED " == " #ACTUAL);\
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies two std::string values are equal.
  #define EXPECT_STRING_TO_BE(EXPECTED, ACTUAL)                           \
    if ((EXPECTED) != (ACTUAL))                                           \
    {                                                                     \
      LOG_ERROR("EXPECT_STRING_TO_BE failed : " #EXPECTED " != " #ACTUAL);\
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies two C-style strings are equal.
  #define EXPECT_C_STRING_TO_BE(EXPECTED, ACTUAL)                           \
    if (std::strcmp((EXPECTED), (ACTUAL)) != 0)                             \
    {                                                                       \
      LOG_ERROR("EXPECT_C_STRING_TO_BE failed : " #EXPECTED " != " #ACTUAL);\
      return FAIL_TEST;                                                     \
    }

  /**
    * @brief Verifies two floating point values are approximately equal.
    *
    * @param EXPECTED Expected value.
    * @param ACTUAL Actual value.
    * @param EPS Allowed tolerance.
  */
  #define EXPECT_FLOAT_TO_BE(EXPECTED, ACTUAL, EPS)                       \
    if (std::fabs((EXPECTED) - (ACTUAL)) > (EPS))                         \
    {                                                                     \
      LOG_ERROR("EXPECT_FLOAT_TO_BE failed :" #EXPECTED " != " #ACTUAL);  \
      return FAIL_TEST;                                                   \
    }

  ///@brief Verifies a pointer is null.
  #define EXPECT_TO_BE_NULL(PTR)                                          \
    if ((PTR) != nullptr)                                                 \
    {                                                                     \
      LOG_ERROR("EXPECT_TO_BE_NULL failed : " #PTR);                      \
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies a pointer is not null.
  #define EXPECT_TO_BE_NOT_NULL(PTR)                                      \
    if ((PTR) == nullptr)                                                 \
    {                                                                     \
      LOG_ERROR("EXPECT_TO_BE_NOT_NULL failed : " #PTR);                  \
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies an expression evaluates to true.
  #define EXPECT_TO_BE_TRUE(EXPR)                                         \
    if (!(EXPR))                                                          \
    {                                                                     \
      LOG_ERROR("EXPECT_TO_BE_TRUE failed : " #EXPR);                     \
      return FAIL_TEST;                                                   \
    }

  /// @brief Verifies an expression evaluates to false.
  #define EXPECT_TO_BE_FALSE(EXPR)                                        \
    if ((EXPR))                                                           \
    {                                                                     \
      LOG_ERROR("EXPECT_TO_BE_FALSE failed : " #EXPR);                    \
      return FAIL_TEST;                                                   \
    }


  // - --  Test result codes - - -

  /// @brief Test failed.
  #define FAIL_TEST 0

  /// @brief Test passed.
  #define PASS_TEST 1

  /// @brief Test skipped.
  #define SKIP_TEST 2
};
