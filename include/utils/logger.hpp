/**
  * @file logger.hpp
  * @brief Simple colored console logging macros.
  *
  * Provides lightweight logging utilities for console output with
  * optional ANSI color formatting. Logging behavior changes depending
  * on whether `DEBUG` is defined.
  * 
  * Log levels:
  * - FATAL   : Critical error (bright red)
  * - ERROR   : Recoverable error (red)
  * - WARNING : Non-critical warning
  * - INFO    : Informational message
  * - DEBUG   : Debugging output
  * - TRACE   : Verbose tracing information
  *
  * When `DEBUG` is not defined, WARNING/INFO/DEBUG/TRACE logs are disabled.
  *
  * @note Adapted from Forge Library:
  * https://github.com/Asher-Ul-Haque/ForgeLibrary
*/

#pragma once
#include <defines.hpp>
#include <iostream>

namespace utils
{
  /// @brief ANSI color for fatal log messages.
  #define LOG_COLOR_FATAL   "\033[1;31m"

  /// @brief ANSI color for error log messages.
  #define LOG_COLOR_ERROR   "\033[31m"

  /// @brief ANSI escape sequence to reset terminal color.
  #define LOG_COLOR_RESET   "\033[0m"

  #ifdef DEBUG
    /// @brief ANSI color for warning messages.
    #define LOG_COLOR_WARNING "\033[33m"

    /// @brief ANSI color for informational messages.
    #define LOG_COLOR_INFO    "\033[32m"

    /// @brief ANSI color for debug messages.
    #define LOG_COLOR_DEBUG   "\033[36m"

    /// @brief ANSI color for trace messages.
    #define LOG_COLOR_TRACE   "\033[90m"
  #else
    #define LOG_COLOR_WARNING ""
    #define LOG_COLOR_INFO    ""
    #define LOG_COLOR_DEBUG   ""
    #define LOG_COLOR_TRACE   ""
  #endif

  /**
    * @brief Logs a fatal error message.
    *
    * Writes the message to `std::cerr` using fatal formatting.
  */
  #define LOG_FATAL(x) \
    std::cerr << LOG_COLOR_FATAL "[FATAL] " << x << LOG_COLOR_RESET << '\n';

  /**
    * @brief Logs an error message.
    *
    * Writes the message to `std::cerr`.
  */
  #define LOG_ERROR(x) \
    std::cerr << LOG_COLOR_ERROR "[ERROR] " << x << LOG_COLOR_RESET << '\n';

  #ifdef DEBUG
    /// @brief Logs a warning message.
    #define LOG_WARNING(x) \
      std::cerr << LOG_COLOR_WARNING "[WARNING] " << x << LOG_COLOR_RESET << '\n'

    /// @brief Logs an informational message.
    #define LOG_INFO(x) \
      std::cout << LOG_COLOR_INFO "[INFO] " << x << LOG_COLOR_RESET << '\n'

    /// @brief Logs a debug message.
    #define LOG_DEBUG(x) \
      std::cout << LOG_COLOR_DEBUG "[DEBUG] " << x << LOG_COLOR_RESET << '\n'

    /// @brief Logs a trace message.
    #define LOG_TRACE(x) \
      std::cout << LOG_COLOR_TRACE "[TRACE] " << x << LOG_COLOR_RESET << '\n'

  #else
    /// @brief Disabled warning log in non-debug builds.
    #define LOG_WARNING(x)

    /// @brief Disabled info log in non-debug builds.
    #define LOG_INFO(x)

    /// @brief Disabled debug log in non-debug builds.
    #define LOG_DEBUG(x)

    /// @brief Disabled trace log in non-debug builds.
    #define LOG_TRACE(x)

  #endif
}
