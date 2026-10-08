/*
 * Copyright (C) [2026] [Asher-Ul-Haque aka Just Somebody]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file : justLibrary.h
 * @brief : Single header file include for the entire library.
 * @note : Makes using the library very simple. Just #include <justLibrary.h> and then use IMPLEMENTATION MACROS to include particular system implementation
 * @warning : Make sure to make all #define IMPLEMENTATION_MACROS before you include this file
*/

/// @brief : If IMPL_ALL is defined, include every system
#if (defined JUST_LIB_IMPL_ALL) || (defined PROMO_CODE)

  #ifndef JUST_LIB_IMPL_MEMORY
    #define JUST_LIB_IMPL_MEMORY
  #endif

  #ifndef JUST_LIB_IMPL_DS
    #define JUST_LIB_IMPL_DS
  #endif

  #ifndef JUST_LIB_IMPL_TESTING
    #define JUST_LIB_IMPL_TESTING
  #endif

  #ifndef JUST_LIB_IMPL_SYSTEMS
    #define JUST_LIB_IMPL_SYSTEMS
  #endif

#endif


// - - - Every system is made of specific things - - - 

/// @brief : Memory system is tracker + linear allocator + object pool
#ifdef JUST_LIB_IMPL_MEMORY

  #ifndef JUST_LIB_IMPL_TRACKER
    #define JUST_LIB_IMPL_TRACKER
  #endif

  #ifndef JUST_LIB_IMPL_LIN_ALLOC
    #define JUST_LIB_IMPL_LIN_ALLOC
  #endif

  #ifndef JUST_LIB_IMPL_OBJ_POOL
    #define JUST_LIB_IMPL_OBJ_POOL
  #endif

#endif


/// @brief : Datastructure system is dynamic array, hashmap, ordered set, queue, ringBuffer, stack, bitSet
#ifdef JUST_LIB_IMPL_DS

  #ifndef JUST_LIB_IMPL_DYN_ARRAY
    #define JUST_LIB_IMPL_DYN_ARRAY
  #endif

  #ifndef JUST_LIB_IMPL_HASHMAP
    #define JUST_LIB_IMPL_HASHMAP
  #endif

  #ifndef JUST_LIB_IMPL_ORDERED_SET
    #define JUST_LIB_IMPL_ORDERED_SET
  #endif

  #ifndef JUST_LIB_IMPL_QUEUE
    #define JUST_LIB_IMPL_QUEUE
  #endif
  
  #ifndef JUST_LIB_IMPL_STACK
    #define JUST_LIB_IMPL_STACK
  #endif

  #ifndef JUST_LIB_IMPL_RING_BUFF
    #define JUST_LIB_IMPL_RING_BUFF
  #endif

  #ifndef JUST_LIB_IMPL_BITSET
    #define JUST_LIB_IMPL_BITSET
  #endif

#endif


/// @brief : Systems system is a file system and a thread pool
#ifdef JUST_LIB_IMPL_SYSTEMS

  #ifndef JUST_LIB_IMPL_FILE_SYS
    #define JUST_LIB_IMPL_FILE_SYS 
  #endif

  #ifndef JUST_LIB_IMPL_THREAD_POOL
    #define JUST_LIB_IMPL_THREAD_POOL
  #endif

#endif


/// @brief : Dependencies

#if defined(JUST_LIB_IMPL_OBJ_POOL) && defined (DEBUG)
  #define JUST_LIB_IMPL_BITSET
#endif

#ifdef JUST_LIB_IMPL_STACK
  #define JUST_LIB_IMPL_DYN_ARRAY
#endif

#ifdef JUST_LIB_IMPL_THREAD_POOL
  #define JUST_LIB_IMPL_QUEUE
#endif


// - - - defines.h
#ifndef JUST_DEFINES
#define JUST_DEFINES
/**
 * @file defines.h 
 * @brief Core platform detection, Clang visibility attributes, and single-header guards.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// - - - Platform Detection - - -

#define JUST_PLATFORM_UNKNOWN 0
#define JUST_PLATFORM_WINDOWS 1
#define JUST_PLATFORM_LINUX   2
#define JUST_PLATFORM_ANDROID 3
#define JUST_PLATFORM_APPLE   4

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #define JUST_PLATFORM JUST_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__gnu_linux__)
  #define JUST_PLATFORM JUST_PLATFORM_LINUX
#elif defined(__ANDROID__)
  #define JUST_PLATFORM JUST_PLATFORM_ANDROID
#elif defined(__APPLE__)
  #define JUST_PLATFORM JUST_PLATFORM_APPLE
#else
  #define JUST_PLATFORM JUST_PLATFORM_UNKNOWN
#endif


// - - - Symbol Visibility & Export Control - - -

#if JUST_PLATFORM == JUST_PLATFORM_WINDOWS
  #if defined(JUST_EXPORT)
    #define JUST_API __declspec(dllexport)
  #else
    #define JUST_API __declspec(dllimport)
  #endif
  #define JUST_LOCAL
#else
  #define JUST_API   __attribute__((visibility("default")))
  #define JUST_LOCAL __attribute__((visibility("hidden")))
#endif

// - - - Cplus ki mkc
#if defined(__cplusplus)
  #define JUST_RESTRICT __restrict
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
  #define JUST_RESTRICT restrict
#else
  #define JUST_RESTRICT
#endif

#ifdef __cplusplus
}
#endif

#endif // JUST_DEFINES


// - - - | Core Utils | - - -

// - - - Logger - - - (Always enabled)

// - - - header
#ifndef JUST_LOGGER

/**
 * @file logger.h 
 * @brief provides logging utilities to any program, have colored logging output, severity based coloring etc
 * 
 * @warning use stdout and not thread safe, if you want logging in a file, pipe stdout to a file 
 * @see logger.c 
*/

#ifdef __cplusplus
extern "C" {
#endif


// - - - | Log macros and defs | - - - 


// - - - Enables - - -

/// @brief The DEBUG macro turns thorough debugging on by default
#ifdef DEBUG

  #ifndef LOG_WARNING_ENABLED 
    #define LOG_WARNING_ENABLED
  #endif

  #ifndef LOG_DEBUG_ENABLED
    #define LOG_DEBUG_ENABLED
  #endif

  #ifndef LOG_TRACE_ENABLED
    #define LOG_TRACE_ENABLED
  #endif

  #ifndef LOG_INFO_ENABLED
    #define LOG_INFO_ENABLED
  #endif

#endif 

#ifndef PRINT_LOG_TYPES
  #define PRINT_LOG_TYPES 1
#endif

#ifndef PRINT_LOG_COLORS
  #define PRINT_LOG_COLORS 1
#endif


// - - - Log Levels - - -

/// @brief Represents log level, internally
typedef enum LogLevel 
{
  LOG_LEVEL_FATAL   =   0,      /// @brief Just give up and die 
  LOG_LEVEL_ERROR   =   1,      /// @brief Something went wrong badly
  LOG_LEVEL_WARNING =   2,      /// @brief Are you sure about that
  LOG_LEVEL_INFO    =   3,      /// @brief Just some information
  LOG_LEVEL_DEBUG   =   4,      /// @brief Debugging information
  LOG_LEVEL_TRACE   =   5       /// @brief Trace every step, be verbose
} LogLevel;


// - - - API Controls - - -

JUST_API void justLogOutput(LogLevel LEVEL, const char* MESSAGE, ...); // - - - Multivariate, takes any number of arguments greater than 1

static inline void justLogNoOp(const char* NOTHING, ...) { (void)NOTHING; }

// - - - Fatal log
#define JUST_LOG_FATAL(...) justLogOutput(LOG_LEVEL_FATAL, __VA_ARGS__, "%s", "");

// - - - Error log
#define JUST_LOG_ERROR(...) justLogOutput(LOG_LEVEL_ERROR, __VA_ARGS__, "%s", "");

// - - - For the rest, define only when enabled, else define to nothingness
#ifdef LOG_WARNING_ENABLED
  #define JUST_LOG_WARNING(...) justLogOutput(LOG_LEVEL_WARNING, __VA_ARGS__, "%s", "");
#else
  #define JUST_LOG_WARNING(...) justLogNoOp(__VA_ARGS__)
#endif

#ifdef LOG_INFO_ENABLED
  #define JUST_LOG_INFO(...) justLogOutput(LOG_LEVEL_INFO, __VA_ARGS__, "%s", "")
#else
  #define JUST_LOG_INFO(...)  justLogNoOp(__VA_ARGS__)
#endif

#ifdef LOG_DEBUG_ENABLED
  #define JUST_LOG_DEBUG(...) justLogOutput(LOG_LEVEL_DEBUG, __VA_ARGS__, "%s", "")
#else
  #define JUST_LOG_DEBUG(...) justLogNoOp(__VA_ARGS__)
#endif

#ifdef LOG_TRACE_ENABLED
  #define JUST_LOG_TRACE(...) justLogOutput(LOG_LEVEL_TRACE, __VA_ARGS__, "%s", "")
#else
  #define JUST_LOG_TRACE(...) justLogNoOp(__VA_ARGS__)
#endif

#define JUST_LOG_CLEAR() printf("\033[H\033[J")


#ifdef __cplusplus
}
#endif

#endif // JUST_LOGGER

// - - - implementation

#if (defined JUST_LIB_IMPL_ALL) || (defined PROMO_CODE)
/**
 * @file logger.c 
 * Implementation of logging utilities for forgelib 
 *
 * @see logger.h for usage 
 * @warning This file is internal implementation
 */

#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>


// - - - Writing Controls - - - 


/**
 * @brief Writes a log to the console 
 * @param MESSAGE what to log 
 * @param COLOR the severity represented by a color 
 */
static void writeConsole(const char* MESSAGE, LogLevel COLOR)
{
  FILE*       stream    = NULL;
  const char* colorStr  = NULL;

  switch (COLOR)
  {
    case LOG_LEVEL_FATAL    : colorStr = "1;97;41"; stream = stderr; break;
    case LOG_LEVEL_ERROR    : colorStr = "1;31";    stream = stderr; break;
    case LOG_LEVEL_WARNING  : colorStr = "33";      stream = stdout; break;
    case LOG_LEVEL_INFO     : colorStr = "32";      stream = stdout; break;
    case LOG_LEVEL_DEBUG    : colorStr = "36";      stream = stdout; break;
    case LOG_LEVEL_TRACE    : colorStr = "90";      stream = stdout; break;
    default                 : colorStr = "0";       stream = stdout; break;
  }

  #if PRINT_LOG_COLORS == 1
    fprintf(stream, "\033[%sm%s\033[0m\n", colorStr, MESSAGE);
  #else 
    (void) colorStr;
    fprintf(stream, "%s\n", MESSAGE); // - - -This looks scary
  #endif
  /*Here is how it works:
  \033[     - This is the escape character
  %s        - This is the color code
  m         - This is the end of the color code
  %s        - This is the message
  \033[0m   - This is the end of the color
  This is how you print colored text in the terminal*/
}


// - - - API Controls - - -


/**
 * @brief constructs a output string and log it
 * @param LEVEL the log severity 
 * @param MESSAGE the main message given by the user 
 * @warning this function is internal to forge, do not use directly 
 * @see FORGE_LOG macros for usage
 */
JUST_API void justLogOutput(LogLevel LEVEL, const char* MESSAGE, ...)
{
  const int   messageLength     = 1024 * 4;
  char        outputMessage     [messageLength];
  memset(outputMessage, 0, sizeof(outputMessage));

  // - - - Add the rest of the arguments
  __builtin_va_list argumentPointer;
  va_start(argumentPointer, MESSAGE);
  vsnprintf(outputMessage, sizeof(outputMessage),
            MESSAGE, argumentPointer);
  va_end(argumentPointer);

  // - - - Prepend with level header
  char finalMessage[messageLength];
  #if PRINT_LOG_TYPES == 1
    const char* levelStrings[6]   = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    sprintf(finalMessage, "%s\t%s", levelStrings[LEVEL], outputMessage);
  #else 
    sprintf(finalMessage, "%s", outputMessage);
  #endif

  writeConsole(finalMessage, LEVEL);    
}
#endif

// - - - Asserts (always defined) - - -

// - - - Header
#ifndef JUST_ASSERTS
#define JUST_ASSERTS

/**
  * @file asserts.hpp
  * @brief Assertion and TODO helper macros used throughout the utils module.
  *
  * Provides compile-time assertions, runtime assertion checks, and TODO
  * markers that log an error before terminating the program.
*/

#include <stdnoreturn.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


// - - - Assert Methods - - -

/**
 * @brief : Internal forge library function to report an assert based on where it happened 
 * @param EXPRESSION : what was the expression being asserted 
 * @param MESSAGE : the message to be logged in case assert failed 
 * @param FILE : which file was the expression in 
 * @param FUNCTION : which function was the expression in, if any 
 * @param LINE : what line was the expression on
 */
JUST_API _Noreturn void justReportAssertionFailure(
  const char* EXPRESSION, 
  const char* MESSAGE, 
  const char* FILE, 
  const char* FUNCTION, 
  size_t      LINE);

/**
 * @brief Internal forge library function to report a TODO 
 * @param COMMENT the message to be logged in case TODO was executed
 * @param FILE which file was the TODO in
 * @param FUNCTION which function was the TODO in, if any 
 * @param LINE what line was the TODO on
 */
JUST_API _Noreturn void reportTODO(
  const char* COMMENT, 
  const char* FILE, 
  const char* FUNCTION, 
  size_t      LINE);

/**
  * @brief Compile-time assertion macro.
  *
  * Evaluates a condition during compilation and produces a compiler error
  * if the condition is false. Used to enforce invariants that must hold
  * at compile time.
  *
  * @see Usage: 
  ```C
    JUST_COMPILE_TIME_ASSERT(sizeof(myStruct) < 8);
  ```
*/
#define JUST_COMPILE_TIME_ASSERT(EXPRESSION)                   _Static_assert(EXPRESSION, "Compile-time assertion failed: " #EXPRESSION)
#define JUST_COMPILE_TIME_ASSERT_MESSAGE(EXPRESSION, MESSAGE)  _Static_assert(EXPRESSION, #MESSAGE)

/**
  * @brief Runtime assertion check.
  *
  * Evaluates the expression at runtime. If the expression evaluates to
  * `false`, a fatal log message is emitted and the program aborts.
  *
  * @param EXPRESSION Boolean expression that must evaluate to true.
  *
  * @see Usage: 
  ```C 
  JUST_ASSERT(value < 32 && myStruct.isValid == true);
  ```
*/
#define JUST_ASSERT(EXPRESSION)                                                              \
  do                                                                                          \
  {                                                                                           \
    if (EXPRESSION){}                                                                         \
    else                                                                                      \
    {                                                                                         \
      justReportAssertionFailure(#EXPRESSION, "Assert Fail\t", __FILE__, __func__, __LINE__); \
    }                                                                                         \
  }                                                                                           \
  while(0)

/**
  * @brief Runtime assertion check with custom message.
  *
  * Same as `RUNTIME_ASSERT` but allows attaching additional diagnostic
  * information to the log output.
  *
  * @param EXPRESSION Boolean expression that must evaluate to true.
  * @param MESSAGE Custom message describing the failure.
  *
  * @see Usage:
  ```C 
  JUST_ASSERT_MESSAGE(value < 32, "Value must be less than 32");
  ```
*/
#define JUST_ASSERT_MESSAGE(EXPRESSION, MESSAGE)                                 \
  do                                                                              \
  {                                                                               \
    if (EXPRESSION){}                                                             \
    else                                                                          \
    {                                                                             \
    justReportAssertionFailure(#EXPRESSION, MESSAGE, __FILE__, __func__, __LINE__);   \
    }                                                                             \
  }                                                                               \
  while (0)

#ifdef DEBUG

/**
 * @brief TODO, haven't written your code? just write TODO 
 * @warning crashes the program on execution in debug mode, 
 * @warning wont allow the program to compile in release mode, fix todos before you make a release
 *
 * @see DEBUG 
 * @see Usage: 
 ```C
 TODO;
 ```
*/
  #define TODO reportTODO(NULL,    __FILE__, __func__, __LINE__);

/**
 * @brief TODO, haven't written your code? just write TODO, but with a comment
 *
 * @param COMMENT, the comment to print if execution reaches TODO
 * @warning crashes the program on execution in debug mode, 
 * @warning wont allow the program to compile in release mode, fix todos before you make a release
 *
 * @see DEBUG 
 * @see Usage: 
 ```C
 TODO_COMMENT("Finish the forge library before going too ther projects";
 ```
*/
  #define TODO_COMMENT(COMMENT) reportTODO(COMMENT, __FILE__, __func__, __LINE__);

/**
 * @brief JUST_ASSERT but in debug mode 
 * @see JUST_ASSERT
*/
  #define JUST_ASSERT_DEBUG(EXPRESSION) JUST_ASSERT(EXPRESSION)

/**
 * @brief JUST_ASSERT_MESSAGE but in debug mode 
 * @see JUST_ASSERT_MESSAGE
*/
  #define JUST_ASSERT_DEBUG_MESSAGE(EXPRESSION, MESSAGE) JUST_ASSERT_MESSAGE(EXPRESSION, MESSAGE)

#else
  #define TODO JUST_COMPILE_TIME_ASSERT_MESSAGE(0, "Unresolved TODO detected in release build");

  #define TODO_COMMENT(COMMENT) JUST_COMPILE_TIME_ASSERT_MESSAGE(0, "Unresolved TODO : " COMMENT);

  #define JUST_ASSERT_DEBUG(EXPRESSION)  \
    do                                    \
    {                                     \
      (void) (EXPRESSION);                \
    }                                     \
    while (0)

  #define JUST_ASSERT_DEBUG_MESSAGE(EXPRESSION, MESSAGE) \
    do                                                    \
    {                                                     \
      (void) (EXPRESSION);                                \
      (void) (MESSAGE);                                   \
    }                                                     \
    while (0)

#endif 


#ifdef __cplusplus
}
#endif

#endif // JUST_ASSERTS


// - - - Implementation
#if (defined JUST_LIB_IMPL_ALL) || (defined PROMO_CODE)

JUST_API _Noreturn void justReportAssertionFailure(const char* EXPRESSION, const char* MESSAGE, const char* FILE, const char* FUNCTION, size_t LINE)
{
  justLogOutput(LOG_LEVEL_FATAL,
            "ASSERT FAIL :     %s\n"
            "MESSAGE     :     %s\n"
            "LOCATION    :     file: %s at line: %zu in function: %s\n", 
            EXPRESSION, MESSAGE, FILE, LINE, FUNCTION);
  abort();
}

JUST_API _Noreturn void reportTODO(const char* COMMENT, const char* FILE, const char* FUNCTION, size_t LINE)
{
  justLogOutput(LOG_LEVEL_ERROR, "Oopsie!, you have a TODO!!!");
  justLogOutput(LOG_LEVEL_ERROR, "At: %s:%zu -> Function: %s", FILE, LINE, FUNCTION);
  if (COMMENT) justLogOutput(LOG_LEVEL_WARNING, "COMMENT: %s", COMMENT);
  abort();
}
#endif

// - - - | Memory | - - -


// - - - Tracker - - -

// - - - Header
#ifndef JUST_TRACKER
#define JUST_TRACKER

/**
 * @file : tracker.h 
 * @brief : Overwrites of malloc, realloc and free to have memory tracking in debug mode, with tagging and allocation limits
*/

#pragma once 

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef JUST_MEMORY_TAG_LIMIT
  #define JUST_MEMORY_TAG_LIMIT 64
#endif

#ifndef JUST_MEMORY_ALLOC_DEFAULT_LIMIT
  #define JUST_MEMORY_ALLOC_DEFAULT_LIMIT 1024ULL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : Allocates memory with surrounding magic canary guards and tracking metadata.
 * @param SIZE : how much to allocate 
 * @param FILE : which file is it allocated in 
 * @param FUNCTION : Which function is it allocated in
 * @param LINE : Which line is it allocated in
 * @param TAG : Why are you allocating
 * @warning : TAG is a char[32]
 */
void* justTrackedMalloc(
  size_t      SIZE,
  const char* FILE,
  const char* FUNC,
  int32_t     LINE,
  const char* TAG);

/**
 * @brief : Reallocates memory, updating canary guards and size tracking.
 * @param PTR : Ptr to realloc 
 * @param NEW_SIZE : new allocation size
 * @param FILE : What file is the reallocate in 
 * @param FUNCTION : What function is the reallocate in 
 * @param LINE : What line is the reallocate in
 * @warning : Make sure pointer is not null, realloc != malloc
*/
JUST_API void* justTrackedRealloc(
  void*       PTR,
  size_t      NEW_SIZE,
  const char* FILE,
  const char* FUNCTION,
  int32_t     LINE);

/**
 * @brief : Allocates zero-initialized memory with canary guards and tracking metadata.
 * @param COUNT : How many objects 
 * @param SIZE : Size of one object 
 * @param FILE : What file is the callocate in 
 * @param FUNCTION : What function is the callocate in 
 * @param LINE : What line is the callocate in
 * @param TAG : Why are you allocating
 * @warning : TAG is a char[32]
 */
JUST_API void* justTrackedCalloc(
  size_t      COUNT,
  size_t      SIZE,
  const char* FILE,
  const char* FUNC,
  int32_t     LINE,
  const char* TAG);

/**
 * @brief : Frees memory and verifies Canary safety bounds.
 * @param PTR : What ptr to free 
 * @param FILE : What file is free called in 
 * @param FUNCTION : What function is the free in 
 * @param LINE : What line is the free in
 */
JUST_API void justTrackedFree(
  void*       PTR,
  const char* FILE,
  const char* FUNCTION,
  int32_t     LINE);

/**
 * @brief : Validates all active allocations against canary corruption.
 * @return : true if all allocations are intact, false if corruption detected.
 */
JUST_API bool justMemoryCheckBounds(void);

/// @brief Reports all active allocations that haven't been freed (Memory Leaks).
JUST_API void justMemoryReportLeaks(void);

/**
 * @brief : Returns total active allocated bytes currently in use.
 * @param TAG : The tag for which you want to check memory, set it to MEMORY_TAG_COUNT to get all
 * @warning : TAG is a char[32]
 * @return : total active allocated bytes in use
 */
JUST_API size_t justMemoryGetActiveBytes(const char* TAG);

/**
 * @brief : Returns the memory allocated as a string,
 * @param VERBOSE : If true, then every information is printed, where did each allocation happen, line, file and function
 * @warning : just for debugging, not for actually parsing memory usage
 * @see : memoryGetActiveBytes for better usage API
*/
JUST_API void justMemoryLogUsage(bool VERBOSE);

/**
 * @brief : Sets a limit on the memory allocation of a particular type
 * @warning : If you exceed the limit, the allocations will return NULL
 * @warning : TAG must be valid
 * @param LIMIT : The limit you want to set in bytes
 * @param TAG : What do you want to set the limit for
 * @warning : TAG is a char[32]
*/
JUST_API void justMemorySetLimit(size_t LIMIT, const char* TAG);

/**
 * @brief : Returns the memory allocation limit
 * @warning : TAG must be valid
*/
JUST_API size_t justMemoryGetLimit(const char* TAG);

// - - - Optional Macro Overrides for Debug Mode
#ifdef DEBUG
  #define JUST_MALLOC_TAGGED(size, tag)        justTrackedMalloc((size), __FILE__, __func__, __LINE__, (tag))
  #define JUST_CALLOC_TAGGED(count, size, tag) justTrackedCalloc((count), (size), __FILE__, __func__, __LINE__, (tag))
  #define JUST_MALLOC(size)                    JUST_MALLOC_TAGGED(size, "UNKNOWN")
  #define JUST_CALLOC(count, size)             JUST_CALLOC_TAGGED(count, size, "UNKNOWN")
  #define JUST_REALLOC(ptr, size)              justTrackedRealloc((ptr), (size), __FILE__, __func__, __LINE__)
  #define JUST_FREE(ptr)                       justTrackedFree((ptr), __FILE__, __func__, __LINE__)
#else
  #define JUST_MALLOC_TAGGED(size, tag)        ((void)(tag), malloc((size)))
  #define JUST_CALLOC_TAGGED(count, size, tag) ((void)(tag), calloc((count), (size)))
  #define JUST_MALLOC(size)                    malloc((size))
  #define JUST_CALLOC(count, size)             calloc((count), (size))
  #define JUST_REALLOC(ptr, size)              realloc((ptr), (size))
  #define JUST_FREE(ptr)                       free((ptr))
#endif

#ifdef __cplusplus
}
#endif
#endif // JUST_TRACKER

// - - - Implementation
#ifdef JUST_LIB_IMPL_TRACKER

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdalign.h>


// - - -  Magic numbers for canary guards
#define JUST_HEADER_MAGIC 0xCAFEBABEU
#define JUST_FOOTER_MAGIC 0xDEADBEEFU
#define JUST_FREED_MAGIC  0xAA00BB11U

/// @brief : Tag Entry, how much per tag
typedef struct justTagEntry
{
  char        name[32];       ///< 32 bit tag
  size_t      allocatedBytes;
  size_t      allocationLimit;
  size_t      activeCount;
} justTagEntry;

/// @brief : Memory tracker header
typedef struct justMemoryHeader
{
  alignas(max_align_t) size_t requestedSize;
  size_t                      alignedPayloadSize;
  const char*                 file;
  const char*                 func;
  const char*                 tag;
  struct justMemoryHeader*   next;
  struct justMemoryHeader*   prev;
  uint32_t                    magic;
  int32_t                     line;
} justMemHeader;

// - - - Doubly linked list tracking all active allocations
static justMemHeader*  activeAllocations                   = NULL;
static size_t          registeredTagCount                  = 1;
static justTagEntry    memoryTags[JUST_MEMORY_TAG_LIMIT]  =
  {
    [0] =
      {
        .name            = "UNKNOWN",
        .allocatedBytes  = 0,
        .allocationLimit = JUST_MEMORY_ALLOC_DEFAULT_LIMIT,
        .activeCount     = 0
      }
  };


// - - - Internal Helpers - - -

static justTagEntry* getOrCreateTag(const char* TAG)
{
  // - - -If NULL, empty, or literally "UNKNOWN", route directly to slot 0
  if (!TAG || TAG[0] == '\0' || TAG == memoryTags[0].name || strcmp(TAG, "UNKNOWN") == 0)
  {
    return &memoryTags[0];
  }

  for (size_t i = 0; i < registeredTagCount; ++i)
  {
    if (memoryTags[i].name == TAG || strcmp(memoryTags[i].name, TAG) == 0)
    { return &memoryTags[i]; }
  }

  if (registeredTagCount < JUST_MEMORY_TAG_LIMIT)
  {
    justTagEntry* entry  = &memoryTags[registeredTagCount++];

    size_t        tagLen = strlen(TAG);
    if (tagLen >= sizeof(entry->name))
    {
      JUST_LOG_WARNING("[MEMORY TRACKER] : Tag '%s' length (%zu) exceeds max tag name length (%zu)! Truncating tag.",
                        TAG, tagLen, sizeof(entry->name) - 1);
    }
    snprintf(entry->name, sizeof(entry->name), "%s", TAG);

    entry->allocatedBytes   = 0;
    entry->allocationLimit  = JUST_MEMORY_ALLOC_DEFAULT_LIMIT;
    entry->activeCount      = 0;

    return entry;
  }

  JUST_LOG_WARNING("[MEMORY TRACKER] : Max tag limit (%zu) reached! Assigning tag '%s' to UNKNOWN",
                    (size_t)JUST_MEMORY_TAG_LIMIT, TAG);
  JUST_LOG_WARNING("[MEMORY TRACKER] : The tag limit is configurable at compile time, check docs to see how");
  return &memoryTags[0];
}

static inline uint8_t* getPayloadPtr(const justMemHeader* HEADER)
{
  return (uint8_t*)HEADER + sizeof(justMemHeader);
}

static inline justMemHeader* getHeaderPtr(const void* PAYLOAD)
{
  return (justMemHeader*)((uint8_t*)PAYLOAD - sizeof(justMemHeader));
}

static inline uint32_t* getFooterPtr(const justMemHeader* header)
{
  return (uint32_t*)(getPayloadPtr(header) + header->alignedPayloadSize);
}

static void linkAllocation(justMemHeader* HEADER)
{
  HEADER->next = activeAllocations;
  HEADER->prev = NULL;
  if (activeAllocations)
  {
    activeAllocations->prev = HEADER;
  }
  activeAllocations = HEADER;

  justTagEntry* tagEntry   = getOrCreateTag(HEADER->tag);
  tagEntry->allocatedBytes += HEADER->requestedSize;
  tagEntry->activeCount++;
}

static void unlinkAllocation(justMemHeader* header)
{
  if (header->prev) header->prev->next = header->next;
  else              activeAllocations = header->next;

  if (header->next) header->next->prev = header->prev;

  justTagEntry* tagEntry   = getOrCreateTag(header->tag);
  tagEntry->allocatedBytes -= header->requestedSize;
  tagEntry->activeCount--;
}

static bool verifyIntegrity(justMemHeader* HEADER, const char* FILE, const char* FUNC, int32_t LINE)
{
  if (!HEADER) return false;

  if (HEADER->magic == JUST_FREED_MAGIC)
  {
    JUST_LOG_FATAL("[MEMORY TRACKER] : Use-after-free or double-free at %s:%d in %s()! Buffer was already freed.",
                    FILE, LINE, FUNC);
    abort();
  }

  if (HEADER->magic != JUST_HEADER_MAGIC)
  {
    JUST_LOG_FATAL("[MEMORY TRACKER] : Header corruption at %s:%d in %s()! Magic mismatch.",
                    FILE, LINE, FUNC);
    abort();
  }

  uint32_t* footer = getFooterPtr(HEADER);
  uint32_t  footerVal;
  memcpy(&footerVal, footer, sizeof(uint32_t));

  if (footerVal != JUST_FOOTER_MAGIC)
  {
    JUST_LOG_FATAL("[MEMORY TRACKER] : Buffer overflow detected! Tail canary overwritten for %zu-byte allocation (origin: %s:%d in %s). Triggered at %s:%d in %s.",
                    HEADER->requestedSize, HEADER->file, HEADER->line, HEADER->func, FILE, LINE, FUNC);
    abort();
  }

  return true;
}


// - - - Public API Implementation - - -

JUST_API void* justTrackedMalloc(
  size_t      SIZE,
  const char* FILE,
  const char* FUNC,
  int32_t     LINE,
  const char* TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(SIZE != 0, "[TRACKER] : Cannot allocate 0 bytes");

  justTagEntry* tagEntry = getOrCreateTag(TAG);

  if (tagEntry->allocationLimit > 0 && (tagEntry->allocatedBytes + SIZE > tagEntry->allocationLimit))
  {
    JUST_LOG_ERROR("[MEMORY TRACKER] : Limit of %zu bytes exceeded by allocating %zu bytes for tag '%s' at %s:%d",
                    tagEntry->allocationLimit, SIZE, tagEntry->name, FILE, LINE);
    return NULL;
  }

  size_t alignedPayload = (SIZE + 7) & ~7;
  size_t totalBytes     = sizeof(justMemHeader) + alignedPayload + sizeof(uint32_t);

  justMemHeader* header = (justMemHeader*) malloc(totalBytes);
  if (!header)
  {
    JUST_LOG_FATAL("[MEMORY TRACKER] : OS malloc failed for %zu bytes", totalBytes);
    return NULL;
  }

  header->magic              = JUST_HEADER_MAGIC;
  header->requestedSize      = SIZE;
  header->alignedPayloadSize = alignedPayload;
  header->file               = FILE;
  header->func               = FUNC;
  header->line               = LINE;
  header->tag                = tagEntry->name;

  uint32_t footerMagic = JUST_FOOTER_MAGIC;
  memcpy(getFooterPtr(header), &footerMagic, sizeof(uint32_t));

  linkAllocation(header);
  return getPayloadPtr(header);
}

JUST_API void* justTrackedCalloc(
  size_t      COUNT,
  size_t      SIZE,
  const char* FILE,
  const char* FUNC,
  int32_t     LINE,
  const char* TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(COUNT != 0 && SIZE != 0, "[MEMORY TRACKER] : Calloc arguments cannot be 0");

  size_t  total = COUNT * SIZE;
  void*   ptr   = justTrackedMalloc(total, FILE, FUNC, LINE, TAG);

  if (ptr) memset(ptr, 0, total);
  return ptr;
}

JUST_API void* justTrackedRealloc(
  void*       PTR,
  size_t      NEW_SIZE,
  const char* FILE,
  const char* FUNC,
  int32_t     LINE)
{
  JUST_ASSERT_DEBUG_MESSAGE(PTR != NULL, "[MEMORY TRACKER] : Unlike realloc, this library wont call malloc for you if you pass a NULL PTR");
  JUST_ASSERT_DEBUG_MESSAGE(NEW_SIZE != 0, "[MEMORY TRACKER] : Realloc size cannot be 0");

  justMemHeader* oldHeader = getHeaderPtr(PTR);
  if (!verifyIntegrity(oldHeader, FILE, FUNC, LINE)) return NULL;

  justTagEntry* tagEntry = getOrCreateTag(oldHeader->tag);

  if (tagEntry->allocationLimit > 0)
  {
    size_t projected = (tagEntry->allocatedBytes - oldHeader->requestedSize) + NEW_SIZE;
    if (projected > tagEntry->allocationLimit)
    {
      JUST_LOG_ERROR("[MEMORY TRACKER] : Limit exceeded during realloc of %zu bytes for tag '%s'", NEW_SIZE, tagEntry->name);
      return NULL;
    }
  }

  unlinkAllocation(oldHeader);

  size_t alignedPayload = (NEW_SIZE + 7) & ~7;
  size_t totalBytes     = sizeof(justMemHeader) + alignedPayload + sizeof(uint32_t);

  justMemHeader* newHeader = (justMemHeader*) realloc(oldHeader, totalBytes);
  if (!newHeader)
  {
    linkAllocation(oldHeader);
    JUST_LOG_FATAL("[MEMORY TRACKER] : OS realloc failed for %zu bytes", totalBytes);
    return NULL;
  }

  newHeader->requestedSize      = NEW_SIZE;
  newHeader->alignedPayloadSize = alignedPayload;
  newHeader->file               = FILE;
  newHeader->func               = FUNC;
  newHeader->line               = LINE;

  uint32_t footerMagic = JUST_FOOTER_MAGIC;
  memcpy(getFooterPtr(newHeader), &footerMagic, sizeof(uint32_t));

  linkAllocation(newHeader);
  return getPayloadPtr(newHeader);
}

JUST_API void justTrackedFree(void* PTR, const char* FILE, const char* FUNC, int32_t LINE)
{
  if (!PTR) return;

  justMemHeader* header = getHeaderPtr(PTR);
  if (!verifyIntegrity(header, FILE, FUNC, LINE)) return;

  unlinkAllocation(header);

  header->magic         = JUST_FREED_MAGIC;
  uint32_t freedFooter  = JUST_FREED_MAGIC;
  memcpy(getFooterPtr(header), &freedFooter, sizeof(uint32_t));

  free(header);
}

JUST_API void justMemoryReportLeaks(void)
{
  if (!activeAllocations)
  {
    JUST_LOG_INFO("[MEMORY TRACKER] : No memory leaks detected. Clean exit.");
    return;
  }

  size_t totalLeaked = 0;
  size_t count       = 0;

  JUST_LOG_ERROR("[MEMORY TRACKER] : Memory Leaks Detected (Trying to report, but reporting itself may cause segfault):");
  justMemHeader* curr = activeAllocations;
  while (curr)
  {
    count++;
    JUST_LOG_ERROR("  Leak #%zu: %zu bytes | Tag: '%s' | Origin: %s:%d (%s)",
                    count, curr->requestedSize, curr->tag, curr->file, curr->line, curr->func);
    totalLeaked  += curr->requestedSize;
    curr          = curr->next;
  }

  JUST_LOG_ERROR("[MEMORY TRACKER] : Total Leaked: %zu bytes across %zu allocations.", totalLeaked, count);
}

JUST_API bool justMemoryCheckBounds(void)
{
  bool            clean = true;
  justMemHeader*  curr  = activeAllocations;
  while (curr)
  {
    if (!verifyIntegrity(curr, __FILE__, __func__, __LINE__)) clean = false;
    curr = curr->next;
  }
  return clean;
}

JUST_API void justMemorySetLimit(size_t LIMIT_BYTES, const char* TAG)
{
  justTagEntry* entry     = getOrCreateTag(TAG);
  entry->allocationLimit  = LIMIT_BYTES == 0 ? SIZE_MAX : LIMIT_BYTES;
}

JUST_API size_t justMemoryGetLimit(const char* TAG)
{
  justTagEntry* entry = getOrCreateTag(TAG);
  return entry->allocationLimit;
}

JUST_API size_t justMemoryGetActiveBytes(const char* TAG)
{
  if (!TAG)
  {
    size_t total = 0;
    for (size_t i = 0; i < registeredTagCount; ++i) total += memoryTags[i].allocatedBytes;
    return total;
  }
  justTagEntry* entry = getOrCreateTag(TAG);
  return entry->allocatedBytes;
}

JUST_API void justMemoryLogUsage(bool VERBOSE)
{
  static const char*  units[]   = { "Bytes", "KiB", "MiB", "GiB", "TiB", "PiB" };
  const size_t        maxUnits  = sizeof(units) / sizeof(units[0]);

  JUST_LOG_INFO("[MEMORY TRACKER] : Registered Memory Subsystems (%zu tags):", registeredTagCount);

  for (size_t i = 0; i < registeredTagCount; ++i)
  {
    justTagEntry* e = &memoryTags[i];

    // - - - Format allocatedBytes
    char    allocBuf[32];
    double  allocVal  = (double)e->allocatedBytes;
    size_t  allocUnit = 0;
    while (allocVal >= 1024.0 && allocUnit < maxUnits - 1)
    {
      allocVal /= 1024.0;
      allocUnit++;
    }
    if (allocUnit == 0)   snprintf(allocBuf, sizeof(allocBuf), "%zu %s", e->allocatedBytes, units[allocUnit]);
    else                  snprintf(allocBuf, sizeof(allocBuf), "%.2f %s", allocVal, units[allocUnit]);

    // - - - Format allocationLimit
    if (e->allocationLimit > 0)
    {
      char    limitBuf[32];
      double  limitVal  = (double)e->allocationLimit;
      size_t  limitUnit = 0;

      while (limitVal >= 1024.0 && limitUnit < maxUnits - 1)
      {
        limitVal /= 1024.0;
        limitUnit++;
      }
      if (limitUnit == 0) snprintf(limitBuf, sizeof(limitBuf), "%zu %s", e->allocationLimit, units[limitUnit]);
      else                snprintf(limitBuf, sizeof(limitBuf), "%.2f %s", limitVal, units[limitUnit]);

      double pct = (double)e->allocatedBytes / (double)e->allocationLimit * 100.0;
      JUST_LOG_INFO("  ├── Tag: %-16s | %s / %s (%.1f%%) [%zu active]",
                    e->name, allocBuf, limitBuf, pct, e->activeCount);
      #ifndef DEBUG
          (void)pct;
      #endif
    }
    else
    {
      JUST_LOG_INFO("  ├── Tag: %-16s | %s (unlimited) [%zu active]",
                    e->name, allocBuf, e->activeCount);
    }

    if (VERBOSE && e->activeCount > 0)
    {
      justMemHeader* curr = activeAllocations;
      while (curr)
      {
        if (curr->tag == e->name || strcmp(curr->tag, e->name) == 0)
        {
          char    reqBuf[32];
          double  reqVal  = (double)curr->requestedSize;
          size_t  reqUnit = 0;

          while (reqVal >= 1024.0 && reqUnit < maxUnits - 1)
          {
            reqVal /= 1024.0;
            reqUnit++;
          }
          if (reqUnit == 0)    snprintf(reqBuf, sizeof(reqBuf), "%zu %s", curr->requestedSize, units[reqUnit]);
          else                 snprintf(reqBuf, sizeof(reqBuf), "%.2f %s", reqVal, units[reqUnit]);

          JUST_LOG_DEBUG("  │   └── %s at %s:%d (%s)",
                         reqBuf, curr->file, curr->line, curr->func);
        }
        curr = curr->next;
      }
    }
  }
}

#endif // JUST_LIB_IMPL_TRACKER


// - - - Linear Allocator - - -

// - - - HEADER
#ifndef JUST_LINEAR_ALLOC
#define JUST_LINEAR_ALLOC

/**
 * @file linearAlloc.h 
 * @brief Dead-simple fixed-capacity linear/bump allocator in C11.
 *        No reallocations, stable pointers, O(1) resets and marker rollbacks.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_ALIGNMENT_BYTES  
  #define DEFAULT_ALIGNMENT_BYTES alignof(max_align_t)
#endif

/// @brief : Marker representing a rollback point in the allocator
typedef size_t justLinearMarker;

/// @brief Linear Allocator struct 
typedef struct justLinearAllocator 
{
  void*  memory;      ///< Base pointer to contiguous memory buffer
  size_t totalSize;   ///< Total buffer capacity in bytes
  size_t allocated;   ///< Current bump offset
  bool   ownsMemory;  ///< True if memory was allocated by this struct and must be freed
} JustLinearAllocator;

/**
 * @brief : Initializes a fixed-capacity linear allocator.
 * @param ALLOCATOR : Pointer to LinearAllocator struct.
 * @param TOTAL_SIZE : Total capacity in bytes.
 * @param USER_BUFFER : Optional user-supplied pre-allocated buffer (or NULL to allocate from heap).
 * @param TAG : Tracking tag used when allocating heap memory (ignored if USER_BUFFER is provided).
 * @return : true if initialized successfully, false otherwise.
 */
JUST_API bool justLinearAllocCreate(
  JustLinearAllocator* ALLOCATOR,
  size_t                TOTAL_SIZE,
  void*                 USER_BUFFER,
  const char*           TAG);

/**
 * @brief : deletes a linear allocator 
 * @param ALLOCATOR : a pointer to the linear allocator to destroy
*/
JUST_API void justLinearAllocDestroy(JustLinearAllocator* ALLOCATOR);

/**
 * @brief : Alloactes memory from the allocator and returns it 
 * @param ALLOACTOR : a pointer to the linear allocator from where memory is to be allocated 
 * @param SIZE : how much to allocate 
 * @param ALIGNMENT : ALIGNMENT Required alignment in bytes (must be a power of two, or 0 for default). 
 * @return : a pointer to the memory if successful, NULL if fail (for example not being able to resize)
*/
JUST_API void* justLinearAllocAllocate(
  JustLinearAllocator* ALLOCATOR,
  size_t                SIZE,
  size_t                ALIGNMENT);

/**
 * @brief : Resets the entire allocator back to zero. Memory is retained for reuse.
 * @param ALLOCATOR :Pointer to the allocator.
 */
JUST_API static inline void justLinearAllocReset(JustLinearAllocator* ALLOCATOR)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot reset a NULL ALLOCATOR");
  if (ALLOCATOR) ALLOCATOR->allocated = 0;
}

/**
 * @brief : Retrieves the current allocation marker for scoped lifetime rollbacks.
 * @param ALLOCATOR : Pointer to the allocator.
 * @return : Marker offset.
 */
JUST_API static inline justLinearMarker justLinearAllocGetMarker(const JustLinearAllocator* ALLOCATOR)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot get a marker from NULL ALLOCATOR");
  return ALLOCATOR->allocated;
}

/**
 * @brief : Rolls back the allocator to a previously saved marker.
 * @param ALLOCATOR : Pointer to the allocator.
 * @param MARKER : Previously captured marker.
 */
JUST_API static inline void justLinearAllocRewind(JustLinearAllocator* ALLOCATOR, justLinearMarker MARKER)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot rewind on a NULL MARKER");

  if (MARKER <= ALLOCATOR->allocated)
  {
    ALLOCATOR->allocated = MARKER;
  }
  else
  {
    JUST_LOG_ERROR("[LINEAR ALLOCATOR] : Cannot rewind if MARKER (%zu) is greater than what is currently allocated (%zu)", MARKER, ALLOCATOR->allocated);
  }
}

/**
 * @brief : Returns total active bytes allocated.
 * @param ALLOCATOR : Pointer to the allocator
 * @return : how many bytes are used
 */
JUST_API static inline size_t justLinearAllocGetUsed(const JustLinearAllocator* ALLOCATOR)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot get used on NULL ALLOCATOR");

  return ALLOCATOR->allocated;
}

/**
 * @brief : Returns remaining capacity in bytes.
 * @param ALLOCATOR : Pointer to the allocator
 * @return : How many bytes can still be allocatoed here
 */
JUST_API static inline size_t justLinearAllocGetRemaining(const JustLinearAllocator* ALLOCATOR)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot get remaining on NULL ALLOCATOR");

  return ALLOCATOR->totalSize - ALLOCATOR->allocated;
}
/**
 * @brief : prints debug info on the allocator in debug mode, does nothing in release mode 
 * @param ALLOCATOR : a pointer to the allocator to be visualized
*/
JUST_API void justLinearAllocDebugPrint(JustLinearAllocator* ALLOCATOR);

#ifdef __cplusplus
}
#endif

#endif // JUST_LINEAR_ALLOC

// - - - Implementation
#ifdef JUST_LIB_IMPL_LIN_ALLOC

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

static inline bool justlinearAllocIsPowerOfTwo(size_t X)
{
  return (X != 0) && ((X & (X - 1)) == 0);
}

bool justLinearAllocCreate(
  JustLinearAllocator* ALLOCATOR,
  size_t                TOTAL_SIZE,
  void*                 USER_BUFFER,
  const char*           TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOC] : Target ALLOCATOR pointer cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(TOTAL_SIZE > 0,    "[LINEAR ALLOC] : TOTAL_SIZE must be greater than 0");

  ALLOCATOR->totalSize  = TOTAL_SIZE;
  ALLOCATOR->allocated  = 0;
  ALLOCATOR->ownsMemory = (USER_BUFFER == NULL);

  if (ALLOCATOR->ownsMemory)
  {
    const char* allocTag = (TAG && TAG[0] != '\0') ? TAG : "LINEAR_ALLOC_BUFFER";
    ALLOCATOR->memory = JUST_MALLOC_TAGGED(TOTAL_SIZE, allocTag);

    if (!ALLOCATOR->memory)
    {
      JUST_LOG_FATAL("[LINEAR ALLOC] : Failed to allocate %zu bytes for backing buffer", TOTAL_SIZE);
      return false;
    }
  }
  else
  {
    // - - - Verify user buffer satisfies max_align_t alignment
    JUST_ASSERT_DEBUG_MESSAGE(
      ((uintptr_t)USER_BUFFER % alignof(max_align_t)) == 0,
      "[LINEAR ALLOC] : Supplied USER_BUFFER must be aligned to max_align_t");

    ALLOCATOR->memory = USER_BUFFER;
  }

  return true;
}

void justLinearAllocDestroy(JustLinearAllocator* ALLOCATOR)
{
  if (!ALLOCATOR) return;

  if (ALLOCATOR->ownsMemory && ALLOCATOR->memory)
  {
    JUST_FREE(ALLOCATOR->memory);
  }

  ALLOCATOR->memory     = NULL;
  ALLOCATOR->totalSize  = 0;
  ALLOCATOR->allocated  = 0;
  ALLOCATOR->ownsMemory = false;
}

void* justLinearAllocAllocate(
  JustLinearAllocator* ALLOCATOR, 
  size_t                SIZE, 
  size_t                ALIGNMENT)
{
  JUST_ASSERT_DEBUG(ALLOCATOR != NULL);
  if (SIZE == 0) return NULL;

  if (ALIGNMENT == 0)
  {
    ALIGNMENT = DEFAULT_ALIGNMENT_BYTES;
  }

  JUST_ASSERT_DEBUG_MESSAGE(justlinearAllocIsPowerOfTwo(ALIGNMENT), "[LINEAR ALLOC] : ALIGNMENT must be a power of two");

  // - - - Absolute pointer alignment
  uintptr_t currentPtr  = (uintptr_t)ALLOCATOR->memory + ALLOCATOR->allocated;
  uintptr_t alignedPtr  = (currentPtr + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
  size_t    padding     = alignedPtr - currentPtr;

  if (ALLOCATOR->allocated + padding + SIZE > ALLOCATOR->totalSize)
  {
    JUST_LOG_ERROR("[LINEAR ALLOC] : Out of memory! Requested %zu bytes (pad %zu), but only %zu bytes remaining",
                    SIZE, padding, ALLOCATOR->totalSize - ALLOCATOR->allocated);
    return NULL;
  }

  ALLOCATOR->allocated += padding + SIZE;
  return (void*)alignedPtr;
}

void justLinearAllocDebugPrint(JustLinearAllocator* ALLOCATOR)
{
  #ifdef DEBUG 
    #include <memory.h>
    JUST_ASSERT_DEBUG_MESSAGE(ALLOCATOR != NULL, "[LINEAR ALLOCATOR] : Cannot debug print a NULL ALLOCATOR");

    size_t BAR_WIDTH = 50;

    size_t used = ALLOCATOR->allocated;
    if (used > ALLOCATOR->totalSize) used = ALLOCATOR->totalSize;

    size_t freeBytes = ALLOCATOR->totalSize - used;

    double usedPercent = 0.0;
    double freePercent = 0.0;

    if (ALLOCATOR->totalSize > 0)
    {
      usedPercent = 100.0 * (double)used / (double)ALLOCATOR->totalSize;
      freePercent = 100.0 - usedPercent;
    }

    size_t filled = (ALLOCATOR->totalSize == 0)
      ? 0
      : (used * BAR_WIDTH) / ALLOCATOR->totalSize;

    char bar[BAR_WIDTH + 1];

    memset(bar, '_', BAR_WIDTH);
    memset(bar, '*', filled);

    if (filled < BAR_WIDTH) bar[filled] = '@';

    bar[(int)BAR_WIDTH] = '\0';

    JUST_LOG_INFO(
      "Pool [%s]\n"
      "Capacity : %zu bytes\n"
      "Used     : %zu (%.1f%%)\n"
      "Free     : %zu (%.1f%%)",
      bar,
      ALLOCATOR->totalSize,
      used,
      usedPercent,
      freeBytes,
      freePercent);
  #else 
    (void)ALLOCATOR;
  #endif
}

#endif // JUST_LIB_IMPL_LIN_ALLOC


// - - - Bitset (Tangent) - - -

// - - - Header
#ifndef JUST_BISET
#define JUST_BITSET

/**
 * @file : justBitset.h
 * @brief : Lightweight dynamic justBitset
*/

#include <memory.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
  extern "C" {
#endif

#define JUST_BITS_PER_WORD (sizeof(uint64_t) * 8)

/// @brief : justBitset struct
typedef struct justBitset
{
  size_t    capacity;   ///< Total trackable entity count
  size_t    wordCount;  ///< Number of 64-bit backing words
  uint64_t* words;      ///< Backing memory buffer
  bool      ownsMemory; ///< True if allocated internally, false if passed in
} JustBitset;


// - - - Internal Tail Helper
JUST_API static inline uint64_t justBitsetGetTailMask(size_t CAPACITY)
{
  size_t rem = CAPACITY % JUST_BITS_PER_WORD;
  return (rem == 0) ? ~(uint64_t)0 : (((uint64_t)1 << rem) - 1);
}

/**
 * @brief : Initializes a justBitset
 * @param SET : Pointer to the justBitset struct to initialize.
 * @param CAPACITY : Number of bits / entities to track
 * @param USER BUFFER : Optional pre-allocaed memory buffer or NULL to allocate
 * @param TAG : Tracking tag used if allocated intenrally
 * @warning : MEMORY must be aligned to sizeof(size_t)
 * @return : true on success, false on failure
*/
JUST_API bool justBitsetCreate(
  JustBitset*   SET,
  size_t        CAPACITY,
  void*         USER_BUFFER,
  const char*   TAG);

/**
 * @brief : Destroys the justBitset and frees backing memory if owned
 * @param SET: Pointer to the justBitset to destroy
*/
JUST_API void justBitsetDestroy(JustBitset* SET);

/**
 * @brief : Sets a bit at index to 1
 * @param SET : Pointer to the justBitset
 * @param INDEX : Which index to set
*/
JUST_API static inline void justBitsetSet(JustBitset* SET, size_t INDEX)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot set a bit in a NULL SET");
  JUST_ASSERT_DEBUG_MESSAGE(INDEX < SET->capacity, "[BITSET] : Index out of bounds");

  SET->words[INDEX / JUST_BITS_PER_WORD] |= ((uint64_t)1 << (INDEX % JUST_BITS_PER_WORD));
}

/**
 * @brief : Sets a bit at index to 0
 * @param SET : Pointer to the justBitset
 * @param INDEX : Which index to clear
*/
JUST_API static inline void justBitsetClear(JustBitset* SET, size_t INDEX)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot set a bit in a NULL SET");
  JUST_ASSERT_DEBUG_MESSAGE(INDEX < SET->capacity, "[BITSET] : Index out of bounds");

  SET->words[INDEX / JUST_BITS_PER_WORD] &= ~((uint64_t)1 << (INDEX % JUST_BITS_PER_WORD));
}

/**
 * @brief : Toggles a bit at index
 * @param SET : Pointer to the justBitset
 * @param INDEX : Which index to toggle
*/
JUST_API static inline void justBitsetToggle(JustBitset* SET, size_t INDEX)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot toggle a bit in a NULL SET");
  JUST_ASSERT_DEBUG_MESSAGE(INDEX < SET->capacity, "[BITSET] : Index out of bounds");

  SET->words[INDEX / JUST_BITS_PER_WORD] ^= ((uint64_t)1 << (INDEX % JUST_BITS_PER_WORD));
}

/**
 * @brief : Gets a bit at index
 * @param SET : Pointer to the justBitset
 * @param INDEX : Which index to set
 * @return : True if the bit is set, false otherwise
*/
JUST_API static inline bool justBitsetGet(JustBitset* SET, size_t INDEX)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot get a bit in a NULL SET");
  JUST_ASSERT_DEBUG_MESSAGE(INDEX < SET->capacity, "[BITSET] : Index out of bounds");

  return (SET->words[INDEX / JUST_BITS_PER_WORD] & ((uint64_t)1 << (INDEX % JUST_BITS_PER_WORD))) != 0;
}

/**
 * @brief : Clears all bits to 0
 * @param SET : Pointer to the justBitset
*/
JUST_API static inline void justBitsetClearAll(JustBitset* SET)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot clear all in a NULL SET");

  memset(SET->words, 0, SET->wordCount * sizeof(uint64_t));
}

/**
 * @brief : Sets all bits to 1
 * @param SET : Pointer to the justBitset
*/
JUST_API static inline void justBitsetSetAll(JustBitset* SET)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot set all in a NULL SET");

  memset(SET->words, 0xFF, SET->wordCount * sizeof(uint64_t));
  if (SET->wordCount > 0)
  {
    SET->words[SET->wordCount - 1] &= justBitsetGetTailMask(SET->capacity);
  }
}

/**
 * @brief : Check capacity of a bitset
 * @param SET : Pointer to the justBitset
*/
JUST_API static inline size_t justBitsetCapacity(const JustBitset* SET)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot check capacity of a NULL SET");

  return SET->capacity;
}

/**
 * @brief : Computes in -place union : DST = DST | SRC
 * @warning : The smaller capacity is used
 * @warning : There should be no memory overlap
 * @param DST : The destination justBitset
 * @param SRC : The source justBitset
*/
JUST_API void justBitsetUnion(JustBitset* DST, const JustBitset* SRC);

/**
 * @brief : Computes in -place intersection : DST = DST & SRC
 * @warning : The smaller capacity is used
 * @warning : There should be no memory overlap
 * @param DST : The destination justBitset
 * @param SRC : The source justBitset
*/
JUST_API void justBitsetIntersection(JustBitset* DST, const JustBitset* SRC);

/**
 * @brief : Computes in -place difference : DST = DST & ~SRC
 * @warning : The smaller capacity is used
 * @warning : There should be no memory overlap
 * @param DST : The destination justBitset
 * @param SRC : The source justBitset
*/
JUST_API void justBitsetDifference(JustBitset* DST, const JustBitset* SRC);

/**
 * @brief : Compares two justBitsets for equality
 * @param A : One justBitset
 * @param B : Second justBitset
 * @return : True if identical in capacity and set bits, false otherwise
 */
JUST_API bool justBitsetEquals(const JustBitset* A, const JustBitset* B);

#ifdef __cplusplus
}
#endif

#endif // JUST_BITSET

// - - - Implementation
#ifdef JUST_LIB_IMPL_BITSET

#include <stdalign.h>
#include <stdint.h>

JUST_API bool justBitsetCreate(
  JustBitset* SET,
  size_t       CAPACITY,
  void*        USER_BUFFER,
  const char*  TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Target bitset cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(CAPACITY > 0, "[BITSET] : Capacity must be > 0");

  SET->capacity   = CAPACITY;
  SET->wordCount  = (CAPACITY + JUST_BITS_PER_WORD - 1) / JUST_BITS_PER_WORD;
  SET->ownsMemory = (USER_BUFFER == NULL);

  size_t totalBytes = SET->wordCount * sizeof(uint64_t);

  if (SET->ownsMemory)
  {
    const char* allocTag = (TAG && TAG[0] != '\0') ? TAG : "BITSET";
    SET->words = (uint64_t*) JUST_MALLOC_TAGGED(totalBytes, allocTag);
    if (!SET->words)
    {
      JUST_LOG_ERROR("[BITSET] : Failed to allocate buffer of %zu bytes", totalBytes);
      return false;
    }
  }
  else
  {
    JUST_ASSERT_DEBUG_MESSAGE(
      ((uintptr_t)USER_BUFFER % alignof(uint64_t)) == 0,
      "[BITSET] : Provided userBuffer is not properly word-aligned");
    SET->words = (uint64_t*) USER_BUFFER;
  }

  justBitsetClearAll(SET);
  return true;
}

JUST_API void justBitsetDestroy(JustBitset* SET)
{
  JUST_ASSERT_DEBUG_MESSAGE(SET != NULL, "[BITSET] : Cannot destroy a NULL set");

  if (SET->ownsMemory && SET->words)
  {
    JUST_FREE(SET->words);
  }

  SET->words      = NULL;
  SET->capacity   = 0;
  SET->wordCount  = 0;
  SET->ownsMemory = false;
}

JUST_API bool justBitsetEquals(const JustBitset* A, const JustBitset* B)
{
  JUST_ASSERT_DEBUG_MESSAGE(A != NULL && B != NULL, "[BITSET] : Cannot compare NULL sets");

  if (A->capacity != B->capacity) return false;
  if (A->wordCount == 0)          return true;

  // - - - Compare full words
  for (size_t i = 0; i < A->wordCount - 1; ++i)
  {
    if (A->words[i] != B->words[i]) return false;
  }

  uint64_t  tailMask  = justBitsetGetTailMask(A->capacity);
  size_t    lastIdx   = A->wordCount - 1;
  return ((A->words[lastIdx] & tailMask) == (B->words[lastIdx] & tailMask));
}

JUST_API void justBitsetUnion(JustBitset* DST, const JustBitset* SRC)
{
  JUST_ASSERT_DEBUG_MESSAGE(DST != NULL && SRC != NULL, "[BITSET] : Cannot union NULL sets");

  size_t                        count = (DST->wordCount < SRC->wordCount) ? DST->wordCount : SRC->wordCount;
  uint64_t* JUST_RESTRICT       d     = DST->words;
  const uint64_t* JUST_RESTRICT s     = SRC->words;

  for (size_t i = 0; i < count; ++i)
  {
    d[i] |= s[i];
  }
}

JUST_API void justBitsetIntersection(JustBitset* DST, const JustBitset* SRC)
{
  JUST_ASSERT_DEBUG_MESSAGE(DST != NULL && SRC != NULL, "[BITSET] : Cannot intersect NULL bitsets");

  size_t                        count = (DST->wordCount < SRC->wordCount) ? DST->wordCount : SRC->wordCount;
  uint64_t*       JUST_RESTRICT d     = DST->words;
  const uint64_t* JUST_RESTRICT s     = SRC->words;

  for (size_t i = 0; i < count; ++i)
  {
    d[i] &= s[i];
  }
}

JUST_API void justBitsetDifference(JustBitset* DST, const JustBitset* SRC)
{
  JUST_ASSERT_DEBUG_MESSAGE(DST != NULL && SRC != NULL, "[BITSET] : Cannot difference NULL bitsets");

  size_t                        count = (DST->wordCount < SRC->wordCount) ? DST->wordCount : SRC->wordCount;
  uint64_t*       JUST_RESTRICT d     = DST->words;
  const uint64_t* JUST_RESTRICT s     = SRC->words;

  for (size_t i = 0; i < count; ++i)
  {
    d[i] &= ~s[i];
  }
}

#endif // JUST_LIB_IMPL_BITSET


// - - - Object Pool - - - 

// - - - Header
#ifndef JUST_OBJ_POOL
#define JUST_OBJ_POOL

/**
 * @file objectPool.h 
 * @brief Great memory management tool for objects of the same size
*/

#include <stdbool.h>
#include <stddef.h>

#define JUST_POOL_END_OF_LIST ((size_t)-1)

#ifdef __cplusplus
extern "C" {
#endif


/// @brief ObjectPool metadata
typedef struct justObjectPool
{
  size_t       stride;          ///< Actual slot stride including alignment padding
  size_t       objectSize;      ///< Raw requested object size in bytes
  size_t       capacity;        ///< Maximum objects pool can track
  size_t       freeListOffset;  ///< Byte offset to head of free list (-1 if empty)
  size_t       freeCount;       ///< Number of currently free slots
  void*        memory;          ///< Backing memory block
  bool         ownsMemory;      ///< True if pool allocated memory itself
#ifdef DEBUG
  JustBitset  allocatedBits;   ///< Tracks active allocations to trap double-returns
#endif
} justObjectPool;

/**
 * @brief : ObjectPool creation function, the struct itself is the config, set fields before calling this function 
 * @param POOL : a pointer to the pool to be initialized, also acts as the config 
 * @param CAPACITY : how many objects should be in the pool 
 * @param OBJECT_SIZE : size of an object in buyes 
 * @param USER_MEMORY : the backing memory, pass NULL if you want the pool to handle memory itself
 * @param TAG : Tracking tag for memory tracker (ignored if userMemory is provided)
 * @return : True on success, false on fail
*/
JUST_API bool justObjectPoolCreate(
  justObjectPool*  POOL,
  size_t            CAPACITY,
  size_t            OBJECT_SIZE,
  size_t            ALIGNMENT,
  void*             USER_MEMORY,
  const char*       TAG);

/**
 * @brief : Takes an object from object pool 
 * @param POOL : a pointer to the pool from which the object is to be taken 
 * @return : Pointer to object, or NULL if full and cannot resize
*/
JUST_API void* justObjectPoolTakeObject(justObjectPool* POOL);

/**
 * @brief : Returns an object back to the pool for reuse 
 * @param POOL : the pool to be returned to 
 * @param OBJECT : the object to be returned
*/
JUST_API void justObjectPoolReturnObject(justObjectPool* POOL, void* OBJECT);

/**
 * @brief : Destroys the Object Pool and frees backing memory if owned 
 * @param POOL : Pointer to the pool to be destroyed 
*/
JUST_API void justObjectPoolDestroy(justObjectPool* POOL);

/**
 * @brief : Returns true if no objects are currently taken from the pool.
 * @param POOL : Pointer to the pool
 * @return : whether the pool is empty
 */
JUST_API static inline bool justObjectPoolIsEmpty(const justObjectPool* POOL)
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] : Cannot check if a NULL POOL is empty");
  return POOL->freeCount == POOL->capacity;
}

/**
 * @brief : Returns true if the pool is completely out of free objects.
 * @param POOL : Pointer to the pool
 * @return : whether the pool is full
 */
JUST_API static inline bool justObjectPoolIsFull(const justObjectPool* POOL)
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] : Cannot check if a NULL POOL is full");
  return POOL->freeCount == 0;
}
/**
 * @brief : debug pritns the object pool in debug mode, does nothing in release mode 
 * @param POOL : a pointer to the pool to be visualized
*/
JUST_API void justObjectPoolDebugPrint(const justObjectPool* POOL);


#ifdef __cplusplus
}
#endif

#endif // JUST_OBJ_POOL

// - - - Implementation
#ifdef JUST_LIB_IMPL_OBJ_POOL

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

static inline bool objectPoolIsPowerOfTwo(size_t x)
{
  return (x != 0) && ((x & (x - 1)) == 0);
}

bool justObjectPoolCreate(
  justObjectPool*  POOL,
  size_t            CAPACITY,
  size_t            OBJECT_SIZE,
  size_t            ALIGNMENT,
  void*             USER_MEMORY,
  const char*       TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL,        "[OBJECT POOL] : Cannot initialize a NULL ObjectPool pointer");
  JUST_ASSERT_DEBUG_MESSAGE(CAPACITY > 0,        "[OBJECT POOL] : CAPACITY must be at least 1");
  JUST_ASSERT_DEBUG_MESSAGE(OBJECT_SIZE > 0,     "[OBJECT POOL] : OBJECT_SIZE must be greater than 0");

  POOL->capacity    = CAPACITY;
  POOL->objectSize  = OBJECT_SIZE;
  POOL->ownsMemory  = (USER_MEMORY == NULL);

  if (ALIGNMENT == 0) ALIGNMENT = alignof(max_align_t); 
  JUST_ASSERT_DEBUG_MESSAGE(objectPoolIsPowerOfTwo(ALIGNMENT), "[OBJECT POOL] : Alignment must be a power of 2");

  POOL->capacity   = CAPACITY;
  POOL->objectSize = OBJECT_SIZE;
  POOL->ownsMemory = (USER_MEMORY == NULL);

  // - - - Stride must fit at least a free-list size_t offset and be aligned to requested alignment
  size_t minSlotSize  = (OBJECT_SIZE < sizeof(size_t)) ? sizeof(size_t) : OBJECT_SIZE;
  POOL->stride        = (minSlotSize + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

  size_t      totalBytes  = POOL->capacity * POOL->stride;
  const char* allocTag    = (TAG && TAG[0] != '\0') ? TAG : "OBJECT_POOL";

  if (POOL->ownsMemory)
  {
    POOL->memory = JUST_MALLOC_TAGGED(totalBytes, allocTag);
    if (!POOL->memory)
    {
      JUST_LOG_FATAL("[OBJECT POOL] : Failed to allocate %zu bytes for POOL backing buffer", totalBytes);
      return false;
    }
  }
  else
  {
    JUST_ASSERT_DEBUG_MESSAGE(((uintptr_t)USER_MEMORY & (ALIGNMENT - 1)) == 0,
                               "[OBJECT POOL] : Provided userMemory is not aligned to requested boundary");
    POOL->memory = USER_MEMORY;
  }

  #ifdef DEBUG
    if (!justBitsetCreate(&POOL->allocatedBits, CAPACITY, NULL, "OBJECT_POOL"))
    {
      if (POOL->ownsMemory) JUST_FREE(POOL->memory);
      return false;
    }
  #endif

  // - - - Wire singly linked free-list
  POOL->freeListOffset = 0;
  POOL->freeCount      = CAPACITY;

  uint8_t* bytePtr = (uint8_t*)POOL->memory;
  for (size_t i = 0; i < CAPACITY - 1; ++i)
  {
    size_t* nextSlot  = (size_t*)(bytePtr + (i * POOL->stride));
    *nextSlot         = (i + 1) * POOL->stride;
  }

  size_t* lastSlot  = (size_t*)(bytePtr + ((CAPACITY - 1) * POOL->stride));
  *lastSlot         = JUST_POOL_END_OF_LIST;

  return true;
}

void justObjectPoolDestroy(justObjectPool* POOL) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] : Cannot destroy a NULL ObjectPool pointer");

  #ifdef DEBUG
    justBitsetDestroy(&POOL->allocatedBits);
  #endif

  if (POOL->ownsMemory && POOL->memory) 
  {
    JUST_FREE(POOL->memory);
  }

  POOL->memory          = NULL;
  POOL->capacity        = 0;
  POOL->objectSize      = 0;
  POOL->stride          = 0;
  POOL->freeListOffset  = JUST_POOL_END_OF_LIST;
  POOL->freeCount       = 0;
  POOL->ownsMemory      = false;
}

void* justObjectPoolTakeObject(justObjectPool* POOL) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] : Cannot take from NULL pool");
  JUST_ASSERT_DEBUG_MESSAGE(POOL->memory != NULL, "[OBJECT POOL] : Pool memory is NULL, make sure pool is initialized");

  if (POOL->freeListOffset == JUST_POOL_END_OF_LIST) 
  {
    JUST_LOG_ERROR("[OBJECT POOL] : Out of objects!");
    return NULL;
  }

  // - - - Pop element from free list using byte offset
  uintptr_t objAddr   = (uintptr_t)POOL->memory + POOL->freeListOffset;
  size_t    slotIndex = POOL->freeListOffset / POOL->stride;

  // - - - Pop head of free list
  POOL->freeListOffset = *(size_t*)objAddr;
  POOL->freeCount--;

  #ifdef DEBUG
    JUST_ASSERT_DEBUG_MESSAGE(!justBitsetGet(&POOL->allocatedBits, slotIndex),
                             "[OBJECT POOL] : Internal invariant failure: taking already allocated slot");
    justBitsetSet(&POOL->allocatedBits, slotIndex);
  #else
    (void) slotIndex;
  #endif

  return (void*)objAddr;
}

void justObjectPoolReturnObject(justObjectPool* POOL, void* OBJECT) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] Cannot return object to NULL pool");
  JUST_ASSERT_DEBUG_MESSAGE(OBJECT != NULL, "[OBJECT POOL] Cannot return NULL object");

  uintptr_t objAddr     = (uintptr_t)OBJECT;
  uintptr_t baseAddr    = (uintptr_t)POOL->memory;
  size_t    byteOffset  = objAddr - baseAddr;

  // - - - Verify bounds and alignment
  JUST_ASSERT_DEBUG_MESSAGE(objAddr >= baseAddr && byteOffset < (POOL->capacity * POOL->stride),
                             "[OBJECT POOL] : Returned pointer is outside pool boundary");
  JUST_ASSERT_DEBUG_MESSAGE((byteOffset % POOL->stride) == 0,
                             "[OBJECT POOL] : Returned pointer is misaligned with pool stride");

  size_t slotIndex = byteOffset / POOL->stride;

  #ifdef DEBUG
    JUST_ASSERT_DEBUG_MESSAGE(justBitsetGet(&POOL->allocatedBits, slotIndex),
                              "[OBJECT POOL] : Double-free detected! Slot was not active or already returned");
    justBitsetClear(&POOL->allocatedBits, slotIndex);
  #else
    (void) slotIndex;
  #endif

  // - - - Push back onto head of free list
  *(size_t*)OBJECT      = POOL->freeListOffset;
  POOL->freeListOffset  = byteOffset;
  POOL->freeCount++;
}

void justObjectPoolDebugPrint(const justObjectPool* POOL)
{
  #ifdef DEBUG
    JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[OBJECT POOL] Cannot visualize a NULL pool");

    if (!POOL || POOL->capacity == 0) return;

    bool* freeSlots = (bool*)JUST_MALLOC(POOL->capacity * sizeof(bool));
    if (!freeSlots) return;
    memset(freeSlots, 0, POOL->capacity * sizeof(bool));

    char* bar = (char*)JUST_MALLOC(POOL->capacity + 1);
    if (!bar)
    {
      JUST_FREE(freeSlots);
      return;
    }

    // - - - Walk free list using byte offsets
    size_t curr_offset   = POOL->freeListOffset;
    size_t visited_count = 0;

    while (curr_offset != JUST_POOL_END_OF_LIST)
    {
      size_t slot_index = curr_offset / POOL->stride;

      if (slot_index >= POOL->capacity || (curr_offset % POOL->stride != 0))
      {
        JUST_LOG_ERROR("[OBJECT POOL] : Pool is corrupted (invalid offset %zu)", curr_offset);
        break;
      }

      if (visited_count >= POOL->capacity || freeSlots[slot_index])
      {
        JUST_LOG_ERROR("[OBJECT POOL] : Cycle detected in free list!");
        break;
      }

      freeSlots[slot_index] = true;
      visited_count++;

      uintptr_t slot_addr = (uintptr_t)POOL->memory + curr_offset;
      curr_offset = *(size_t*)slot_addr;
    }

    // - - - - Build bar representation
    size_t head_index = (POOL->freeListOffset != JUST_POOL_END_OF_LIST) ? (POOL->freeListOffset / POOL->stride) : JUST_POOL_END_OF_LIST;
    for (size_t i = 0; i < POOL->capacity; ++i)
    {
      if (i == head_index && freeSlots[i]) {
        bar[i] = '@'; // Free list head
      } else {
        bar[i] = freeSlots[i] ? '_' : '*'; // _ = Free, * = Occupied
      }
    }
    bar[POOL->capacity] = '\0';

    const size_t used = POOL->capacity - POOL->freeCount;

    JUST_LOG_INFO(
      "Pool [%s]\n"
      "Capacity : %zu\n"
      "Used     : %zu (%.1f%%)\n"
      "Free     : %zu (%.1f%%)\n",
      bar,
      POOL->capacity,
      used,
      POOL->capacity ? (100.0 * (double)used) / (double)POOL->capacity : 0.0,
      POOL->freeCount,
      POOL->capacity ? (100.0 * (double)POOL->freeCount) / (double)POOL->capacity : 0.0
    );

    JUST_FREE(bar);
    JUST_FREE(freeSlots);
  #else 
    (void)POOL;
  #endif
}

#endif // JUST_LIB_IMPL_OBJ_POOL


// - - - | Data Structures | - - -


// - - - Dynamic Array - - -

// - - - Header
#ifndef JUST_DYN_ARRAY
#define JUST_DYN_ARRAY

/**
 * @file : dynamicArray.h 
 * @brief : Dynamic Array implementation in C
*/

#include <stdint.h>
#include <memory.h>

#ifdef __cplusplus 
extern "C" {
#endif 

#define JUST_DARRAY_DEFAULT_GROWTH_CAPACITY 8

/// @brief Dynamic Array : similar to std::vector in c++
typedef struct justDynamicArray
{
  uint8_t*                data;         ///< POinter to contiguois element memory 
  size_t                  capacity;     ///< Total number of elements allocated
  size_t                  size;         ///< Current number of elements stored
  size_t                  elementSize;  ///< Size of an individual element in bytes
  JustLinearAllocator*    allocator;    ///< Optional custom linear allocator, NULL for the vector to manage its own memory
  char                    tag[32];      ///< Why are you creating this (char[32])
} JustDynamicArray;


// - - - C API - - - 

/**
 * @brief : Creates a dynamic array instance.
 * @param DARRAY: Pointer to dynamic array struct 
 * @param INITIAL_CAPACITY : Initial element capacity (0 defaults to 8)
 * @param ELEMENT_SIZE : Size of each element in bytes 
 * @param ALLOCATOR : Pointer to linear allocator or NULL for the dynamic array to allocate memory on its own 
 * @warning : TAG is a char[32]
 * @return true if initialized successfully, false otherwise
*/
JUST_API bool justDynamicArrayCreate(
  JustDynamicArray*    DARRAY, 
  size_t               INITIAL_CAPACITY, 
  size_t               ELEMENT_SIZE, 
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG);

/**
 * @brief : Destroys the dynamic array and releases memory if owned. 
 * @param DARRAY : Pointer to the dynamic array to be destroyed
*/
JUST_API void justDynamicArrayDestroy(JustDynamicArray* DARRAY);

/**
 * @brief : Ensures capacity exists for at least MIN_CAPACITY elements.
 * @param DARRAY : The array to be reserved 
 * @param MIN_CAPACITY : How much to reserve as count of elements 
 * @return : True if successfull and false if not
*/
JUST_API bool justDynamicArrayReserve(JustDynamicArray* DARRAY, size_t MIN_CAPACITY);

/**
 * @brief : Internal slow-path growth function
 * @warning: Internal function
 * @param DARRAY : The array to be grown
 * @return : True if succesful and false if not
 */
bool __justDynamicArrayGrow(JustDynamicArray* DARRAY);

/**
 * @brief : Appends a contigous range of elements via a single block memcpy
 * @param DARRAY : The dynamic array to which the range is to be pushed.
 * @param SRC_BUFFER : Pointer to the source to be copied
 * @param COUNT : How many elements in the buffer
 * @return : True if succesful, false if not
 */
JUST_API bool justDynamicArrayPushRange(JustDynamicArray* DARRAY, const void* SRC_BUFFER, size_t COUNT);

/**
 * @brief : Reserves a slot at the end and returns a direct pointer to unitialized element memory. Enables zero-copy costruction directly into array storage, and bypasses memcpy
 * @param DARRAY : The dynamic array pointer
 * @return : Pointer to the unitialized element
 * @warning : Does not intialize the element, use the pointer to initialize
 */
JUST_API void* justDynamicArrayEmplace(JustDynamicArray* DARRAY);

/**
 * @brief : Pushes a new element value to the back of the array.
 * @param DARRAY : The dynamic array to which the value is to be pushed 
 * @param VALUE_PTR : Pointer to the value being stored
 * @warning : VALUE_PTR's value will be copied
 * @return : True if push was succesful, false if not 
*/
JUST_API static inline bool justDynamicArrayPush(JustDynamicArray* DARRAY, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot push into null DARRAY");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_PTR != NULL, "[DYNAMIC DARRAY] : Cannot push a null VALUE_PTR");

  void* slot = justDynamicArrayEmplace(DARRAY);
  if (!slot) return false;

  memcpy(slot, VALUE_PTR, DARRAY->elementSize);
  return true;
}

/**
 * @brief : Pops the last element from the array 
 * @param OUT_VALUE_PTR : Optional pointer to receive the popped element bytes
 * @warning : the size of OUT_VALUE_PTR should be big enough to store the element
 * @return : whether the pop was successfull
*/
JUST_API static inline bool justDynamicArrayPop(JustDynamicArray* DARRAY, void* OUT_VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot Pop from a NULL DARRAY");

  if (DARRAY->size == 0)
  {
    JUST_LOG_WARNING("[DYNAMIC DARRAY] : The size of the DARRAY is 0, cannot pop");
    return false;
  }

  DARRAY->size--;
  if (OUT_VALUE_PTR)
  {
    uint8_t* source = DARRAY->data + (DARRAY->size * DARRAY->elementSize);
    memcpy(OUT_VALUE_PTR, source, DARRAY->elementSize);
  }

  return true;
}

/**
 * @brief : Returns a pointer to the element at the given index 
 * @param DARRAY : Pointer to the array which is to be accessed
 * @param INDEX : The index to access at 
 * @warning : The index must be in bounds
 * @warning : Since this returns a void*, you can override it directly, but be careful, since you get access to the memory underneath
 * @return : A pointer to the object in the array at the given index
*/
JUST_API static inline void* justDynamicArrayAt(const JustDynamicArray* DARRAY, size_t INDEX)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot access a NULL DARRAY");
  JUST_ASSERT_DEBUG_MESSAGE(INDEX < DARRAY->size, "[DYNAMIC DARRAY] : INDEX out of bounds");

  return (void*) (DARRAY->data + (INDEX * DARRAY->elementSize));
}

/**
 * @brief : Clears all elements without freeing memory.
 * @param DARRAY : A pointer to the array to be cleared
*/
JUST_API static inline void justDynamicArrayClear(JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot clear a NULL DARRAY");
  DARRAY->size = 0;
}

/**
 * @brief : Gets the size of the array
 * @param DARRAY : Pointer to the dynamic array
 * @return : size of the array in terms of how many elements
 */
JUST_API static inline size_t justDynamicArraySize(const JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot size a NULL DARRAY");
  return DARRAY->size;
}

/**
 * @brief : Gets the capacity of the array
 * @param DARRAY : Pointer to the dynamic array
 * @return : capacity of the array in terms of how many elements
 */
JUST_API static inline size_t justDynamicArrayCapacity(const JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot check capacity of a NULL DARRAY");
  return DARRAY->capacity;
}

/**
 * @brief : Tells whether the array is empty
 * @param DARRAY : Pointer to the dynamic array
 * @return : true if empty, false otherwise
 */
JUST_API static inline bool justDynamicArrayIsEmpty(const JustDynamicArray* DARRAY)
{
  return (DARRAY->size == 0);
}

/**
 * @brief : Shrinks the array on demand
 * @param DARRAY : Pointer to the dynamic array to be shrunk
 * @return : True if the array shrunk, false otherwise
 */
JUST_API bool justDynamicArrayShrinkToFit(JustDynamicArray* DARRAY);


// - - - Helper Macros for Ergonomic Usage - - - 

/**
 * @brief : Helper macro to initialize array with implicit type sizing 
 * @see dynamicArrayCreate
*/
#define JUST_DARRAY_INIT_TAGGED(DARRAY_PTR, CAPACITY, TYPE, TAG) \
  justDynamicArrayCreate((DARRAY_PTR), (CAPACITY), sizeof(TYPE), NULL, (TAG))

/**
 * @brief : Helper macro to initialize array with implicit type sizing 
 * @see dynamicArrayCreate
*/
#define JUST_DARRAY_INIT(DARRAY_PTR, CAPACITY, TYPE) JUST_DARRAY_INIT_TAGGED(DARRAY_PTR, CAPACITY, TYPE, "DYNAMIC DARRAY")

/// @brief : View the dynamic array as a standard C array. Like dynamicArray to []
#define JUST_DARRAY_DATA(DARRAY_PTR, TYPE) \
  ((TYPE*) (DARRAY_PTR)->data)

/**
 * @brief : Type-safe get element macro
 * @see : dynamicArrayAt
*/
#define JUST_DARRAY_GET(DARRAY_PTR, TYPE, INDEX) \
  (JUST_DARRAY_DATA(DARRAY_PTR, TYPE)[INDEX])

/// @brief : Direct zero-copy typed emplace
#define JUST_DARRAY_EMPLACE(DARRAY_PTR, TYPE) \
  ((TYPE*) justDynamicArrayEmplace(DARRAY_PTR))

/// @brief : Fast-path push value
#define JUST_DARRAY_PUSH_VAL(DARRAY_PTR, TYPE, VALUE)     \
  do                                                      \
  {                                                       \
    TYPE* _slot = JUST_DARRAY_EMPLACE(DARRAY_PTR, TYPE);  \
    if (_slot) *_slot = (VALUE);                          \
  } while(0)

/// @brief : Batch range append
#define JUST_DARRAY_PUSH_RANGE(DARRAY_PTR, SRC_PTR, COUNT) \
  justDynamicArrayPushRange((DARRAY_PTR), (const void*)(SRC_PTR), (COUNT))


#ifdef __cplusplus
}
#endif

#endif // JUST_DYN_ARRAY

// - - - Implementation
#ifdef JUST_LIB_IMPL_DYN_ARRAY

#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool justDynamicArrayCreate(
  JustDynamicArray*    DARRAY,
  size_t               INITIAL_CAPACITY,
  size_t               ELEMENT_SIZE,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot create a NULL DARRAY");
  JUST_ASSERT_DEBUG_MESSAGE(ELEMENT_SIZE > 0, "[DYNAMIC DARRAY] : Element size must be greater than 0");

  DARRAY->data          = NULL;
  DARRAY->elementSize   = ELEMENT_SIZE;
  DARRAY->size          = 0;
  DARRAY->capacity      = INITIAL_CAPACITY;
  DARRAY->allocator     = ALLOCATOR;
  snprintf(DARRAY->tag, sizeof(DARRAY->tag), "%s", TAG);

  size_t totalBytes = DARRAY->capacity * DARRAY->elementSize;

  if (DARRAY->capacity == 0) return true;

  if (DARRAY->allocator) 
  {
    DARRAY->data = (uint8_t*) justLinearAllocAllocate(DARRAY->allocator, totalBytes, 0);
  }
  else 
  {
    DARRAY->data = (uint8_t*) JUST_MALLOC_TAGGED(totalBytes, TAG);
  }

  if (!DARRAY->data)
  {
    JUST_LOG_ERROR("[DYNAMIC DARRAY] : Failed to allocate memory for array!");
    DARRAY->capacity = 0;
    return false;
  }

  return true;
}

void justDynamicArrayDestroy(JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot destroy a NULL array.");

  if (DARRAY->data)
  {
    if (!DARRAY->allocator) JUST_FREE(DARRAY->data);
  }

  DARRAY->data         = NULL;
  DARRAY->capacity     = 0;
  DARRAY->size         = 0;
  DARRAY->elementSize  = 0;
  DARRAY->allocator    = NULL;
}

bool justDynamicArrayReserve(JustDynamicArray* DARRAY, size_t MIN_CAPACITY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot reserve capcity in a NULL DARRAY");

  if (MIN_CAPACITY <= DARRAY->capacity) return true;

  size_t newCapacity = DARRAY->capacity * 2;
  if (newCapacity < MIN_CAPACITY) newCapacity = MIN_CAPACITY;

  size_t    newBytes  = newCapacity * DARRAY->elementSize;
  uint8_t*  newData   = NULL;

  // - - - allocate new chunk from Linear Allocator and copy existing data
  if (DARRAY->allocator)
  {
    newData = (uint8_t*) justLinearAllocAllocate(DARRAY->allocator, newBytes, 0);
    if (!newData)
    {
      JUST_LOG_ERROR("[DYNAMIC DARRAY] : Failed to allocate new chunks via Linear Allocator");
      return false;
    }
    memcpy(newData, DARRAY->data, DARRAY->size * DARRAY->elementSize);
  }
  else
  {
    if (DARRAY->data == NULL) newData = (uint8_t*) JUST_MALLOC_TAGGED(newBytes, DARRAY->tag);
    else                      newData = (uint8_t*) JUST_REALLOC(DARRAY->data, newBytes);
  }

  if (!newData)
  {
    JUST_LOG_ERROR("[DYNAMIC DARRAY] : Failed to expenad array capacity!");
    return false;
  }

  DARRAY->data     = newData;
  DARRAY->capacity = newCapacity;
  return true;
}

bool __justDynamicArrayGrow(JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG(DARRAY != NULL);

  size_t targetCap = DARRAY->capacity ? (DARRAY->capacity * 2) : JUST_DARRAY_DEFAULT_GROWTH_CAPACITY;
  return justDynamicArrayReserve(DARRAY, targetCap);
}

bool justDynamicArrayPushRange(JustDynamicArray* DARRAY, const void* SRC_BUFFER, size_t COUNT)
{
  JUST_ASSERT_DEBUG(DARRAY != NULL);
  if (!SRC_BUFFER || COUNT == 0) return true;

  size_t requiredCapacity = DARRAY->size + COUNT;
  if (requiredCapacity > DARRAY->capacity)
  {
    if (!justDynamicArrayReserve(DARRAY, requiredCapacity)) 
    {
      return false;
    }
  }

  uint8_t* dest = DARRAY->data + (DARRAY->size * DARRAY->elementSize);
  memcpy(dest, SRC_BUFFER, COUNT * DARRAY->elementSize);
  DARRAY->size += COUNT;

  return true;
}

bool justDynamicArrayShrinkToFit(JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot shrink a NULL DARRAY");

  // - - - Linear allocators cannot free or shrink intermediate allocations
  if (DARRAY->allocator) 
  {
    JUST_LOG_ERROR("[DYNAMIC DARRAY] : Cannot shrink an DARRAY that uses a linear allocator");
    return false;
  }

  if (DARRAY->size == DARRAY->capacity) return true;

  if (DARRAY->size == 0)
  {
    if (DARRAY->data)
    {
      JUST_FREE(DARRAY->data);
      DARRAY->data = NULL;
    }
    DARRAY->capacity = 0;
    return true;
  }

  size_t  newBytes  = DARRAY->size * DARRAY->elementSize;
  void*   newData   = NULL;
  if (DARRAY->data)  newData = JUST_REALLOC(DARRAY->data, newBytes);
  else              newData = JUST_MALLOC_TAGGED(newBytes, DARRAY->tag);

  if (!newData)
  {
    JUST_LOG_ERROR("[DYNAMIC DARRAY] : Failed to shrink array buffer!");
    return false;
  }

  DARRAY->data     = (uint8_t*) newData;
  DARRAY->capacity = DARRAY->size;
  return true;
}

JUST_API void* justDynamicArrayEmplace(JustDynamicArray* DARRAY)
{
  JUST_ASSERT_DEBUG_MESSAGE(DARRAY != NULL, "[DYNAMIC DARRAY] : Cannot emplace in a NULL array");

  if (DARRAY->size >= DARRAY->capacity)
  {
    if (!__justDynamicArrayGrow(DARRAY)) return NULL;
  }

  void* slot = DARRAY->data + (DARRAY->size * DARRAY->elementSize);
  DARRAY->size++;
  return slot;
}

#endif // JUST_LIB_IMPL_DYN_ARRAY


// - - - STACK (Header only) - - - 

#ifndef JUST_STACK
#define JUST_STACK

#ifdef JUST_LIB_IMPL_STACK
/*
 * @file : stack.h 
 * @brief : Stack implementation using dynamicArray 
 * @see : dynamicArray.h
 */


#ifdef __cplusplus
  extern "C" {
#endif

/// @brief : Stack just has an underlying dynamic Array 
typedef struct justStack 
{
  JustDynamicArray array; ///< The underlying dynamic Array
} JustStack;

/**
 * @brief : Initializes a stack instance.
 * @param STACK : The stack to create 
 * @param INITIAL_CAPACITY : The initial capacity of the stack 
 * @param ELEMENT_SIZE : The size of one element in the stack 
 * @param ALLOCATOR : Optional linear allocator
 * @return : True if successful, false if not
*/
JUST_API static inline bool justStackCreate(
  JustStack*           STACK,
  size_t               INITIAL_CAPACITY,
  size_t               ELEMENT_SIZE,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot create a NULL STACK");
  JUST_ASSERT_DEBUG_MESSAGE(ELEMENT_SIZE > 0, "[STACK] : Element size must be greater than 0");

  return justDynamicArrayCreate(&STACK->array, INITIAL_CAPACITY, ELEMENT_SIZE, ALLOCATOR, TAG);
}

/**
 * @brief : Destroys the stack and frees internal resources.
 * @param STACK : The stack to destroy 
 */
JUST_API static inline void justStackDestroy(JustStack* STACK) 
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot destroy a NULL STACK");
  justDynamicArrayDestroy(&STACK->array); 
}

/**
 * @brief : Pushes an element onto the top of the stack (O(1)).
 * @param STACK : The stack to push into 
 * @param VALUE_PTR : The value to push
 * @return : True if successful, false if not
*/
JUST_API static inline bool justStackPush(JustStack* STACK, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot push to a NULL stack");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_PTR != NULL, "[STACK] : Cannot push a NULL VALUE_PTR to a stack");

  return justDynamicArrayPush(&STACK->array, VALUE_PTR);
}

/**
 * @brief : Reserves a slot at the top and returns a direct pointer to uninitialized element memory.
 * @param STACK : The stack to be emplaced
 * @return : Pointer to uninitialized data
 * @warning: The element is uninitialized, use the pointer to Initialize
 */
JUST_API static inline void* justStackEmplace(JustStack* STACK)
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot emplace in a NULL STACK");

  return justDynamicArrayEmplace(&STACK->array);
}

/**
 * @brief : Pops the top element from the stack (O(1)).
 * @param STACK : The stack to pop from 
 * @param OUT_VALUE_PTR : Optional pointer to store the value in
 * @return : True if successful, False if not
 */
JUST_API static inline bool justStackPop(JustStack* STACK, void* OUT_VALUE_PTR) 
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot pop from a NULL ARRAY");

  if (STACK->array.size == 0)
  {
    JUST_LOG_WARNING("[STACK] : Cannot pop, stack is empty");
    return false;
  }

  return justDynamicArrayPop(&STACK->array, OUT_VALUE_PTR);
}

/**
 * @brief : Views the top element without removing it.
 * @param STACK : The stack to peek from 
 * @return : Pointer to the top of the stack
 */
JUST_API static inline void* justStackPeek(const JustStack* STACK) 
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot peek into a NULL STACK");

  if (STACK->array.size == 0) return NULL;
  return justDynamicArrayAt(&STACK->array, STACK->array.size - 1);
}

/**
 * @brief : Returns current element count.
 * @param STACK : The stack whose size is to be known
 * @return : how many elements in the stack
 */
JUST_API static inline size_t justStackSize(const JustStack* STACK) 
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot check size of a NULL STACK");
  return STACK->array.size; 
}

/**
 * @brief : Checks if stack is empty.
 * @param STACK : The stack to check 
 * @return : True if stack empty, False if not
 */
JUST_API static inline bool justStackIsEmpty(const JustStack* STACK) 
{
  JUST_ASSERT_DEBUG_MESSAGE(STACK != NULL, "[STACK] : Cannot check if a NULL STACK is empty");
  return STACK->array.size == 0; 
}


// - - - Ergonomic & Type-Safe Macros - - -

/// @brief Direct creation of stack from type
#define JUST_STACK_INIT_TAGGED(STACK_PTR, CAPACITY, TYPE, TAG) \
  justStackCreate((STACK_PTR), (CAPACITY), sizeof(TYPE), (NULL), (TAG))

/// @brief Stack creation but without a tag
#define JUST_STACK_INIT(STACK_PTR, CAPACITY, TYPE) JUST_STACK_INIT_TAGGED(STACK_PTR, CAPACITY, TYPE, "STACK")

/// @brief Direct typed top element inspection: *JUST_STACK_TOP(s, MyType)
#define JUST_STACK_TOP(STACK_PTR, TYPE) \
  (&((TYPE*)(STACK_PTR)->array.data)[(STACK_PTR)->array.size - 1])

/// @brief Zero-copy emplace onto stack
#define JUST_STACK_EMPLACE(STACK_PTR, TYPE) \
  ((TYPE*) justStackEmplace(STACK_PTR))

/// @brief Type-safe push by value
#define JUST_STACK_PUSH_VAL(STACK_PTR, TYPE, VALUE) \
  JUST_DARRAY_PUSH_VAL(&(STACK_PTR)->array, TYPE, (VALUE))

#ifdef __cplusplus
}
#endif

#endif // JUST_LIB_IMPL_STACK
#endif // JUST_STACK


// - - - Queue - - -

// - - - HEADER
#ifndef JUST_KYU
#define JUST_KYU

/**
 * @file : queue.h 
 * @brief : Queue implementation using dynamic Array 
 */

#include <stdint.h>

#define JUST_QUEUE_DEFAULT_GROWTH_CAPACITY 8

#ifdef __cplusplus
extern "C" {
#endif



/// @brief : The queue struct
typedef struct justQueue
{
  uint8_t*              data;         ///< Contigous element storage
  size_t                capacity;     ///< Allocated capacity (always a power of 2)
  size_t                mask;         ///< capacity - 1 for bitwise modulo
  size_t                size;         ///< Current number of active elements
  size_t                head;         ///< Index of oldest element
  size_t                tail;         ///< Next write index
  size_t                elementSize;  ///< sizeof(T)
  JustLinearAllocator*  allocator;    ///< Optional linear allocator, NULL for system heap
  char                  tag[32];      ///< WHy create this queue
} JustQueue;

/**
 * @brief : Creates a queue instance.
 * @param QUEUE : A pointer to the Queue to be created 
 * @param INITIAL_CAPACITY : How many elements at the start
 * @param ELEMENT_SIZE : What is the size of an element 
 * @param ALLOCATOR : Optional Linear allocator
 * @param TAG : Optional why create this queue
 * @return : True if successful, False if not
 */
JUST_API bool justQueueCreate(
  JustQueue*            QUEUE, 
  size_t                INITIAL_CAPACITY,
  size_t                ELEMENT_SIZE,
  JustLinearAllocator*  ALLOCATOR,
  const char*           TAG);

/**
 * @brief : Destroys the queue.
 * @param QUEUE : Pointer to the queue to be destroyed
*/
JUST_API void justQueueDestroy(JustQueue* QUEUE);

/**
 * @brief : Sets queue size, growth and shrink both
 * @param QUEUE : The queue to be grown or shrunk
 * @param TARGET_CAPACITY : The new size of the queue
 * @return : True if succesful and false if not
 */
JUST_API bool justQueueReserve(JustQueue* QUEUE, size_t TARGET_CAPACITY);

/**
 * @brief : Reserves a slot at the tail and returns a pointer for direct
 * @param QUEUE : The queue to be emplaced
 * @return : Pointer to the unitialized element 
 * @warning : The element is not initialized, use the pointer to initialize it
 */
JUST_API static inline void* justQueueEmplace(JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot emplace in a NULL QUEUE");

  if (QUEUE->size >= QUEUE->capacity)
  {
    size_t newCap = QUEUE->capacity ? (QUEUE->capacity * 2) : JUST_QUEUE_DEFAULT_GROWTH_CAPACITY;
    if (!justQueueReserve(QUEUE, newCap)) return NULL;
  }

  void* slot  = (uint8_t*) QUEUE->data + (QUEUE->tail * QUEUE->elementSize);
  QUEUE->tail = (QUEUE->tail + 1) & QUEUE->mask;
  QUEUE->size++;
  return slot;
}

/**
 * @brief : Enqueues an item to the back of the queue (O(1)).
 * @param QUEUE : A pointer to the queue 
 * @param VALUE_PTR : Pointer to the value to be enqueued 
 * @return : True if successful, False if not
 */
JUST_API static inline bool justQueueEnqueue(JustQueue* QUEUE, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot enqueue to a NULL QUEUE");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_PTR != NULL, "[QUEUE] : Cannot enqueue a NULL VALUE_PTR");

  void* slot = justQueueEmplace(QUEUE);
  if (!slot) return false;

  memcpy(slot, VALUE_PTR, QUEUE->elementSize);
  return true;
}

/**
 * @brief : Dequeues an item from the front of the queue in O(1) time.
 * @param QUEUE : A pointer to the queue 
 * @param OUT_VALUE_PTR : Optional pointer to store the dequeued value 
 * @return : True if successful, False if not
 */
JUST_API static inline bool justQueueDequeue(JustQueue* QUEUE, void* OUT_VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot dequeue from a NULL QUEUE");

  if (QUEUE->size == 0) return false;

  if (OUT_VALUE_PTR)
  {
    void* slot = (uint8_t*)QUEUE->data + (QUEUE->head * QUEUE->elementSize);
    memcpy(OUT_VALUE_PTR, slot, QUEUE->elementSize);
  }

  QUEUE->head = (QUEUE->head + 1) & QUEUE->mask;
  QUEUE->size--;

  return true;
}

/**
 * @brief : Returns pointer to item at the front without dequeuing.
 * @param QUEUE : The queue to peek from 
 * @return : Pointer to the head variable of the queue
 */
JUST_API static inline void* justQueuePeek(const JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot peek into a NULL QUEUE");

  if (QUEUE->size == 0) return NULL;
  return (void*) ((uint8_t*)QUEUE->data + (QUEUE->head * QUEUE->elementSize));
}

/**
 * @brief : Returns active item count in queue.
 * @param QUEUE : Pointer to the queue whose size is to be measured 
 * @return : The size of the queue
 */
JUST_API static inline size_t justQueueSize(const JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot check size of a NULL QUEUE");

  return QUEUE->size;
}

/**
 * @brief : Returns capacity of queue.
 * @param QUEUE : Pointer to the queu
 * @return : The capacity of the queue
 */
JUST_API static inline size_t justQueueCapacity(const JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot check capacity of a NULL QUEUE");

  return QUEUE->capacity;
}

/**
 * @brief : Returns whether the queue is emptu
 * @param QUEUE : Pointer to the queu
 * @return : true if the queue is empty, false otherwise
 */
JUST_API static inline size_t justQueueIsEmpty(const JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot check if a NULL QUEUE is empty");

  return (QUEUE->size == 0);
}

/**
 * @brief : Clears a queue 
 * @param QUEUE : The queue to be cleared
*/
JUST_API static inline void justQueueClear(JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot clear a NULL QUEUE");

  QUEUE->size = 0;
  QUEUE->head = 0;
  QUEUE->tail = 0;
}

/** 
 * @brief : Trims capacity to the smallest power of two that fits current size 
 * @param QUEUE : The queue to be shrunk
 * @return : True if shrunk, false otherwise
*/
JUST_API static inline bool justQueueShrinkToFit(JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot shrink a NULL QUEUE");

  return justQueueReserve(QUEUE, QUEUE->size);
}


// - - - Ergonomic Macros - - -

#define JUST_QUEUE_INIT_TAGGED(QUEUE_PTR, CAPACITY, TYPE, TAG) \
  justQueueCreate((QUEUE_PTR), (CAPACITY), sizeof(TYPE), NULL, (TAG))

#define JUST_QUEUE_INIT(QUEUE_PTR, CAPACITY, TYPE) JUST_QUEUE_INIT_TAGGED(QUEUE_PTR, CAPACITY, TYPE, "QUEUE")

#define JUST_QUEUE_EMPLACE(QUEUE_PTR, TYPE) \
  ((TYPE*) justQueueEmplace(QUEUE_PTR))

#define JUST_QUEUE_PEEK(QUEUE_PTR, TYPE) \
  ((TYPE*) justQueuePeek(QUEUE_PTR))

#define JUST_QUEUE_ENQUEUE_VAL(QUEUE_PTR, TYPE, VALUE)  \
  do                                                    \
  {                                                     \
    TYPE  _val  = (TYPE) VALUE;                         \
    TYPE* _slot = JUST_QUEUE_EMPLACE(QUEUE_PTR, TYPE);  \
    if (_slot) *_slot = _val;                           \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif // JUST_KYU

// - - - Implementation
#ifdef JUST_LIB_IMPL_QUEUE

#include <stdint.h>
#include <string.h>
#include <stdio.h>

static inline size_t justQueueRoundToPowerOfTwo(size_t n)
{
  if (n == 0) return 0;
  if (n < JUST_QUEUE_DEFAULT_GROWTH_CAPACITY) return JUST_QUEUE_DEFAULT_GROWTH_CAPACITY;
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
#if UINTPTR_MAX > 0xFFFFFFFF
  n |= n >> 32;
#endif
  n++;
  return n;
}

JUST_API bool justQueueCreate(
  JustQueue*           QUEUE,
  size_t                INITIAL_CAPACITY,
  size_t                ELEMENT_SIZE,
  JustLinearAllocator* ALLOCATOR,
  const char*           TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot create a NULL queue");
  JUST_ASSERT_DEBUG_MESSAGE(ELEMENT_SIZE > 0, "[QUEUE] : Cannot create a queue with 0 ELEMENT_SIZE");

  QUEUE->head         = 0;
  QUEUE->tail         = 0;
  QUEUE->size         = 0;
  QUEUE->capacity     = justQueueRoundToPowerOfTwo(INITIAL_CAPACITY);
  QUEUE->elementSize  = ELEMENT_SIZE;
  QUEUE->mask         = QUEUE->capacity - 1;
  QUEUE->allocator    = ALLOCATOR;
  QUEUE->data         = NULL;
  snprintf(QUEUE->tag, sizeof(QUEUE->tag), "%s", TAG);

  if (QUEUE->capacity > 0)
  {
    size_t totalBytes = QUEUE->capacity * QUEUE->elementSize;

    if (QUEUE->allocator)
    {
      QUEUE->data = (uint8_t*)justLinearAllocAllocate(QUEUE->allocator, totalBytes, 0);
    }
    else
    {
      QUEUE->data = (uint8_t*) JUST_MALLOC_TAGGED(totalBytes, QUEUE->tag);
    }

    if (!QUEUE->data)
    {
      JUST_LOG_ERROR("[QUEUE] : Failed to allocate initial buffer for %zu slots", QUEUE->capacity);
      QUEUE->capacity = 0;
      QUEUE->mask     = 0;
      return false;
    }
  }

  return true;
}

JUST_API void justQueueDestroy(JustQueue* QUEUE)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot destroy a NULL queue");

  if (QUEUE->data && !QUEUE->allocator)
  {
    JUST_FREE(QUEUE->data);
  }

  QUEUE->data = NULL;
  QUEUE->capacity    = 0;
  QUEUE->mask        = 0;
  QUEUE->size        = 0;
  QUEUE->head        = 0;
  QUEUE->tail        = 0;
  QUEUE->elementSize = 0;
  QUEUE->allocator   = NULL;
}

JUST_API bool justQueueReserve(JustQueue* QUEUE, size_t TARGET_CAPACITY)
{
  JUST_ASSERT_DEBUG_MESSAGE(QUEUE != NULL, "[QUEUE] : Cannot reserve size in a NULL QUEUE");

  size_t newCap = justQueueRoundToPowerOfTwo(TARGET_CAPACITY);

  if (newCap < QUEUE->size)                             newCap = justQueueRoundToPowerOfTwo(QUEUE->size);
  if (newCap == QUEUE->capacity && QUEUE->data != NULL) return true;

  if (newCap == 0)
  {
    if (QUEUE->data && !QUEUE->allocator)
    {
      if (QUEUE->data && !QUEUE->allocator) JUST_FREE(QUEUE->data);
    }

    QUEUE->data     = NULL;
    QUEUE->capacity = 0;
    QUEUE->mask     = 0;
    QUEUE->head     = 0;
    QUEUE->tail     = 0;

    return true;
  }

  size_t    totalBytes  = newCap * QUEUE->elementSize;
  uint8_t*  newData     = QUEUE->allocator
                          ? (uint8_t*) justLinearAllocAllocate(QUEUE->allocator, totalBytes, 0)
                          : (uint8_t*) JUST_MALLOC_TAGGED(totalBytes, QUEUE->tag);

  if (!newData)
  {
    JUST_LOG_ERROR("[QUEUE] : Failed to allcoate buffer for capacity %zu", newCap);
    return false;
  }

  // - - - Linearize existing ring buffer into [0 ... size]
  if (QUEUE->data && QUEUE->size > 0)
  {
    size_t firstPartCount = QUEUE->capacity - QUEUE->head;
    if (QUEUE->size <= firstPartCount)
    {
      memcpy(newData, QUEUE->data + (QUEUE->head * QUEUE->elementSize), QUEUE->size * QUEUE->elementSize);
    }
    else
    {
      size_t firstPartBytes  = firstPartCount * QUEUE->elementSize;
      size_t secondPartBytes = (QUEUE->size - firstPartCount) * QUEUE->elementSize;

      memcpy(newData, QUEUE->data + (QUEUE->head * QUEUE->elementSize), firstPartBytes);
      memcpy(newData + firstPartBytes, QUEUE->data, secondPartBytes);
    }

    if (!QUEUE->allocator)
    {
      JUST_FREE(QUEUE->data);
    }
  }

  QUEUE->data     = newData;
  QUEUE->head     = 0;
  QUEUE->tail     = QUEUE->size;
  QUEUE->capacity = newCap;
  QUEUE->mask     = newCap - 1;

  return true;
}
#endif // JUST_LIB_IMPL_QUEUE


// - - - Ring Buffer - - -

// - - - HEADER 
#ifndef JUST_RING
#define JUST_RING

/**
 * @file : ringBuffer.h 
 * @brief : Cache-friendly power-of-two circular ring buffer
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @brief : Ring Buffer structure
typedef struct justRingBuffer 
{
  uint8_t*              data;           ///< Contiguous ring memory
  size_t                capacity;       ///< Total element capacity (power of 2)
  size_t                mask;           ///< capacity - 1 for fast bitwise modulo
  size_t                elementSize;    ///< Size of each element in bytes
  size_t                head;           ///< Next write index
  size_t                tail;           ///< Next read index
  size_t                count;          ///< Active item count
  bool                  allowOverwrite; ///< Overwrite oldest item when full
  JustLinearAllocator*  allocator;      ///< Optional linear allocator (NULL for heap)
  char                  tag[32];        ///< Why allocating this
} JustRingBuffer;

/**
 * @brief : Initializes a fixed-capacity Ring Buffer.
 * @param RING : Pointer to RingBuffer struct.
 * @param CAPACITY : Maximum element count (rounded up to nearest power of 2).
 * @param ELEMENT_SIZE : Size of an individual element in bytes.
 * @param ALLOW_OVERWRITE : If true, pushing to a full buffer drops the oldest item.
 * @param ALLOCATOR : Optional linear allocator, or NULL for managed heap.
 * @param TAG : Tracking tag for memory tracker.
 * @return : true on success, false on allocation failure.
 */
JUST_API bool justRingBufferCreate(
  JustRingBuffer*      RING,
  size_t               CAPACITY,
  size_t               ELEMENT_SIZE,
  bool                 ALLOW_OVERWRITE,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG);

JUST_API void justRingBufferDestroy(JustRingBuffer* RING);


/**
 * @brief : Reserves a slot at the head for writing and returns its pointer.
 * @param RING : Point to the ring buffer
 * @warning : This does not initialize the element
 * @return : pointer to the uniitialized element
 */
JUST_API static inline void* justRingBufferEmplace(JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot emplace in a NULL RING buffer");

  if (RING->count == RING->capacity)
  {
    if (!RING->allowOverwrite)
    {
      return NULL;
    }

    // - - - Overwrite oldest item: advance tail
    RING->tail = (RING->tail + 1) & RING->mask;
    RING->count--;
  }

  void* slot = RING->data + (RING->head * RING->elementSize);
  RING->head = (RING->head + 1) & RING->mask;
  RING->count++;

  return slot;
}

/**
 * @brief : Writes/pushes an element into the ring buffer.
 * @param RING : Pointer to the ring buffer
 * @param ITEM_PTR : Pointer to the item to be pushed
 * @return : true if success, false otherwise
 */
JUST_API static inline bool justRingBufferPush(JustRingBuffer* RING, const void* ITEM_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot push to a NULL RING");
  JUST_ASSERT_DEBUG_MESSAGE(ITEM_PTR != NULL, "[RING BUFFER] : Cannot push a NULL ITEM_PTR");

  void* slot = justRingBufferEmplace(RING);
  if (!slot) return false;

  memcpy(slot, ITEM_PTR, RING->elementSize);
  return true;
}

/**
 * @brief : Reads/pops an element from the ring buffer.
 * @param RING : Pointer to the ring buffer
 * @param OUT_ITEM_PTR : Optional pointer to store the popped element
 * @return : true if success, false otherwise
 */
JUST_API static inline bool justRingBufferPop(JustRingBuffer* RING, void* OUT_ITEM_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot pop from a NULL RING");

  if (RING->count == 0) return false;

  if (OUT_ITEM_PTR)
  {
    const void* source = RING->data + (RING->tail * RING->elementSize);
    memcpy(OUT_ITEM_PTR, source, RING->elementSize);
  }

  RING->tail = (RING->tail + 1) & RING->mask;
  RING->count--;

  return true;
}

/**
 * @brief : Peeks at the oldest element without removing it. Returns NULL if empty.
 * @param RING : Pointer to the ring buffer
 * @return : Pointer to the oldest element
 */
JUST_API static inline void* justRingBufferPeek(const JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot peek in a NULL RING");

  if (RING->count == 0) return NULL;
  return (void*)(RING->data + (RING->tail * RING->elementSize));
}

/**
 * @brief : Clears the entire buffer
 * @param RING : Pointer to the ring buffer
*/
JUST_API static inline void justRingBufferClear(JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot clear a NULL RING");

  RING->head  = 0;
  RING->tail  = 0;
  RING->count = 0;
}

/**
 * @brief : Tells whether the buffer is empty
 * @param RING : Pointer to the ring
 * @return : true if the buffer is empty
*/
JUST_API static inline bool justRingBufferIsEmpty(const JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot check if a NULL RING is empty");
  return (RING->count == 0); 
}

/**
 * @brief : Tells whether the buffer is full
 * @param RING : Pointer to the ring
 * @return : true if the buffer is full
*/
JUST_API static inline bool justRingBufferIsFull(const JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot check if a NULL RING is full");

  return (RING->count == RING->capacity); 
}

/**
 * @brief : Tells the size of a buffer
 * @param RING : POinter to the ring buffer
 * @return : The size of the buffer in elements 
*/
JUST_API static inline size_t justRingBufferSize(const JustRingBuffer* RING) 
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot check size of a RING");

  return RING->count; 
}

/**
 * @brief : Tells the capacity of a buffer
 * @param RING : Pointer to the ring buffer
 * @return : The capacity of the buffer in elements
*/
JUST_API static inline size_t justRingBufferCapacity(const JustRingBuffer* RING) 
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot check capacity of a RING");

  return RING->capacity; 
}


// - - - Ergonomic Macros - - -

#define JUST_RING_INIT(RING_PTR, CAPACITY, TYPE, ALLOW_OVERWRITE) \
  justRingBufferCreate((RING_PTR), (CAPACITY), sizeof(TYPE), (ALLOW_OVERWRITE), NULL, "RING BUFFER")

#define JUST_RING_INIT_TAGGED(RING_PTR, CAPACITY, TYPE, ALLOW_OVERWRITE, TAG) \
  justRingBufferCreate((RING_PTR), (CAPACITY), sizeof(TYPE), (ALLOW_OVERWRITE), NULL, (TAG))

#define JUST_RING_EMPLACE(RING_PTR, TYPE) \
  ((TYPE*)justRingBufferEmplace(RING_PTR))

#define JUST_RING_PEEK(RING_PTR, TYPE) \
  ((TYPE*)justRingBufferPeek(RING_PTR))

#define JUST_RING_PUSH_VAL(RING_PTR, TYPE, VALUE)         \
  do                                                      \
  {                                                       \
    TYPE  _val  = (VALUE);                                \
    TYPE* _slot = (TYPE*)justRingBufferEmplace(RING_PTR); \
    if (_slot) *_slot = _val;                             \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif // JUST_RING

// - - - Implementation
#ifdef JUST_LIB_IMPL_RING_BUFF

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static inline size_t justRingNextPowerOfTwo(size_t NUM)
{
  if (NUM <= 1) return 1;
  NUM--;
  NUM |= NUM >> 1;
  NUM |= NUM >> 2;
  NUM |= NUM >> 4;
  NUM |= NUM >> 8;
  NUM |= NUM >> 16;
#if UINTPTR_MAX > 0xFFFFFFFF
  NUM |= NUM >> 32;
#endif
  NUM++;
  return NUM;
}

JUST_API bool justRingBufferCreate(
  JustRingBuffer*      RING,
  size_t               CAPACITY,
  size_t               ELEMENT_SIZE,
  bool                 ALLOW_OVERWRITE,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Target pointer cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(CAPACITY > 0, "[RING BUFFER] : CAPACITY must be > 0");
  JUST_ASSERT_DEBUG_MESSAGE(ELEMENT_SIZE > 0, "[RING BUFFER] : Element size must be > 0");

  RING->capacity       = justRingNextPowerOfTwo(CAPACITY);
  RING->mask           = RING->capacity - 1; 
  RING->elementSize    = ELEMENT_SIZE;
  RING->head           = 0;
  RING->tail           = 0;
  RING->count          = 0;
  RING->allowOverwrite = ALLOW_OVERWRITE;
  RING->allocator      = ALLOCATOR;
  snprintf(RING->tag, sizeof(RING->tag), "%s", TAG);

  size_t totalBytes = RING->capacity * RING->elementSize;

  if (RING->allocator)
  {
    RING->data = (uint8_t*) justLinearAllocAllocate(RING->allocator, totalBytes, 0);
  }
  else
  {
    RING->data = (uint8_t*) JUST_MALLOC_TAGGED(totalBytes, RING->tag);
  }

  if (!RING->data)
  {
    JUST_LOG_ERROR("[RING BUFFER] : Failed to allocate %zu bytes for ring buffer", totalBytes);
    RING->capacity = 0;
    RING->mask     = 0;
    return false;
  }

  return true;
}

JUST_API void justRingBufferDestroy(JustRingBuffer* RING)
{
  JUST_ASSERT_DEBUG_MESSAGE(RING != NULL, "[RING BUFFER] : Cannot destroy a NULL RING");

  if (RING->data && !RING->allocator)
  {
    JUST_FREE(RING->data);
  }

  RING->data        = NULL;
  RING->capacity    = 0;
  RING->mask        = 0;
  RING->elementSize = 0;
  RING->head        = 0;
  RING->tail        = 0;
  RING->count       = 0;
  RING->allocator   = NULL;
}
#endif // JUST_LIB_IMPL_RING_BUFF


// - - - Ordered Set - - -

// - - - HEADER
#ifndef JUST_ORDER_SET
#define JUST_ORDER_SET

/**
 * @file : orderedSet.h 
 * @brief : Ordered set implementation using a self-balancing AVL tree
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : A normal compare function, that checks which of the two elements is greater
 * @param A : One item
 * @param B : Another item
 * @param SIZE : Size of the items (for memcmp in default mostly)
 * @return : 1 if A is greater, -1 is B is greater, 0 if equal
 */
typedef int32_t (*justCompareFunc)(const void* A,      const void* B, size_t SIZE);

/**
 * @brief : A visitor function / iterator
 * @param VALUE :
 * @param USER_DATA : Optional parameter to call along the iteration, although not const, just lib does not touch it
 */
typedef void    (*justVisitorFunc)(const void* VALUE,  void* USER_DATA);


/// @brief : Structure of an AVL tree, a NODE
typedef struct justAVLNode
{
  struct justAVLNode*          left;   ///< Left child
  struct justAVLNode*          right;  ///< Right child
  int32_t                      height; ///< height of th etree
  alignas(max_align_t) uint8_t data[]; ///< data as an array, to ensure that it is on the same memory location as the entire struct, that data* does not give me
} JustAVLNode;

/// @brief : Structure of an AVL Trree, the view of the entire tree
typedef struct justAVLTree
{
  JustAVLNode*          root;         ///< The root node
  size_t                size;         ///< How many nodes
  size_t                elementSize;  ///< How big is an element in bytes
  justCompareFunc       compare;      ///< Compare function
  JustLinearAllocator*  allocator;    ///< Optional linear allocator
  char                  tag[32];      ///< Why was this tree created
} JustAVLTree;

typedef JustAVLTree OrderedSet;

/**
 * @brief : Initializes an AVL Tree (Ordered Set).
 * @param TREE : Pointer to AVLTree struct.
 * @param ELEMENT_SIZE : Size of each element in bytes.
 * @param COMPARATOR : Comparison callback (or NULL for default memcmp).
 * @param ALLOCATOR : Optional linear allocator (NULL for heap).
 * @param TAG : Tracking tag for memory tracker.
 * @return : true on success, false on allocation failure.
 */
JUST_API bool justOrderedSetCreate(
  JustAVLTree*         TREE,
  size_t                ELEMENT_SIZE,
  justCompareFunc      COMPARATOR,
  JustLinearAllocator* ALLOCATOR,
  const char*           TAG);

/**
 * @brief : Destroys and cleans up a tree
 * @param TREE : Pointer to the tree
*/
JUST_API void justOrderedSetDestroy(JustAVLTree* TREE);

/**
 * @param : Inserts a value to the orderedSet
 * @param TREE : Pointer to the orderedSet
 * @param VALUE_PTR : Pointer to the value to be inserted
 * @return : true if inserted, false otherwise
*/
JUST_API bool justOrderedSetInsert(JustAVLTree* TREE, const void* VALUE_PTR);

/**
 * @param : Removes a value from the orderedSet
 * @param TREE : Pointer to the orderedSet
 * @param VALUE_PTR : Pointer to the value to be removed
 * @return : true if removed, false otherwise
*/
JUST_API bool justOrderedSetRemove(JustAVLTree* TREE, const void* VALUE_PTR);

/**
 * @param : Inserts a value to the orderedSet
 * @param TREE : Pointer to the orderedSet
 * @param VALUE_PTR : Pointer to the value to be inserted
 * @return : true if inserted, false otherwise
*/
JUST_API void* justOrderedSetFind(const JustAVLTree* TREE, const void* VALUE_PTR);

/**
 * @param : Traverses a tree in order
 * @param TREE : Pointer to the orderedSet
 * @param VISITOR : Pointer to the visitor function
 * @param USER_DATA : Optional user data to be used while visiting, although not const, the library does not touch it
*/
JUST_API void justOrderedSetTraverseInorder(const JustAVLTree* TREE, justVisitorFunc VISITOR, void* USER_DATA);

/**
 * @brief : Clear out an orderedSet
 * @param TREE : Pointer to the tree
*/
JUST_API void justOrderedSetClear(JustAVLTree* TREE);

/**
 * @brief : Tells whether the set contains an item
 * @param TREE : Pointer to the set
 * @param VALUE_PTR : Pointer to the item to search
 * return : true if the item is in the set, false otherwise
*/
JUST_API static inline bool justOrderedSetContains(const JustAVLTree* TREE, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(TREE != NULL, "[ORDERED SET] : TREE cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_PTR != NULL, "[ORDERED SET] : VALUE_PTR cannot be NULL");

  return (justOrderedSetFind(TREE, VALUE_PTR) != NULL);
}

/**
 * @brief : Tells the size of an orderedSet
 * @param TREE : Pointer to the tree whose size is to be checked
 * @return : size in terms of number of nodes
*/
JUST_API static inline size_t justOrderedSetSize(const JustAVLTree* TREE)
{
  JUST_ASSERT_DEBUG_MESSAGE(TREE != NULL, "[ORDERED SET] : TREE cannot be NULL for checking size");

  return TREE->size;
}

/**
 * @brief : Tells whether the set is empty
 * @param TREE : Pointer to the tree whose size is to be checked
 * @return : true if set is empty, false otherwise
*/
JUST_API static inline bool justOrderedSetIsEmpty(const JustAVLTree* TREE)
{
  JUST_ASSERT_DEBUG_MESSAGE(TREE != NULL, "[ORDERED SET] : TREE cannot be NULL for checking emptiness");

  return (TREE->size == 0);
}


// - - - Ergonomic Macros - - -

#define JUST_ORDERED_SET_INIT(TREE_PTR, TYPE) \
  justOrderedSetCreate((TREE_PTR), sizeof(TYPE), NULL, NULL, "ORDERED_SET")

#define JUST_ORDERED_SET_INIT_TAGGED(TREE_PTR, TYPE, TAG) \
  justOrderedSetCreate((TREE_PTR), sizeof(TYPE), NULL, NULL, (TAG))

#define JUST_ORDERED_SET_INSERT_VAL(TREE_PTR, TYPE, VAL) \
  justOrderedSetInsert((TREE_PTR), &(TYPE){(VAL)})

#define JUST_ORDERED_SET_REMOVE_VAL(TREE_PTR, TYPE, VAL) \
  justOrderedSetRemove((TREE_PTR), &(TYPE){(VAL)})

#define JUST_ORDERED_SET_CONTAINS_VAL(TREE_PTR, TYPE, VAL) \
  justOrderedSetContains((TREE_PTR), &(TYPE){(VAL)})

#define JUST_ORDERED_SET_FIND_VAL(TREE_PTR, TYPE, VAL) \
  ((TYPE*)justOrderedSetFind((TREE_PTR), &(TYPE){(VAL)}))

#define JUST_ORDERED_SET_FIND_AS(TREE_PTR, TYPE, PTR) \
  ((TYPE*)justOrderedSetFind((TREE_PTR), (PTR)))

#ifdef __cplusplus
}
#endif

#endif // JUST_ORDER_SET

// - - - Implementation
#ifdef JUST_LIB_IMPL_ORDERED_SET

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int32_t defaultMemcmp(const void* A, const void* B, size_t SIZE)
{
  return memcmp(A, B, SIZE);
}

static inline int32_t nodeHeight(JustAVLNode* NODE)
{
  return NODE ? NODE->height : 0;
}

static inline int32_t maxInt(int32_t A, int32_t B)
{
  return (A > B) ? A : B;
}

static inline int32_t getBalance(JustAVLNode* NODE)
{
  return NODE ? (nodeHeight(NODE->left) - nodeHeight(NODE->right)) : 0;
}

static inline JustAVLNode* rotateRight(JustAVLNode* Y)
{
  JustAVLNode* x  = Y->left;
  JustAVLNode* t2 = x->right;

  x->right = Y;
  Y->left  = t2;

  Y->height = maxInt(nodeHeight(Y->left), nodeHeight(Y->right)) + 1;
  x->height = maxInt(nodeHeight(x->left), nodeHeight(x->right)) + 1;

  return x;
}

static inline JustAVLNode* rotateLeft(JustAVLNode* X)
{
  JustAVLNode* y  = X->right;
  JustAVLNode* t2 = y->left;

  y->left  = X;
  X->right = t2;

  X->height = maxInt(nodeHeight(X->left), nodeHeight(X->right)) + 1;
  y->height = maxInt(nodeHeight(y->left), nodeHeight(y->right)) + 1;

  return y;
}

static JustAVLNode* createNode(JustAVLTree* TREE, const void* VALUE_PTR) 
{
  size_t totalBytes = sizeof(JustAVLNode) + TREE->elementSize;

  JustAVLNode* node = TREE->allocator
                      ? (JustAVLNode*) justLinearAllocAllocate(TREE->allocator, totalBytes, alignof(JustAVLNode))
                      : (JustAVLNode*) JUST_MALLOC_TAGGED(totalBytes, TREE->tag);

  if (!node) 
  {
    JUST_LOG_ERROR("[ORDERED SET] : Failed to allocate node of %zu bytes", totalBytes);
    return NULL;
  }

  node->height = 1;
  node->left   = NULL;
  node->right  = NULL;
  memcpy(node->data, VALUE_PTR, TREE->elementSize);

  return node;
}

static inline void freeNode(JustAVLTree* TREE, JustAVLNode* NODE) 
{
  if (!NODE) return;
  if (!TREE->allocator) JUST_FREE(NODE);
}

static void destroySubtree(JustAVLTree* TREE, JustAVLNode* NODE) 
{
  if (!NODE) return;
  destroySubtree(TREE, NODE->left);
  destroySubtree(TREE, NODE->right);
  freeNode(TREE, NODE);
}

static JustAVLNode* insertRecursive(
  JustAVLTree* TREE,
  JustAVLNode* NODE,
  const void*  VALUE_PTR,
  bool*        OUT_INSERTED)
{
  if (!NODE)
  {
    *OUT_INSERTED = true;
    return createNode(TREE, VALUE_PTR);
  }

  int32_t cmp = TREE->compare(VALUE_PTR, NODE->data, TREE->elementSize);

  if (cmp < 0) 
  {
    NODE->left = insertRecursive(TREE, NODE->left, VALUE_PTR, OUT_INSERTED);
  } 
  else if (cmp > 0) 
  {
    NODE->right = insertRecursive(TREE, NODE->right, VALUE_PTR, OUT_INSERTED);
  } 
  else 
  {
    *OUT_INSERTED = false;
    return NODE;
  }

  NODE->height    = 1 + maxInt(nodeHeight(NODE->left), nodeHeight(NODE->right));
  int32_t balance = getBalance(NODE);

  // - - - Left Left Case
  if (balance > 1 && TREE->compare(VALUE_PTR, NODE->left->data, TREE->elementSize) < 0)
  {
    return rotateRight(NODE);
  }

  // - - - Right Right Case
  if (balance < -1 && TREE->compare(VALUE_PTR, NODE->right->data, TREE->elementSize) > 0)
  {
    return rotateLeft(NODE);
  }

  // - - - Left Right Case
  if (balance > 1 && TREE->compare(VALUE_PTR, NODE->left->data, TREE->elementSize) > 0)
  {
    NODE->left = rotateLeft(NODE->left);
    return rotateRight(NODE);
  }

  // - - - Right Left Case
  if (balance < -1 && TREE->compare(VALUE_PTR, NODE->right->data, TREE->elementSize) < 0)
  {
    NODE->right = rotateRight(NODE->right);
    return rotateLeft(NODE);
  }

  return NODE;
}

static JustAVLNode* minValueNode(JustAVLNode* NODE)
{
  JustAVLNode* curr = NODE;
  while (curr && curr->left) curr = curr->left;
  return curr;
}

static JustAVLNode* removeRecursive(
  JustAVLTree* TREE,
  JustAVLNode* ROOT,
  const void*  VALUE_PTR,
  bool*        OUT_REMOVED)
{
  if (!ROOT) 
  {
    *OUT_REMOVED = false;
    return NULL;
  }

  int32_t cmp = TREE->compare(VALUE_PTR, ROOT->data, TREE->elementSize);

  if (cmp < 0) 
  {
    ROOT->left = removeRecursive(TREE, ROOT->left, VALUE_PTR, OUT_REMOVED);
  } 
  else if (cmp > 0) 
  {
    ROOT->right = removeRecursive(TREE, ROOT->right, VALUE_PTR, OUT_REMOVED);
  } 
  else 
  {
    *OUT_REMOVED = true;

    if (!ROOT->left || !ROOT->right) 
    {
      JustAVLNode* temp = ROOT->left ? ROOT->left : ROOT->right;

      if (!temp) 
      {
        freeNode(TREE, ROOT);
        return NULL;
      } 
      else 
      {
        memcpy(ROOT->data, temp->data, TREE->elementSize);
        ROOT->left   = temp->left;
        ROOT->right  = temp->right;
        ROOT->height = temp->height;
        freeNode(TREE, temp);
      }
    } 
    else 
    {
      JustAVLNode* temp = minValueNode(ROOT->right);
      memcpy(ROOT->data, temp->data, TREE->elementSize);
      ROOT->right = removeRecursive(TREE, ROOT->right, temp->data, OUT_REMOVED);
    }
  }

  ROOT->height    = 1 + maxInt(nodeHeight(ROOT->left), nodeHeight(ROOT->right));
  int32_t balance = getBalance(ROOT);

  if (balance > 1 && getBalance(ROOT->left) >= 0)
  {
    return rotateRight(ROOT);
  }

  if (balance > 1 && getBalance(ROOT->left) < 0)
  {
    ROOT->left = rotateLeft(ROOT->left);
    return rotateRight(ROOT);
  }

  if (balance < -1 && getBalance(ROOT->right) <= 0)
  {
    return rotateLeft(ROOT);
  }

  if (balance < -1 && getBalance(ROOT->right) > 0)
  {
    ROOT->right = rotateRight(ROOT->right);
    return rotateLeft(ROOT);
  }

  return ROOT;
}

JUST_API bool justOrderedSetCreate(
  JustAVLTree*         TREE,
  size_t               ELEMENT_SIZE,
  justCompareFunc      COMPARATOR,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(TREE != NULL, "[ORDERED SET] : Target pointer cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(ELEMENT_SIZE > 0, "[ORDERED SET] : ELEMENT_SIZE must be > 0");

  TREE->root        = NULL;
  TREE->size        = 0;
  TREE->elementSize = ELEMENT_SIZE;
  TREE->compare     = COMPARATOR ? COMPARATOR : defaultMemcmp;
  TREE->allocator   = ALLOCATOR;
  snprintf(TREE->tag, sizeof(TREE->tag), "%s", TAG);

  return true;
}

JUST_API void justOrderedSetDestroy(JustAVLTree* TREE) 
{
  JUST_ASSERT_DEBUG_MESSAGE(TREE != NULL, "[ORDERED SET] : Cannot destroy a NULL TREE");
  destroySubtree(TREE, TREE->root);
  TREE->root = NULL;
  TREE->size = 0;
}

JUST_API bool justOrderedSetInsert(JustAVLTree* TREE, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG(TREE != NULL && VALUE_PTR != NULL);

  bool inserted = false;
  TREE->root    = insertRecursive(TREE, TREE->root, VALUE_PTR, &inserted);
  if (inserted) TREE->size++;

  return inserted;
}

JUST_API bool justOrderedSetRemove(JustAVLTree* TREE, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG(TREE != NULL && VALUE_PTR != NULL);
  if (!TREE->root) return false;

  bool removed  = false;
  TREE->root    = removeRecursive(TREE, TREE->root, VALUE_PTR, &removed);
  if (removed) TREE->size--;

  return removed;
}

JUST_API void* justOrderedSetFind(const JustAVLTree* TREE, const void* VALUE_PTR) 
{
  JUST_ASSERT_DEBUG(TREE != NULL && VALUE_PTR != NULL);

  JustAVLNode* curr = TREE->root;
  while (curr) 
  {
    int32_t cmp = TREE->compare(VALUE_PTR, curr->data, TREE->elementSize);

    if (cmp == 0)      return (void*)curr->data;
    else if (cmp < 0) curr = curr->left;
    else              curr = curr->right;
  }

  return NULL;
}

static void inorderRecursive(JustAVLNode* NODE, justVisitorFunc VISITOR, void* USER_DATA)
{
  if (!NODE) return;
  inorderRecursive(NODE->left, VISITOR, USER_DATA);
  VISITOR((const void*)NODE->data, USER_DATA);
  inorderRecursive(NODE->right, VISITOR, USER_DATA);
}

JUST_API void justOrderedSetTraverseInorder(
  const JustAVLTree* TREE,
  justVisitorFunc    VISITOR,
  void*              USER_DATA)
{
  JUST_ASSERT_DEBUG(TREE != NULL && VISITOR != NULL);
  inorderRecursive(TREE->root, VISITOR, USER_DATA);
}

JUST_API void justOrderedSetClear(JustAVLTree* TREE)
{
  JUST_ASSERT_DEBUG(TREE != NULL);
  destroySubtree(TREE, TREE->root);
  TREE->root = NULL;
  TREE->size = 0;
}

#endif // JUST_ORDER_SET


// - - - Hashmap - - -

// - - - HEADER
#ifndef JUST_HASH
#define JUST_HASH

/**
 * @file : hashMap.h 
 * @brief : Simple Hash map in C
 */

#include <stddef.h>
#include <stdint.h>

#define JUST_MAP_DEFAULT_CAPACITY  16


#ifdef __cplusplus
extern "C" {
#endif

/** @brief : Custom hash function signature (if NULL, defaults to FNV-1a)
 * @param KEY : Hash key 
 * @param KEY_SIZE : The size of the key 
 * @return : index
*/
typedef uint64_t (*justHashFunction)(const void* KEY, size_t KEY_SIZE);

/** @brief : Custom key comparison function (if NULL, defaults to memcmp)
 * @param KEY_A : One key 
 * @param KEY_B : Another key 
 * @param KEY_SIZE : The size of the key 
 * @return : comparison result of the two keys
*/
typedef int32_t (*justKeyCompareFunction)(const void* KEY_A, const void* KEY_B, size_t KEY_SIZE);

/// @brief : What can an entry be
typedef enum justHashMapEntryState 
{
  JUST_MAP_EMPTY     = 0,  ///< Empty slot
  JUST_MAP_OCCUPIED  = 1,  ///< Occupied slot
  JUST_MAP_TOMBSTONE = 2   ///< Freed slot
} justHashMapEntryState;

/// @brief : Hashmap view
typedef struct justHashMap
{
  uint8_t*               slots;          ///< Interleaved flat array of slots
  size_t                 capacity;       ///< Always a power of 2
  size_t                 mask;           ///< capacity - 1
  size_t                 count;          ///< Active key-vale pairs
  size_t                 tombstoneCount; ///< Dead slots 
  size_t                 keySize;        ///< Key size in bytes
  size_t                 valueSize;      ///< Value size in bytes
  size_t                 slotStride;     ///< Total bytes per slot (aligned)
  size_t                 keyOffset;      ///< Byte offset of key inside slot
  size_t                 valueOffset;    ///< Byte offset of value inside slot
  justHashFunction       hashFunction;
  justKeyCompareFunction compareFunction;
  JustLinearAllocator*   allocator;
  char                   tag[32];        ///< Why make this hashmap
} JustHashMap;

/**
 * @brief : Initializes a Hash Map.
 * 
 * @param MAP : Pointer to HashMap struct.
 * @param KEY_SIZE : Size of key in bytes (e.g. sizeof(int) or string pointer size).
 * @param VALUE_SIZE : Size of value in bytes.
 * @param INITIAL_CAPACITY : Initial capacity (rounded up to power of 2, minimum 16).
 * @param HASHER :  Custom hash function or NULL for default FNV-1a.
 * @param COMPARATOR : Custom key compare function or NULL for default memcmp.
 * @param ALLOCATOR : Pointer to linear allocator or NULL for global memory tracker.
 * @param TAG : For memory tracker
 * @return : true if initialized successfully, false otherwise.
 */
JUST_API bool justHashmapCreate(
  JustHashMap*           MAP,
  size_t                 KEY_SIZE,
  size_t                 VALUE_SIZE,
  size_t                 INITIAL_CAPACITY,
  justHashFunction       HASHER,
  justKeyCompareFunction COMPARATOR,
  JustLinearAllocator*   ALLOCATOR,
  const char*            TAG);

/**
 * @brief : Destroys the Hash Map and frees backing buffers.
 * @param MAP : Pointer to the map to be destroyed
*/
JUST_API void justHashmapDestroy(JustHashMap* MAP);

/**
 * @brief : Inserts or updates a key-value pair.
 * 
 * @param MAP : Pointer to HashMap.
 * @param KEY_PTR : Pointer to key bytes.
 * @param VALUE_PTR : Pointer to value bytes.
 * @return : true if inserted or updated successfully, false on allocation failure.
 */
JUST_API bool justHashmapSet(JustHashMap* MAP, const void* KEY_PTR, const void* VALUE_PTR);

/**
 * @brief : Retrieves a value pointer associated with the given key.
 * 
 * @param MAP : Pointer to HashMap.
 * @param KEY_PTR : Pointer to search key.
 * @return : Pointer to value data inside the map, or NULL if key is not found.
 */
JUST_API void* justHashmapGet(const JustHashMap* MAP, const void* KEY_PTR);

/**
 * @brief : Removes a key-value pair from the map.
 * 
 * @param MAP : Pointer to HashMap.
 * @param KEY_PTR : Pointer to key bytes to remove.
 * @return : true if key was found and removed, false if not found.
 */
JUST_API bool justHashmapRemove(JustHashMap* MAP, const void* KEY_PTR);

/**
 * @brief : Clears all entries without deallocating the underlying buffer.
 * @param MAP : Pointer to the hashmap to be cleared
 */
JUST_API void justHashmapClear(JustHashMap* MAP);

/**
 * @brief : Checks if a key exists in the Hash Map.
 * @param MAP : Pointer to the hash map 
 * @param KEY_PTR : Pointer to the key to be searched 
 * @return : true if the hashmap contains the key, false otherwise
 */
JUST_API static inline bool justHashmapContains(const JustHashMap* MAP, const void* KEY_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot check in a NULL MAP");
  JUST_ASSERT_DEBUG_MESSAGE(KEY_PTR != NULL, "[HASH MAP] : Cannot check a NULL KEY_PTR");

  return (justHashmapGet(MAP, KEY_PTR) != NULL);
}

/**
 * @brief : Returns total active elements stored.
 * @param MAP : Pointer to the map 
 * @return : Number of elements stored
 */
JUST_API static inline size_t justHashmapSize(const JustHashMap* MAP) 
{ 
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot check in a NULL MAP");

  return MAP->count;
}

/**
 * @brief : Tells whether the map is empty or not
 * @param MAP : Pointer to the map
 * @return : True if empty, false otherwise
*/
JUST_API static inline bool justHashmapIsEmpty(const JustHashMap* MAP)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot check in a NULL MAP");

  return (MAP->count == 0);
}


// - - - Ergonomic Macros - - -

#define JUST_HASHMAP_INIT_TAGGED(MAP_PTR, KEY_TYPE, VAL_TYPE, CAP, TAG) \
  justHashmapCreate((MAP_PTR), sizeof(KEY_TYPE), sizeof(VAL_TYPE), (CAP), NULL, NULL, (NULL), TAG)

#define JUST_HASHMAP_INIT(MAP_PTR, KEY_TYPE, VAL_TYPE, CAP) JUST_HASHMAP_INIT_TAGGED(MAP_PTR, KEY_TYPE, VAL_TYPE, CAP, "HASH MAP")

#define JUST_HASHMAP_GET(MAP_PTR, VAL_TYPE, KEY) \
  (*(VAL_TYPE*)justHashmapGet((MAP_PTR), &(KEY)))

#define JUST_HASHMAP_GET_OR_DEFAULT(MAP_PTR, VAL_TYPE, KEY, DEFAULT_VAL)  \
  (                                                                       \
    justHashmapGet((MAP_PTR), &(KEY))                                     \
        ? *(VAL_TYPE*)justHashmapGet((MAP_PTR), &(KEY))                   \
        : (DEFAULT_VAL)                                                   \
  )

#define JUST_HASHMAP_SET(MAP_PTR, KEY, VAL) \
  justHashmapSet((MAP_PTR), &(KEY), &(VAL))

#ifdef __cplusplus
}
#endif

#endif // JUST_HASH

// - - - IMPLEMENTATION
#ifdef JUST_LIB_IMPL_HASHMAP

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Slot header: hash + state
typedef struct justSlotHeader
{
  alignas(max_align_t) uint64_t   hash;
  justHashMapEntryState           state;
} justSlotHeader;

// - - - Fast 64-bit SplitMix-style hash for <=8 byte keys, fallback to block-hash
static uint64_t defaultFastHash(const void* KEY, size_t KEY_SIZE)
{
  const uint8_t*  bytes = (const uint8_t*)KEY;
  uint64_t        hash  = 14695981039346656037ULL;
  for (size_t i = 0; i < KEY_SIZE; ++i)
  {
    hash ^= bytes[i];
    hash *= 1099511628211ULL;
  }
  return hash;
}

static int32_t defaultComparator(const void* A, const void* B, size_t SIZE)
{
  if (SIZE == sizeof(uint64_t))
  {
    return *(const uint64_t*)A == *(const uint64_t*)B ? 0 : 1;
  }
  return memcmp(A, B, SIZE);
}

static inline size_t justMapNextPowerOfTwo(size_t N)
{
  if (N < JUST_MAP_DEFAULT_CAPACITY) return JUST_MAP_DEFAULT_CAPACITY;
  N--;
  N |= N >> 1; N |= N >> 2; N |= N >> 4; N |= N >> 8; N |= N >> 16;
#if UINTPTR_MAX > 0xFFFFFFFF
  N |= N >> 32;
#endif
  N++;
  return N;
}

static inline justSlotHeader* getHeader(const JustHashMap* MAP, size_t INDEX)
{
  return (justSlotHeader*)(MAP->slots + (INDEX * MAP->slotStride));
}

static inline void* getKeyPtr(const JustHashMap* MAP, size_t INDEX)
{
  return (void*)(MAP->slots + (INDEX * MAP->slotStride) + MAP->keyOffset);
}

static inline void* getValPtr(const JustHashMap* MAP, size_t INDEX)
{
  return (void*)(MAP->slots + (INDEX * MAP->slotStride) + MAP->valueOffset);
}

static bool allocateSlots(JustHashMap* MAP, size_t CAPACITY)
{
  size_t    totalBytes = CAPACITY * MAP->slotStride;
  uint8_t*  buf        = MAP->allocator
                 ? (uint8_t*) justLinearAllocAllocate(MAP->allocator, totalBytes, 16)
                 : (uint8_t*) JUST_MALLOC_TAGGED(totalBytes, MAP->tag);

  if (!buf) return false;

  memset(buf, 0, totalBytes);
  MAP->slots = buf;
  return true;
}

JUST_API bool justHashmapCreate(
  JustHashMap*           MAP,
  size_t                 KEY_SIZE,
  size_t                 VALUE_SIZE,
  size_t                 INITIAL_CAPACITY,
  justHashFunction       HASHER,
  justKeyCompareFunction COMPARATOR,
  JustLinearAllocator*   ALLOCATOR,
  const char*            TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot create a NULL Hashmap");
  JUST_ASSERT_DEBUG_MESSAGE(KEY_SIZE > 0, "[HASH MAP] : Cannot create a Hash map with KEY_SIZE under 1");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_SIZE > 0, "[HASH MAP] : Cannot create a Hash map with VALUE_SIZE under 1");

  MAP->keySize          = KEY_SIZE;
  MAP->valueSize        = VALUE_SIZE;
  MAP->capacity         = justMapNextPowerOfTwo(INITIAL_CAPACITY);
  MAP->mask             = MAP->capacity ? (MAP->capacity - 1) : 0;
  MAP->count            = 0;
  MAP->tombstoneCount   = 0;
  MAP->hashFunction     = HASHER ? HASHER : defaultFastHash;
  MAP->compareFunction  = COMPARATOR ? COMPARATOR : defaultComparator;
  MAP->allocator        = ALLOCATOR;
  snprintf(MAP->tag, sizeof(MAP->tag), "%s", TAG);

  // - - - Interleaved slot layout: [Header] [Key] [Pad] [Value] [Pad]
  MAP->keyOffset    = sizeof(justSlotHeader);
  size_t keyAligned = (KEY_SIZE + 7) & ~7;
  MAP->valueOffset  = MAP->keyOffset + keyAligned;
  size_t valAligned = (VALUE_SIZE + 7) & ~7;
  MAP->slotStride   = MAP->valueOffset + valAligned;

  size_t maxAlign = alignof(max_align_t);
  size_t alignMask = maxAlign - 1;

  MAP->slotStride = (MAP->valueOffset + valAligned + alignMask) & ~alignMask;

  if (MAP->capacity == 0) return true;

  if (!allocateSlots(MAP, MAP->capacity))
  {
    JUST_LOG_ERROR("[HASHMAP] : Failed to allocate slot memory");
    MAP->capacity = 0;
    MAP->mask     = 0;
    return false;
  }

  return true;
}

JUST_API void justHashmapDestroy(JustHashMap* MAP)
{
  JUST_ASSERT_DEBUG(MAP != NULL);

  if (MAP->slots && !MAP->allocator)
  {
    JUST_FREE(MAP->slots);
  }

  MAP->slots          = NULL;
  MAP->capacity       = 0;
  MAP->mask           = 0;
  MAP->count          = 0;
  MAP->tombstoneCount = 0;
  MAP->allocator      = NULL;
}

JUST_API static bool hashmapResize(JustHashMap* MAP, size_t NEW_CAPACITY)
{
  uint8_t* oldSlots     = MAP->slots;
  size_t   oldCap       = MAP->capacity;
  size_t   oldStride    = MAP->slotStride;
  size_t   oldKeyOffset = MAP->keyOffset;
  size_t   oldValOffset = MAP->valueOffset;

  MAP->capacity       = NEW_CAPACITY;
  MAP->mask           = NEW_CAPACITY - 1;
  MAP->count          = 0;
  MAP->tombstoneCount = 0;

  if (!allocateSlots(MAP, NEW_CAPACITY)) return false;

  // - - - Re-insert using cached hash without calling hasher or comparator
  if (oldSlots && oldCap > 0)
  {
    for (size_t i = 0; i < oldCap; ++i)
    {
      justSlotHeader* oldHeader = (justSlotHeader*)(oldSlots + (i * oldStride));
      if (oldHeader->state == JUST_MAP_OCCUPIED)
      {
        const void* key   = (const void*)(oldSlots + (i * oldStride) + oldKeyOffset);
        const void* value = (const void*)(oldSlots + (i * oldStride) + oldValOffset);

        size_t index = oldHeader->hash & MAP->mask;
        while (getHeader(MAP, index)->state == JUST_MAP_OCCUPIED)
        {
          index = (index + 1) & MAP->mask;
        }

        justSlotHeader* newHdr = getHeader(MAP, index);
        newHdr->hash            = oldHeader->hash;
        newHdr->state           = JUST_MAP_OCCUPIED;

        memcpy(getKeyPtr(MAP, index), key, MAP->keySize);
        memcpy(getValPtr(MAP, index), value, MAP->valueSize);
        MAP->count++;
      }
    }
    if (!MAP->allocator) JUST_FREE(oldSlots);
  }

  return true;
}

JUST_API bool justHashmapSet(JustHashMap* MAP, const void* KEY_PTR, const void* VALUE_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot set in a NULL MAP");
  JUST_ASSERT_DEBUG_MESSAGE(KEY_PTR != NULL, "[HASH MAP] : Cannot set with a KEY_PTR");
  JUST_ASSERT_DEBUG_MESSAGE(VALUE_PTR != NULL, "[HASH MAP] : Cannot set with a VALUE_PTR");

  // - - - Lazy alloc or 75% load factor expansion
  if (MAP->capacity == 0)
  {
    if (!hashmapResize(MAP, JUST_MAP_DEFAULT_CAPACITY)) return false;
  }

  // - - - Purge tombstones in-place if count alone is below 50% load, otherwise double
  else if ((MAP->count + MAP->tombstoneCount + 1) * 4 >= MAP->capacity * 3)
  {
    size_t targetCap = (MAP->count * 2 > MAP->capacity) ? (MAP->capacity * 2) : MAP->capacity;
    if (!hashmapResize(MAP, targetCap)) return false;
  }

  uint64_t hash      = MAP->hashFunction(KEY_PTR, MAP->keySize);
  size_t   index     = hash & MAP->mask;
  int64_t  firstTomb = -1;

  for (size_t i = 0; i < MAP->capacity; ++i)
  {
    size_t            probeIdx  = (index + i) & MAP->mask;
    justSlotHeader*  header    = getHeader(MAP, probeIdx);

    if (header->state == JUST_MAP_EMPTY)
    {
      size_t            targetIdx = (firstTomb != -1) ? (size_t)firstTomb : probeIdx;
      justSlotHeader*  targetHdr = getHeader(MAP, targetIdx);

      if (targetHdr->state == JUST_MAP_TOMBSTONE)
      {
        MAP->tombstoneCount--;
      }

      targetHdr->hash  = hash;
      targetHdr->state = JUST_MAP_OCCUPIED;

      memcpy(getKeyPtr(MAP, targetIdx), KEY_PTR, MAP->keySize);
      memcpy(getValPtr(MAP, targetIdx), VALUE_PTR, MAP->valueSize);
      MAP->count++;
      return true;
    }

    if (header->state == JUST_MAP_TOMBSTONE)
    {
      if (firstTomb == -1) firstTomb = (int64_t)probeIdx;
    }
    else if (header->hash == hash)
    {
      if (MAP->compareFunction(getKeyPtr(MAP, probeIdx), KEY_PTR, MAP->keySize) == 0)
      {
        memcpy(getValPtr(MAP, probeIdx), VALUE_PTR, MAP->valueSize);
        return true;
      }
    }
  }

  // - - - If table had tombstones but no EMPTY slots in probe path
  if (firstTomb != -1)
  {
    size_t targetIndex            = (size_t) firstTomb;
    justSlotHeader* targetHeader = getHeader(MAP, targetIndex);

    MAP->tombstoneCount--;
    targetHeader->hash  = hash;
    targetHeader->state = JUST_MAP_OCCUPIED;

    memcpy(getKeyPtr(MAP, targetIndex), KEY_PTR, MAP->keySize);
    memcpy(getValPtr(MAP, targetIndex), VALUE_PTR, MAP->valueSize);
    MAP->count++;
    return true;
  }

  return false;
}

JUST_API void* justHashmapGet(const JustHashMap* MAP, const void* KEY_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot get in a NULL MAP");
  JUST_ASSERT_DEBUG_MESSAGE(KEY_PTR != NULL, "[HASH MAP] : Cannot get with a KEY_PTR");

  if (MAP->count == 0 || MAP->capacity == 0) return NULL;

  uint64_t hash = MAP->hashFunction(KEY_PTR, MAP->keySize);
  size_t   idx  = hash & MAP->mask;

  for (size_t i = 0; i < MAP->capacity; ++i)
  {
    size_t            probeIdx  = (idx + i) & MAP->mask;
    justSlotHeader*  header    = getHeader(MAP, probeIdx);

    if (header->state == JUST_MAP_EMPTY) return NULL;

    if (header->state == JUST_MAP_OCCUPIED && header->hash == hash)
    {
      if (MAP->compareFunction(getKeyPtr(MAP, probeIdx), KEY_PTR, MAP->keySize) == 0)
      {
        return getValPtr(MAP, probeIdx);
      }
    }
  }

  return NULL;
}

JUST_API bool justHashmapRemove(JustHashMap* MAP, const void* KEY_PTR)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot remove from a NULL MAP");
  JUST_ASSERT_DEBUG_MESSAGE(KEY_PTR != NULL, "[HASH MAP] : Cannot remove with a NULL KEY_PTR");

  if (MAP->count == 0 || MAP->capacity == 0) return false;

  uint64_t hash   = MAP->hashFunction(KEY_PTR, MAP->keySize);
  size_t   index  = hash & MAP->mask;

  for (size_t i = 0; i < MAP->capacity; ++i)
  {
    size_t            probeIdx  = (index + i) & MAP->mask;
    justSlotHeader*  header    = getHeader(MAP, probeIdx);

    if (header->state == JUST_MAP_EMPTY) return false;

    if (header->state == JUST_MAP_OCCUPIED && header->hash == hash)
    {
      if (MAP->compareFunction(getKeyPtr(MAP, probeIdx), KEY_PTR, MAP->keySize) == 0)
      {
        header->state = JUST_MAP_TOMBSTONE;
        MAP->count--;
        MAP->tombstoneCount++;
        return true;
      }
    }
  }

  return false;
}

JUST_API void justHashmapClear(JustHashMap* MAP)
{
  JUST_ASSERT_DEBUG_MESSAGE(MAP != NULL, "[HASH MAP] : Cannot clear a NULL MAP");
  if (!MAP->slots) return;

  memset(MAP->slots, 0, MAP->capacity * MAP->slotStride);
  MAP->count          = 0;
  MAP->tombstoneCount = 0;
}

#endif // JUST_LIB_IMPL_HASHMAP


// - - - | Systems | - - -


// - - - File system - - -

// - - - Header
#ifndef JUST_FILE_SYS
#define JUST_FILE_SYS

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if JUST_PLATFORM == JUST_PLATFORM_WINDOWS
  #error "Currently openly support POSIX compliant systems"
#endif 

#ifdef __cplusplus
extern "C" {
#endif


// - - - File Handling Enums - - - 

/// @brief : Type of file
typedef enum justFileType
{
  JUST_FILE_TYPE_UNKOWN    = 0,  ///< No idea 
  JUST_FILE_TYPE_REGULAR   = 1,  ///< Its a regular, everyday normal 
  JUST_FILE_TYPE_DIRECTORY = 2,  ///< Its a folder 
  JUST_FILE_TYPE_SYMLINK   = 3   ///< Its a shortcut
} JustFileType;

///@brief : File modes
typedef enum justFileMode 
{
  JUST_FILE_READ,        ///< Open existing file for reading
  JUST_FILE_WRITE,       ///< Truncate or create file for writing
  JUST_FILE_APPEND,      ///< Append to end or create file for writing
  JUST_FILE_READ_WRITE   ///< Open for both reading and writing (no truncation)
} JustFileMode;

///@brief : Cursor positions
typedef enum justSeekOrigin 
{
  JUST_SEEK_SET,  ///< From beginning of file
  JUST_SEEK_CUR,  ///< From current cursor position
  JUST_SEEK_END   ///< From end of file
} JustSeekOrigin;

///@brief : What a file is
typedef struct justFile 
{
  int32_t fd;     ///< POSIX file descriptor
  bool    isOpen; ///< True if handle is currently open
} JustFile;

/// @brief : Callback type for zero-allocation directory iteration
typedef void (*justDirIterCallback)(const char* ENTRY_NAME, bool IS_DIRECTORY, void* USER_DATA);


// - - - Path Manipulation - - - 

/**
 * @brief : Normalizes a path by collapsing duplicate slashes and resolving . / .. tokens.
 * @param PATH : Raw input path.
 * @param OUT_BUFFER : Output buffer to write normalized string to.
 * @param BUFFER_SIZE : Capacity of out_buffer.
 * @return : true if successful, false if buffer is too small.
 */
JUST_API bool justPathNormalize(
  const char* PATH, 
  char*       OUT_BUFFER, 
  size_t      BUFFER_SIZE);

/**
 * @brief : Joins two path components with a single path separator.
 * @param PART_1: First path 
 * @param PART_2 : Second part 
 * @param OUT_BUFFER : A string buffer big enough to store the join
 * @param BUFFER_SIZE : The size of the buffer
 * @return : true if successful, false if buffer is too small.
 */
JUST_API bool justPathJoin(
  const char* PART_1,
  const char* PART_2,
  char*       OUT_BUFFER,
  size_t      BUFFER_SIZE);

/**
 * @brief : Extracts the parent directory component from a path.
 * @param PATH : Child path
 * @param OUT_BUFFER : Buffer that stores the parent path 
 * @param BUFFER_SIZE : The size of the buffer
 * @return : true if successful, false if buffer is too small.
 */
JUST_API bool justPathParent(
  const char* PATH, 
  char*       OUT_BUFFER, 
  size_t      BUFFER_SIZE);

/**
 * @brief : Returns a pointer to the filename component within the path string (no copy).
 * @param PATH : The path 
 * @return : pointer to the filename within the path
 */
JUST_API const char* justPathFilename(const char* PATH);

/**
 * @brief : Returns a pointer to the extension within the path string (e.g. "txt" or "png", no dot).
 * @param PATH : The file path 
 * @return : pointer to the extension back in the PATH
 */
JUST_API const char* justPathExtension(const char* PATH);


// - - - Handle-Based File I/O - - - 

/**
 * @brief : opens a file 
 * @param FILE : Pointer to a file 
 * @param PATH : File path 
 * @param MODE : Open mode 
 * @return : true if opened, false if not
 */
JUST_API bool justFileOpen(
  JustFile*    FILE,
  const char*  PATH,
  JustFileMode MODE);

/**
 * @brief : closes a file 
 * @param FILE : pointer to the file to be closed
 */
JUST_API void justFileClose(JustFile* FILE);

/**
 * @brief : reads a file 
 * @param FILE : Pointer to the file to be read 
 * @param OUT_BUFFER : Where to write file contents 
 * @param BYTES_TO_READ : How many bytes at max to read 
 * @param OUT_BYTES_READ : The function will fill this with how many bytes were read 
 * @return : true is successful, false if not
 */
JUST_API bool justFileRead(
  JustFile*  FILE,
  void*      OUT_BUFFER,
  size_t     BYTES_TO_READ,
  size_t*    OUT_BYTES_READ);

/**
 * @brief : writes a file 
 * @param FILE : Pointer to the file to be written 
 * @param BUFFER : What to write 
 * @param BYTES_TO_WRITE : How many bytes to write 
 * @param OUT_BYTES_WRITTEN : Function will fill this with how many bytes were written 
 * @return : true if successful, false if not
 */
JUST_API bool justFileWrite(
  JustFile*   FILE, 
  const void* BUFFER, 
  size_t      BYTES_TO_WRITE, 
  size_t*     OUT_BYTES_WRITTEN);

/**
 * @brief : sets cursor position in file 
 * @param FILE : Which file to set cursor in 
 * @param OFFSET : How much to move the cursor 
 * @param ORIGIN : Where to move the cursor from
 * @return : true if successful, false if not
 */
JUST_API bool justFileSeek(
  JustFile*       FILE, 
  int64_t         OFFSET, 
  JustSeekOrigin  ORIGIN);

/**
 * @brief : Tells where the cursor is 
 * @param FILE : Which file's cursor to check 
 * @return : position of the cursor 
 */
JUST_API size_t justFileTell(JustFile* FILE);

/**
 * @brief : Tells the size of the file 
 * @param FILE : The file whose size is to be checked
 * @return : size of the file in bytes
 */
JUST_API size_t justFileSize(JustFile* FILE);

/**
 * @brief : Commits all changes to the file now 
 * @param FILE : Which file to commit to disk 
 * @return : true if successful, false otherwise
 */
JUST_API bool justFileFlush(JustFile* FILE);


// - - - Entire FILE IO - - - 

/**
 * @brief : Reads an entire file intoa  heap buffer via the tracker (+1 null-terminator)
 * @param PATH : Path to file
 * @param OUT_SIZE : Pointer to receive exact byte count read (exclusing null terminator)
 * @param TAG : Memory tracker tag
 * @return : Allocated buffer, (must be freed with JUST_fREE), or NULL on failure
 */
JUST_API void* justFileReadEntire(
  const char* PATH,
  size_t*     OUT_SIZE,
  const char* TAG);

/**
 * @brief : Writes an entire buffer to a file (Creates or truncates).
 * @param PATH : Path to file
 * @param BUFFER : What to write
 * @param BYTES : How much to write, excluding null-terminator
 */
JUST_API bool justFileWriteEntire(
  const char* PATH,
  const void* BUFFER,
  size_t      BYTES);

/**
 * @brief : Appends an entire buffer to the end of a file (Creates if absent)
 * @param PATH : Path to the file
 * @param BUFFER : What to write
 * @param BYTES : How much to append, excluding null-terminator
 */
JUST_API bool justFileAppendEntire(
  const char* PATH,
  const void* BUFFER,
  size_t      BYTES);

// - - - File System Checks & Operations - - - 

/**
 * @brief : Find whether a file at a path exists 
 * @param PATH : The file path 
 * @return : true if it exists, false otherwise
 */
JUST_API bool justFileExists(const char* PATH);

/**
 * @brief : Find whether a path is a file or not 
 * @param PATH : The file path 
 * @return : true if it is a file, false otherwise
 */
JUST_API bool justIsFile(const char* PATH);

/**
 * @brief : Find whether a path is a folder or not 
 * @param PATH : The folder path 
 * @return : true if it is a folder, false otherwise
 */
JUST_API bool justIsDirectory(const char* PATH);

/**
 * @brief : Provides size of the file at the given path 
 * @param PATH : The file path 
 * @return : size of the file in bytes
 */
JUST_API size_t justGetFileSize(const char* PATH);

/**
 * @brief : Make directory 
 * @param PATH : Directory path 
 * @return : true if successful, false otherwise
 */
JUST_API bool justMkdir(const char* PATH);

/**
 * @brief : Make directory but recursive
 * @param PATH : Directory path
 * @return : true if successful, false otherwise
 */
JUST_API bool justMkdirRecursive(const char* PATH);

/**
 * @brief : removes a file at the given path 
 * @param PATH : File path 
 * @return : true if delted, false otherwise
 */
JUST_API bool justFileRemove(const char* PATH);

/**
 * @brief : Renames a file at the given path 
 * @param OLD_PATH : current path 
 * @param NEW_PATH : path to move to 
 * @return : true if successful, false otherwise
 */
JUST_API bool justFileRename(const char* OLD_PATH, const char* NEW_PATH);

/**
 * @brief : Zero-allocation directory iteration calling callback for each entry.
 * @param PATH : Directory path 
 * @param CALLBACK : Directory iteration callback function 
 * @param USER_DATA : Optional user data to hold around 
 */
JUST_API bool justListDir(
  const char*          PATH,
  justDirIterCallback  CALLBACK,
  void*                USER_DATA);

#ifdef __cplusplus
}
#endif

#endif // JUST_FILE_SYS

// - - - Implementation
#ifdef JUST_LIB_IMPL_FILE_SYS

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>


// - - - Path Manipulation - - -

JUST_API const char* justPathFilename(const char* PATH)
{
  if (!PATH) return NULL;
  const char* slash = strrchr(PATH, '/');
  return slash ? (slash + 1) : PATH;
}

JUST_API const char* justPathExtension(const char* PATH)
{
  if (!PATH) return NULL;
  const char* filename = justPathFilename(PATH);
  const char* dot = strrchr(filename, '.');
  return (dot && dot != filename && *(dot + 1) != '\0') ? (dot + 1) : NULL;
}

JUST_API bool justPathJoin(
  const char* PART_1, 
  const char* PART_2, 
  char*       OUT_BUFFER, 
  size_t      BUFFER_SIZE)
{
  JUST_ASSERT_DEBUG(PART_1 != NULL && PART_2 != NULL && OUT_BUFFER != NULL);
  if (BUFFER_SIZE == 0) return false;

  size_t len1 = strlen(PART_1);
  size_t len2 = strlen(PART_2);

  bool needsSlash = (len1 > 0 && PART_1[len1 - 1] != '/') && (len2 == 0 || PART_2[0] != '/');
  size_t totalLen = len1 + (needsSlash ? 1 : 0) + len2;

  if (totalLen >= BUFFER_SIZE) return false;

  int32_t written;
  if (needsSlash)
  {
    written = snprintf(OUT_BUFFER, BUFFER_SIZE, "%s/%s", PART_1, PART_2);
  }
  else
  {
    written = snprintf(OUT_BUFFER, BUFFER_SIZE, "%s%s", PART_1, PART_2);
  }

  return (written >= 0 && (size_t)written < BUFFER_SIZE);
}

JUST_API bool justPathParent(
  const char* PATH, 
  char*       OUT_BUFFER, 
  size_t      BUFFER_SIZE)
{
  JUST_ASSERT_DEBUG(PATH != NULL && OUT_BUFFER != NULL);
  if (BUFFER_SIZE == 0) return false;

  const char* slash = strrchr(PATH, '/');
  if (!slash)
  {
    if (BUFFER_SIZE < 2) return false;
    OUT_BUFFER[0] = '.';
    OUT_BUFFER[1] = '\0';
    return true;
  }

  if (slash == PATH)
  {
    if (BUFFER_SIZE < 2) return false;
    OUT_BUFFER[0] = '/';
    OUT_BUFFER[1] = '\0';
    return true;
  }

  size_t parentLen = (size_t)(slash - PATH);
  if (parentLen >= BUFFER_SIZE) return false;

  memcpy(OUT_BUFFER, PATH, parentLen);
  OUT_BUFFER[parentLen] = '\0';
  return true;
}

JUST_API bool justPathNormalize(
  const char* PATH, 
  char*       OUT_BUFFER, 
  size_t      BUFFER_SIZE)
{
  JUST_ASSERT_DEBUG(PATH != NULL && OUT_BUFFER != NULL);

  if (BUFFER_SIZE == 0) return false;

  char    temp[1024];
  size_t  pathLen = strlen(PATH);
  if (pathLen >= sizeof(temp)) return false;
  memcpy(temp, PATH, pathLen + 1);

  bool        isAbsolute = (temp[0] == '/');
  const char* segments[64];
  size_t      segCount = 0;

  char* cursor = temp;
  while (*cursor)
  {
    while (*cursor == '/') cursor++;
    if (!*cursor) break;

    char* start = cursor;
    while (*cursor && *cursor != '/') cursor++;
    if (*cursor)
    {
      *cursor = '\0';
      cursor++;
    }

    if (strcmp(start, ".") == 0)
    {
      continue;
    }
    else if (strcmp(start, "..") == 0)
    {
      if (segCount > 0 && strcmp(segments[segCount - 1], "..") != 0)
      {
        segCount--;
      }
      else if (!isAbsolute)
      {
        if (segCount < 64) segments[segCount++] = "..";
      }
    }
    else
    {
      if (segCount < 64) segments[segCount++] = start;
    }
  }

  OUT_BUFFER[0] = '\0';
  size_t outPos = 0;

  if (isAbsolute)
  {
    if (outPos + 1 >= BUFFER_SIZE) return false;
    OUT_BUFFER[outPos++] = '/';
    OUT_BUFFER[outPos] = '\0';
  }

  for (size_t i = 0; i < segCount; ++i)
  {
    size_t  segLen      = strlen(segments[i]);
    bool    needsSlash  = (outPos > 0 && OUT_BUFFER[outPos - 1] != '/');
    if (outPos + (needsSlash ? 1 : 0) + segLen >= BUFFER_SIZE) return false;

    if (needsSlash) OUT_BUFFER[outPos++] = '/';
    memcpy(OUT_BUFFER + outPos, segments[i], segLen);
    outPos += segLen;
    OUT_BUFFER[outPos] = '\0';
  }

  if (outPos == 0)
  {
    if (BUFFER_SIZE < 2) return false;
    OUT_BUFFER[0] = '.';
    OUT_BUFFER[1] = '\0';
  }

  return true;
}


// - - - Handle-Based File I/O - - -

JUST_API bool justFileOpen(
  JustFile*    FILE_HANDLE,
  const char*  PATH,
  JustFileMode MODE)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL && PATH != NULL);

  int32_t flags       = 0;
  mode_t  createMode  = 0644; // rw-r--r--

  switch (MODE)
  {
    case JUST_FILE_READ:
      flags = O_RDONLY;
      break;
    case JUST_FILE_WRITE:
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      break;
    case JUST_FILE_APPEND:
      flags = O_WRONLY | O_CREAT | O_APPEND;
      break;
    case JUST_FILE_READ_WRITE:
      flags = O_RDWR | O_CREAT;
      break;
    default:
      return false;
  }

  int32_t fd = open(PATH, flags, createMode);
  if (fd < 0)
  {
    FILE_HANDLE->fd     = -1;
    FILE_HANDLE->isOpen = false;
    return false;
  }

  FILE_HANDLE->fd     = fd;
  FILE_HANDLE->isOpen = true;
  return true;
}

JUST_API void justFileClose(JustFile* FILE_HANDLE)
{
  if (!FILE_HANDLE || !FILE_HANDLE->isOpen) return;

  close(FILE_HANDLE->fd);
  FILE_HANDLE->fd = -1;
  FILE_HANDLE->isOpen = false;
}

JUST_API bool justFileRead(
  JustFile* FILE_HANDLE,
  void*     OUT_BUFFER,
  size_t    BYTES_TO_READ,
  size_t*   OUT_BYTES_READ)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL && OUT_BUFFER != NULL);
  if (!FILE_HANDLE->isOpen) return false;

  ssize_t result = read(FILE_HANDLE->fd, OUT_BUFFER, BYTES_TO_READ);
  if (result < 0)
  {
    if (OUT_BYTES_READ) *OUT_BYTES_READ = 0;
    return false;
  }

  if (OUT_BYTES_READ) *OUT_BYTES_READ = (size_t)result;
  return true;
}

JUST_API bool justFileWrite(
  JustFile*  FILE_HANDLE, 
  const void* BUFFER, 
  size_t      BYTES_TO_WRITE, 
  size_t*     OUT_BYTES_WRITTEN)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL && BUFFER != NULL);
  if (!FILE_HANDLE->isOpen) return false;

  ssize_t result = write(FILE_HANDLE->fd, BUFFER, BYTES_TO_WRITE);
  if (result < 0)
  {
    if (OUT_BYTES_WRITTEN) *OUT_BYTES_WRITTEN = 0;
    return false;
  }

  if (OUT_BYTES_WRITTEN) *OUT_BYTES_WRITTEN = (size_t)result;
  return true;
}

JUST_API bool justFileSeek(
  JustFile*      FILE_HANDLE, 
  int64_t         OFFSET, 
  JustSeekOrigin ORIGIN)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL);
  if (!FILE_HANDLE->isOpen) return false;

  int whence = SEEK_SET;
  switch (ORIGIN)
  {
    case JUST_SEEK_SET: whence = SEEK_SET; break;
    case JUST_SEEK_CUR: whence = SEEK_CUR; break;
    case JUST_SEEK_END: whence = SEEK_END; break;
    default: return false;
  }

  return (lseek(FILE_HANDLE->fd, (off_t)OFFSET, whence) != (off_t)-1);
}

JUST_API size_t justFileTell(JustFile* FILE_HANDLE)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL);
  if (!FILE_HANDLE->isOpen) return 0;

  off_t pos = lseek(FILE_HANDLE->fd, 0, SEEK_CUR);
  return (pos == (off_t)-1) ? 0 : (uint64_t)pos;
}

JUST_API size_t justFileSize(JustFile* FILE_HANDLE)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL);
  if (!FILE_HANDLE->isOpen) return 0;

  struct stat st;
  if (fstat(FILE_HANDLE->fd, &st) != 0) return 0;
  return (uint64_t)st.st_size;
}

JUST_API bool justFileFlush(JustFile* FILE_HANDLE)
{
  JUST_ASSERT_DEBUG(FILE_HANDLE != NULL);
  if (!FILE_HANDLE->isOpen) return false;
  return (fsync(FILE_HANDLE->fd) == 0);
}


// - - - Whole-File Buffer I/O - - -

JUST_API void* justFileReadEntire(
  const char* PATH, 
  size_t*     OUT_SIZE, 
  const char* TAG)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  if (OUT_SIZE) *OUT_SIZE = 0;

  JustFile file;
  if (!justFileOpen(&file, PATH, JUST_FILE_READ))
  {
    JUST_LOG_WARNING("[FILESYS] : Failed to open file for whole read '%s'", PATH);
    return NULL;
  }

  uint64_t size = justFileSize(&file);
  const char* allocTag = (TAG && TAG[0] != '\0') ? TAG : "FILE_BUFFER";

  uint8_t* buffer = (uint8_t*) JUST_MALLOC_TAGGED((size_t)size + 1, allocTag);
  if (!buffer)
  {
    justFileClose(&file);
    JUST_LOG_ERROR("[FILESYS] : Out of memory reading file '%s' (%zu bytes)", PATH, (size_t)size);
    return NULL;
  }

  size_t totalRead = 0;
  while (totalRead < size)
  {
    size_t chunkRead = 0;
    if (!justFileRead(&file, buffer + totalRead, (size_t)(size - totalRead), &chunkRead) || chunkRead == 0)
    {
      break;
    }
    totalRead += chunkRead;
  }

  buffer[totalRead] = '\0';
  justFileClose(&file);

  if (OUT_SIZE) *OUT_SIZE = totalRead;
  return buffer;
}

JUST_API bool justFileWriteEntire(
  const char* PATH, 
  const void* BUFFER, 
  size_t      BYTES)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  if (BYTES > 0) JUST_ASSERT_DEBUG(BUFFER != NULL);

  JustFile file;
  if (!justFileOpen(&file, PATH, JUST_FILE_WRITE))
  {
    JUST_LOG_ERROR("[FILESYS] : Failed to open file for whole write '%s'", PATH);
    return false;
  }

  size_t totalWritten = 0;
  while (totalWritten < BYTES)
  {
    size_t chunkWritten = 0;
    if (!justFileWrite(&file, (const uint8_t*)BUFFER + totalWritten, BYTES - totalWritten, &chunkWritten) || chunkWritten == 0)
    {
      justFileClose(&file);
      return false;
    }
    totalWritten += chunkWritten;
  }

  justFileClose(&file);
  return (totalWritten == BYTES);
}

JUST_API bool justFileAppendEntire(
  const char* PATH, 
  const void* BUFFER, 
  size_t      BYTES)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  if (BYTES == 0) return true;
  JUST_ASSERT_DEBUG(BUFFER != NULL);

  JustFile file;
  if (!justFileOpen(&file, PATH, JUST_FILE_APPEND))
  {
    JUST_LOG_ERROR("[FILESYS] : Failed to open file for whole append '%s'", PATH);
    return false;
  }

  size_t totalWritten = 0;
  while (totalWritten < BYTES)
  {
    size_t chunkWritten = 0;
    if (!justFileWrite(&file, (const uint8_t*)BUFFER + totalWritten, BYTES - totalWritten, &chunkWritten) || chunkWritten == 0)
    {
      justFileClose(&file);
      return false;
    }
    totalWritten += chunkWritten;
  }

  justFileClose(&file);
  return (totalWritten == BYTES);
}


// - - - File System Checks & Operations - - -

JUST_API bool justFileExists(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL && PATH[0] != '\0');
  return (access(PATH, F_OK) == 0);
}

JUST_API bool justIsFile(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  struct stat st;
  if (stat(PATH, &st) != 0) return false;
  return S_ISREG(st.st_mode);
}

JUST_API bool justIsDirectory(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL);

  struct stat st;
  if (stat(PATH, &st) != 0) return false;
  return S_ISDIR(st.st_mode);
}

JUST_API size_t justGetFileSize(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  struct stat st;
  if (stat(PATH, &st) != 0) return 0;
  return (uint64_t)st.st_size;
}

JUST_API bool justMkdir(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  if (mkdir(PATH, 0755) == 0) return true;
  return (errno == EEXIST) && justIsDirectory(PATH);
}

JUST_API bool justMkdirRecursive(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL && PATH[0] != '\0');

  char temp[1024];
  size_t len = strlen(PATH);
  if (len >= sizeof(temp)) return false;
  memcpy(temp, PATH, len + 1);

  for (char* p = temp + 1; *p; p++)
  {
    if (*p == '/')
    {
      *p = '\0';
      if (mkdir(temp, 0755) != 0 && errno != EEXIST)
      {
        return false;
      }
      *p = '/';
    }
  }

  if (mkdir(temp, 0755) != 0 && errno != EEXIST)
  {
    return false;
  }

  return true;
}

JUST_API bool justFileRemove(const char* PATH)
{
  JUST_ASSERT_DEBUG(PATH != NULL);
  return (unlink(PATH) == 0);
}

JUST_API bool justFileRename(const char* OLD_PATH, const char* NEW_PATH)
{
  JUST_ASSERT_DEBUG(OLD_PATH != NULL && NEW_PATH != NULL);
  return (rename(OLD_PATH, NEW_PATH) == 0);
}

JUST_API bool justListDir(
  const char*          PATH,
  justDirIterCallback CALLBACK,
  void*                USER_DATA)
{
  JUST_ASSERT_DEBUG(PATH != NULL && CALLBACK != NULL);

  DIR* dir = opendir(PATH);
  if (!dir) return false;

  struct dirent* entry = NULL;
  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }

    bool isDir = false;
    char subPath[1024];
    if (justPathJoin(PATH, entry->d_name, subPath, sizeof(subPath)))
    {
      isDir = justIsDirectory(subPath);
    }

    CALLBACK(entry->d_name, isDir, USER_DATA);
  }

  closedir(dir);
  return true;
}
#endif // JUST_LIB_IMPL_FILE_SYS


// - - - Thread Pool - - -

// - - - Header
#ifndef JUST_THREAD_POOL
#define JUST_THREAD_POOL

#include <threads.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __STDC_NO_THREADS__
  #error "Target C compiler/platform does not support C11 <threads.h>"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Task callback function signature
typedef void (*justTaskFunc)(void* ARG);

/// @brief Task representation
typedef struct justTask 
{
  justTaskFunc func; ///< Task function to execute
  void*         arg;  ///< Argument passed to func
} justTask;

/// @brief : Thread pool representation
typedef struct justThreadPool 
{
  thrd_t*               threads;        ///< Standard C11 worker thread handles
  size_t                threadCount;    ///< Number of spawned worker threads
  
  JustQueue            taskQueue;      ///< Task queue
  
  mtx_t                 lock;           ///< C11 mutex protecting queue state
  cnd_t                 hasWork;        ///< Signaled when tasks are pushed
  cnd_t                 workingDone;    ///< Signaled when all tasks complete
  
  size_t                activeWorkers;  ///< Workers currently executing a task
  bool                  shutdown;       ///< Shutdown flag
  const char*           tag;            ///< Memory tracking tag
  JustLinearAllocator* allocator;      ///< Optional linear allocator
} justThreadPool;

/**
 * @brief Creates a worker thread pool using C11 threads.
 * @param POOL Pointer to ThreadPool struct.
 * @param THREAD_COUNT Number of worker threads.
 * @param QUEUE_CAPACITY Max pending tasks (rounded to power-of-two).
 * @param ALLOCATOR Optional linear allocator (NULL for heap).
 * @param TAG Tracking tag for memory tracker.
 * @return true on success, false on failure.
 */
JUST_API bool justThreadpoolCreate(
  justThreadPool*      POOL,
  size_t               THREAD_COUNT,
  size_t               QUEUE_CAPACITY,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG);

/**
 * @brief Submits a task to the queue.
 * @return true if enqueued, false if queue is full or pool is shutting down.
 */
JUST_API bool justThreadpoolAddTask(
  justThreadPool* POOL, 
  justTaskFunc    FUNC, 
  void*           ARG);

/**
 * @brief : Blocks calling thread until all currently queued and active tasks complete.
 * @param POOL : The pointer to the POOL
 */
JUST_API void justThreadpoolWait(justThreadPool* POOL);

/**
 * @brief : Gracefully terminates the thread pool and joins all worker threads.
 * @param POOL : Pointer of the pool
 */
JUST_API void justThreadpoolDestroy(justThreadPool* POOL);

/**
 * @brief : Returns total tasks currently pending or executing.
 * @param POOL : Pointer to the pool
 * @return : how many tasks remaining
 */
JUST_API size_t justThreadpoolPendingTasks(justThreadPool* POOL);


// - - - Ergonomic Macros - - -

#define JUST_THREAD_POOL_INIT_TAGGED(POOL, THREAD_COUNT, TAG) \
  justThreadpoolCreate((POOL_PTR), (THREAD_COUNT), (0), NULL, (TAG))

#define JUST_THREAD_POOL_INIT(POOL, THREAD_COUNT) JUST_THREAD_POOL_INIT_TAGGED(POOL, THREAD_COUNT, "THREAD POOL")

#ifdef __cplusplus
}
#endif

#endif // JUST_THREAD_POOL

// - - - Implementation
#ifdef JUST_LIB_IMPL_THREAD_POOL

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

#define JUST_TP_DEFAULT_CAPACITY 1024

static int32_t workerThreadLoop(void* ARGUMENTS) 
{
  justThreadPool* pool = (justThreadPool*)ARGUMENTS;

  while (1) 
  {
    mtx_lock(&pool->lock);

    while (justQueueIsEmpty(&pool->taskQueue) && !pool->shutdown) 
    {
      cnd_wait(&pool->hasWork, &pool->lock);
    }

    if (pool->shutdown && justQueueIsEmpty(&pool->taskQueue)) 
    {
      mtx_unlock(&pool->lock);
      return 0; 
    }

    justTask task;
    justQueueDequeue(&pool->taskQueue, &task);
    pool->activeWorkers++;

    mtx_unlock(&pool->lock);

    if (task.func) 
    {
      task.func(task.arg);
    }

    mtx_lock(&pool->lock);
    pool->activeWorkers--;

    if (justQueueIsEmpty(&pool->taskQueue) && pool->activeWorkers == 0) 
    {
      cnd_broadcast(&pool->workingDone);
    }

    mtx_unlock(&pool->lock);
  }

  return 0;
}

JUST_API bool justThreadpoolCreate(
  justThreadPool*      POOL,
  size_t               THREAD_COUNT,
  size_t               QUEUE_CAPACITY,
  JustLinearAllocator* ALLOCATOR,
  const char*          TAG)
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[THREAD POOL] : Target pool cannot be NULL");

  if (THREAD_COUNT == 0)
  {
    JUST_LOG_ERROR("[THREAD POOL] : threadCount cannot be 0! Explicit worker count is required.");
    return false;
  }

  if (QUEUE_CAPACITY == 0)
  {
    QUEUE_CAPACITY = JUST_TP_DEFAULT_CAPACITY;
  }

  POOL->threadCount   = 0;
  POOL->activeWorkers = 0;
  POOL->shutdown      = false;
  POOL->allocator     = ALLOCATOR;
  POOL->tag           = TAG;

  // - - - Initialize queue via justQueue
  if (!JUST_QUEUE_INIT_TAGGED(&POOL->taskQueue, QUEUE_CAPACITY, justTask, POOL->tag))
  {
    JUST_LOG_FATAL("[THREAD POOL] : Failed to initialize internal justQueue!");
    return false;
  }

  // - - - Allocate thread handles array
  size_t threadBytes = THREAD_COUNT * sizeof(thrd_t);
  if (POOL->allocator) 
  {
    POOL->threads = (thrd_t*)justLinearAllocAllocate(POOL->allocator, threadBytes, alignof(max_align_t));
  } 
  else 
  {
    POOL->threads = (thrd_t*)JUST_MALLOC_TAGGED(threadBytes, POOL->tag);
  }

  if (!POOL->threads) 
  {
    JUST_LOG_FATAL("[THREAD POOL] : Failed to allocate worker thread handles!");
    justQueueDestroy(&POOL->taskQueue);
    return false;
  }

  // - - - Initialize synchronization primitives
  if (mtx_init(&POOL->lock, mtx_plain) != thrd_success ||
      cnd_init(&POOL->hasWork) != thrd_success ||
      cnd_init(&POOL->workingDone) != thrd_success) 
  {
    JUST_LOG_FATAL("[THREAD POOL] : Failed to initialize C11 synchronization primitives!");
    if (!POOL->allocator) JUST_FREE(POOL->threads);
    justQueueDestroy(&POOL->taskQueue);
    return false;
  }

  // - - - Spawn worker threads 
  for (size_t i = 0; i < THREAD_COUNT; ++i) 
  {
    if (thrd_create(&POOL->threads[i], workerThreadLoop, POOL) != thrd_success) 
    {
      JUST_LOG_ERROR("[THREAD POOL] : Failed to spawn worker thread #%zu", i);
      justThreadpoolDestroy(POOL);
      return false;
    }
    POOL->threadCount++;
  }

  return true;
}

JUST_API bool justThreadpoolAddTask(
  justThreadPool*  POOL, 
  justTaskFunc     FUNC, 
  void*            ARG) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[THREAD POOL] : pool cannot be NULL");
  JUST_ASSERT_DEBUG_MESSAGE(FUNC != NULL, "[THREAD POOL] : task func cannot be NULL");

  mtx_lock(&POOL->lock);

  if (POOL->shutdown) 
  {
    mtx_unlock(&POOL->lock);
    return false;
  }

  justTask* slot = JUST_QUEUE_EMPLACE(&POOL->taskQueue, justTask);
  if (!slot) 
  {
    mtx_unlock(&POOL->lock);
    return false;
  }

  slot->func = FUNC;
  slot->arg  = ARG;

  cnd_signal(&POOL->hasWork);
  mtx_unlock(&POOL->lock);

  return true;
}

JUST_API void justThreadpoolWait(justThreadPool* POOL) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[THREAD POOL] : pool cannot be NULL");

  mtx_lock(&POOL->lock);
  while (!justQueueIsEmpty(&POOL->taskQueue) || POOL->activeWorkers > 0) 
  {
    cnd_wait(&POOL->workingDone, &POOL->lock);
  }
  mtx_unlock(&POOL->lock);
}

JUST_API void justThreadpoolDestroy(justThreadPool* POOL) 
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[THREAD POOL] : cannot destroy NULL POOL");

  mtx_lock(&POOL->lock);
  POOL->shutdown = true;
  cnd_broadcast(&POOL->hasWork);
  mtx_unlock(&POOL->lock);

  for (size_t i = 0; i < POOL->threadCount; ++i) 
  {
    thrd_join(POOL->threads[i], NULL);
  }

  mtx_destroy(&POOL->lock);
  cnd_destroy(&POOL->hasWork);
  cnd_destroy(&POOL->workingDone);

  justQueueDestroy(&POOL->taskQueue);

  if (!POOL->allocator && POOL->threads) 
  {
    JUST_FREE(POOL->threads);
  }

  POOL->threads     = NULL;
  POOL->threadCount = 0;
}

JUST_API size_t justThreadpoolPendingTasks(justThreadPool* POOL)
{
  JUST_ASSERT_DEBUG_MESSAGE(POOL != NULL, "[THREAD POOL] : cannot check pending path NULL POOL");

  mtx_lock(&POOL->lock);
  size_t count = justQueueSize(&POOL->taskQueue) + POOL->activeWorkers;
  mtx_unlock(&POOL->lock);

  return count;
}
#endif // JUST_LIB_IMPL_THREAD_POOL


// - - - | Testing | - - -


// - - - Expect header
#ifndef JUST_EXPECT
#define JUST_EXPECT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief : Result of executing a test
typedef enum justTestResult
{
  JUST_TEST_FAIL = 0, ///< Test failed
  JUST_TEST_PASS = 1, ///< Test passed
  JUST_TEST_SKIP = 2  ///< Test skipped
} JustTestResult;


// - - - Assertion Macros - - -

#define JUST_EXPECT_TO_BE(EXPECTED, ACTUAL)                                                           \
  do                                                                                                  \
  {                                                                                                   \
    if ((EXPECTED) != (ACTUAL))                                                                       \
    {                                                                                                 \
      JUST_LOG_ERROR("  [EXPECT FAILED] %s != %s at %s:%d", #EXPECTED, #ACTUAL, __FILE__, __LINE__);  \
      return JUST_TEST_FAIL;                                                                          \
    }                                                                                                 \
  } while(0)

#define JUST_EXPECT_NOT_TO_BE(NOT_EXPECTED, ACTUAL)                                                           \
  do                                                                                                          \
  {                                                                                                           \
    if ((NOT_EXPECTED) == (ACTUAL))                                                                           \
    {                                                                                                         \
      JUST_LOG_ERROR("  [NOT_EXPECT FAILED] %s == %s at %s:%d", #NOT_EXPECTED, #ACTUAL, __FILE__, __LINE__);  \
      return JUST_TEST_FAIL;                                                                                  \
    }                                                                                                         \
  } while(0)

#define JUST_EXPECT_C_STRING_TO_BE(EXPECTED, ACTUAL)                                              \
  do                                                                                              \
  {                                                                                               \
    const char* _exp = (EXPECTED);                                                                \
    const char* _act = (ACTUAL);                                                                  \
    if (_exp == NULL || _act == NULL || strcmp(_exp, _act) != 0)                                  \
    {                                                                                             \
      JUST_LOG_ERROR("  [EXPECT FAILED] String mismatch: Expected \"%s\", got \"%s\" at %s:%d",   \
                      _exp ? _exp : "NULL", _act ? _act : "NULL", __FILE__, __LINE__);            \
      return JUST_TEST_FAIL;                                                                      \
    }                                                                                             \
  } while(0)

#define JUST_EXPECT_FLOAT_TO_BE(EXPECTED, ACTUAL, EPS)                                      \
  do                                                                                        \
  {                                                                                         \
    double _diff = (double)(EXPECTED) - (double)(ACTUAL);                                   \
    if (_diff < 0.0) _diff = -_diff;                                                        \
    if (_diff > (double)(EPS))                                                              \
    {                                                                                       \
      JUST_LOG_ERROR("  [EXPECT FAILED] Float mismatch: %s vs %s (Diff: %f > %f) at %s:%d", \
                      #EXPECTED, #ACTUAL, _diff, (double)(EPS), __FILE__, __LINE__);        \
      return JUST_TEST_FAIL;                                                                \
    }                                                                                       \
  } while(0)

#define JUST_EXPECT_TO_BE_NULL(PTR)                                                                 \
  do                                                                                                \
  {                                                                                                 \
    if ((PTR) != NULL)                                                                              \
    {                                                                                               \
      JUST_LOG_ERROR("  [EXPECT FAILED] Expected NULL for %s at %s:%d", #PTR, __FILE__, __LINE__);  \
      return JUST_TEST_FAIL;                                                                        \
    }                                                                                               \
  } while(0)

#define JUST_EXPECT_TO_BE_NOT_NULL(PTR)                                                                 \
  do                                                                                                    \
  {                                                                                                     \
    if ((PTR) == NULL)                                                                                  \
    {                                                                                                   \
      JUST_LOG_ERROR("  [EXPECT FAILED] Expected non-NULL for %s at %s:%d", #PTR, __FILE__, __LINE__);  \
      return JUST_TEST_FAIL;                                                                            \
    }                                                                                                   \
  } while(0)

#define JUST_EXPECT_TO_BE_TRUE(EXPR)                                                              \
  do                                                                                              \
  {                                                                                               \
    if (!(EXPR))                                                                                  \
    {                                                                                             \
      JUST_LOG_ERROR("  [EXPECT FAILED] Expected TRUE: %s at %s:%d", #EXPR, __FILE__, __LINE__);  \
      return JUST_TEST_FAIL;                                                                      \
    }                                                                                             \
  } while(0)

#define JUST_EXPECT_TO_BE_FALSE(EXPR)                                                             \
  do                                                                                              \
  {                                                                                               \
    if ((EXPR))                                                                                   \
    {                                                                                             \
      JUST_LOG_ERROR("  [EXPECT FAILED] Expected FALSE: %s at %s:%d", #EXPR, __FILE__, __LINE__); \
      return JUST_TEST_FAIL;                                                                      \
    }                                                                                             \
  } while(0)

#ifdef __cplusplus
}
#endif
#endif // JUST_EXPECT

// - - - Test manager header
#ifndef JUST_TEST
#define JUST_TEST

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JUST_MAX_TESTS_PER_GROUP 256
#define JUST_MAX_TEST_GROUPS     32

typedef JustTestResult (*justTestFunc)(void);

/**
 * @brief : Registers a test to be executed sequentially.
 * @param TEST_FUNC : Function returning justTestResult.
 * @param DESCRIPTION : Short description of the test.
 * @param GROUP_ID : Logical cluster/category index (0 to JUST_MAX_TEST_GROUPS - 1).
 */
JUST_API void justTestRegister(
  justTestFunc TEST_FUNC, 
  const char*   DESCRIPTION, 
  uint8_t       GROUP_ID);

/**
 * @brief : Runs all registered tests across all groups and outputs a formatted summary.
 * @return : Total number of failed + crashed tests (0 indicates clean suite).
 */
JUST_API size_t justTestRunAll(void);

#ifdef __cplusplus
}
#endif
#endif // JUST_TEST

// - - - implementation
#ifdef JUST_LIB_IMPL_TESTING

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if JUST_PLATFORM != JUST_PLATFORM_WINDOWS
  #include <unistd.h>
  #include <sys/wait.h>
#endif

typedef struct justTestEntry 
{
  justTestFunc func;
  const char*  description;
} justTestEntry;

typedef struct justTestGroup 
{
  justTestEntry tests[JUST_MAX_TESTS_PER_GROUP];
  size_t        count;
} justTestGroup;

static justTestGroup testGroups[JUST_MAX_TEST_GROUPS];
static size_t         maxGroupID = 0;

JUST_API void justTestRegister(justTestFunc TEST_FUNC, const char* DESCRIPTION, uint8_t GROUP_ID) 
{
  JUST_ASSERT_MESSAGE(TEST_FUNC != NULL, "[TEST MANAGER] : Test function cannot be NULL");
  JUST_ASSERT_MESSAGE(GROUP_ID < JUST_MAX_TEST_GROUPS, "[TEST MANAGER] : GROUP_ID out of bounds");

  if (GROUP_ID > maxGroupID) maxGroupID = GROUP_ID;

  justTestGroup* group = &testGroups[GROUP_ID];
  JUST_ASSERT_MESSAGE(group->count < JUST_MAX_TESTS_PER_GROUP, "[TEST MANAGER] : Group test capacity exceeded!");

  group->tests[group->count].func        = TEST_FUNC;
  group->tests[group->count].description = DESCRIPTION;
  group->count++;
}

#if !defined(_WIN32)
static bool runTestForked(justTestEntry TEST, JustTestResult* OUT_RESULT) 
{
  int32_t pipefd[2];
  if (pipe(pipefd) != 0) 
  {
    JUST_LOG_ERROR("[TEST MANAGER] : Failed to create process pipe");
    return false;
  }

  pid_t pid = fork();
  if (pid == 0) 
  {
    // - - - Redirect child stdout & stderr to pipe
    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    JustTestResult res = TEST.func();
    exit((int)res);
  }

  // - - - Parent Process
  close(pipefd[1]);

  char    buffer[1024];
  ssize_t bytesRead;
  while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) 
  {
    buffer[bytesRead] = '\0';
    fputs(buffer, stdout);   
  }
  close(pipefd[0]);

  int32_t status = 0;
  waitpid(pid, &status, 0);

  // - - - Trapped by signal (SIGSEGV, SIGABRT, SIGBUS, etc.)
  if (WIFSIGNALED(status)) 
  {
    return false;
  }

  *OUT_RESULT = (JustTestResult)WEXITSTATUS(status);
  return true;
}
#endif

JUST_API size_t justTestRunAll(void) 
{
  size_t totalTests   = 0;
  size_t passedTests  = 0;
  size_t skippedTests = 0;
  size_t failedTests  = 0;
  size_t crashedTests = 0;

  JUST_LOG_INFO("==================================================");
  JUST_LOG_INFO("               RUNNING UNIT TESTS                 ");
  JUST_LOG_INFO("==================================================");

  for (size_t g = 0; g <= maxGroupID; ++g) 
  {
    justTestGroup* group = &testGroups[g];
    if (group->count == 0) continue;

    JUST_LOG_INFO("\n--- [TEST GROUP %zu] ---", g);

    for (size_t i = 0; i < group->count; ++i) 
    {
      justTestEntry test = group->tests[i];
      totalTests++;

      JustTestResult result          = JUST_TEST_FAIL;
      bool            executedSafely  = true;

    #if !defined(_WIN32)
      executedSafely = runTestForked(test, &result);
    #else
      result = test.func();
    #endif

      if (!executedSafely) 
      {
        crashedTests++;
        JUST_LOG_ERROR("  [CRASHED] %s", test.description);
      } 
      else if (result == JUST_TEST_PASS) 
      {
        passedTests++;
        JUST_LOG_INFO("  [PASS]    %s", test.description);
      } 
      else if (result == JUST_TEST_SKIP) 
      {
        skippedTests++;
        JUST_LOG_WARNING("  [SKIP]    %s", test.description);
      } 
      else 
      {
        failedTests++;
        JUST_LOG_ERROR("  [FAIL]    %s", test.description);
      }
    }
  }

  JUST_LOG_INFO("\n==================================================");
  JUST_LOG_INFO("                  TEST SUMMARY                    ");
  JUST_LOG_INFO("==================================================");
  JUST_LOG_INFO(" Total Executed : %zu", totalTests);
  JUST_LOG_INFO(" Passed         : %zu", passedTests);
  
  if (skippedTests > 0) JUST_LOG_WARNING(" Skipped        : %zu", skippedTests);
  if (failedTests  > 0) JUST_LOG_ERROR(" Failed         : %zu", failedTests);
  if (crashedTests > 0) JUST_LOG_ERROR(" Crashed        : %zu", crashedTests);

  JUST_LOG_INFO("==================================================\n");

  return (failedTests + crashedTests);
}

#endif // JUST_LIB_IMPL_TESTING
