/**
  * @file asserts.hpp
  * @brief Assertion and TODO helper macros used throughout the utils module.
  *
  * Provides compile-time assertions, runtime assertion checks, and TODO
  * markers that log an error before terminating the program.
  *
  * @note Adapted from Forge Library:
  * https://github.com/Asher-Ul-Haque/ForgeLibrary
*/
#pragma once
#include <defines.hpp>
#include <utils/logger.hpp>

namespace utils 
{
  // - - - Asserts - - - 

  /**
    * @brief Compile-time assertion macro.
    *
    * Evaluates a condition during compilation and produces a compiler error
    * if the condition is false. Used to enforce invariants that must hold
    * at compile time.
  */
#define COMPILE_TIME_ASSERT static_assert

  /**
    * @brief Expands to a string containing file, line, and function information.
    *
    * Used internally by assertion and TODO macros to report the location
    * where the macro was triggered.
  */
#define WHERE_STR __FILE__ << ":" << __LINE__ << " (" << __func__ << ")"


  // - - - Runtime asserts - - - 

  /**
    * @brief Runtime assertion check.
    *
    * Evaluates the expression at runtime. If the expression evaluates to
    * `false`, a fatal log message is emitted and the program aborts.
    *
    * @param EXPRESSION Boolean expression that must evaluate to true.
  */
#define RUNTIME_ASSERT(EXPRESSION)                                      \
    if (!(EXPRESSION))                                                    \
    {                                                                     \
      LOG_FATAL("ASSERT FAILED: " #EXPRESSION " @ " << WHERE_STR);        \
      std::abort();                                                       \
    }

  /**
    * @brief Runtime assertion check with custom message.
    *
    * Same as `RUNTIME_ASSERT` but allows attaching additional diagnostic
    * information to the log output.
    *
    * @param EXPRESSION Boolean expression that must evaluate to true.
    * @param MESSAGE Custom message describing the failure.
  */
  #define RUNTIME_ASSERT_MESSAGE(EXPRESSION, MESSAGE)                     \
    if (!(EXPRESSION))                                                    \
    {                                                                     \
      LOG_FATAL("ASSERT FAILED: " #EXPRESSION                             \
                << " | " << MESSAGE                                       \
                << " @ " << WHERE_STR);                                   \
      std::abort();                                                       \
    }

  /**
    * @brief Runtime assertion check with custom message in only debug mode
    *
    * Same as `RUNTIME_ASSERT` but allows attaching additional diagnostic
    * information to the log output.
    *
    * @param EXPRESSION Boolean expression that must evaluate to true.
    * @param MESSAGE Custom message describing the failure.
    * @warning Only in debug mode
  */
#ifdef DEBUG
  #define RUNTIME_ASSERT_DEBUG(EXPRESSION, MESSAGE)                       \
    if (!(EXPRESSION))                                                    \
    {                                                                     \
      LOG_FATAL("ASSERT FAILED: " #EXPRESSION                             \
                << " | " << MESSAGE                                       \
                << " @ " << WHERE_STR);                                   \
      std::abort();                                                       \
    }
#else 
  #define RUNTIME_ASSERT_DEBUG(EXPRESSION, MESSAGE)
#endif

  // - - - TODOs - - -

  /**
    * @brief Marks unfinished code.
    *
    * When executed, logs an error indicating that a TODO section was reached
    * and terminates the program.
  */
#define TODO                                                            \
    {                                                                   \
      LOG_ERROR("TODO reached @ " << WHERE_STR);                        \
      std::exit(3);                                                     \
    }


  /**
    * @brief Marks unfinished code with a descriptive message.
    *
    * @param MESSAGE Description of the missing functionality.
  */
#define TODO_COMMENT(MESSAGE)                                           \
    {                                                                   \
      LOG_ERROR("TODO: " << MESSAGE << " @ " << WHERE_STR);             \
      std::exit(3);                                                     \
    }
}
