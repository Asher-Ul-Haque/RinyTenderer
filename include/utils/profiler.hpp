/**
  * 
  * @file profiler.hpp
  * @brief Lightweight profiling utilities for measuring execution time of code blocks.
  *
  * When `PROFILE_ENABLED` is defined, this module provides a simple profiling
  * system that records execution timings and writes them to a JSON file.
  * Profiling is typically used with scoped timers via the `PROFILE_SCOPE`
  * or `PROFILE` macros.
*/

#pragma once
#include <defines.hpp>
#ifdef PROFILE_ENABLED

#include <fstream>
#include <string>
#include <mutex>
#include <utils/dataTypes.hpp>
#include <utils/asserts.hpp>

namespace utils
{
  /**
    * @brief Represents a single profiling result.
    *
    * Contains timing information and metadata about a measured scope.
  */
  typedef struct ProfileResult
  {
    std::string name     = "Unknown"; ///< Name of the profiled scope.
    u64 start            = 0;         ///< Start timestamp (microseconds).
    u64 end              = 0;         ///< End timestamp (microseconds).
    u64 threadId         = 0;         ///< Identifier of the executing thread.
  } ProfileResult;


  /**
    * @brief Scoped timer used to measure execution time.
    *
    * The timer starts when constructed and records a profiling result
    * when destroyed. Typically used through the `PROFILE_SCOPE` macro.
  */
  class Timer
  {
  private:
    u64         startTime;
    u64         endTime;
    std::string name;

  public:
    /**
      * @brief Constructs and starts the timer.
      *
      * @param name Identifier for the profiled scope.
    */
    Timer(const std::string& name);

    /// @brief Destructor that finalizes timing and records the profile result.
    ~Timer();

    /// @brief Starts the timer.
    void start();

    /**
      * @brief Returns elapsed time since the timer started.
      *
      * @return Elapsed time in microseconds.
    */
    u64 elapsed();

    /**
      * @brief Returns the profiling result for the measured scope.
      *
      * @return A `ProfileResult` containing timing information.
    */
    ProfileResult elapsedResult();
  };


  /**
    * @brief Singleton profiler responsible for collecting profiling results.
    *
    * Writes profiling data to a JSON file so it can be visualized using
    * external profiling tools like chrome:://tracing. The profiler manages the output file and ensures proper formatting of the JSON data.
  */
  class Profiler
  {
  private:
    std::mutex    writeMutex;
    std::ofstream resultJsonFile;
    bool          firstProfile = true;

    Profiler();

    /// @brief Writes the JSON header for the profiling output file.
    void writeHeader();

    /// @brief Writes the JSON footer for the profiling output file.
    void writeFooter();

  public:
    /**
      * @brief Returns the global profiler instance.
      *
      * @return Reference to the singleton `Profiler`.
    */
    static Profiler& Instance();

    /**
      * @brief Records a profiling result.
      *
      * @param result Profiling data to write to the output file.
    */
    void writeProfile(const ProfileResult& result);

    /// @brief Destructor that finalizes the profiling session.
    ~Profiler();

    // Disable copying
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
  };


  /**
    * @brief Profiles the current scope.
    *
    * Creates a scoped timer with the provided name.
  */
  #define PROFILE_SCOPE(name) utils::Timer timer##__LINE__(name);

  /**
    * @brief Profiles the current function.
    *
    * Uses the function name as the profiling scope identifier.
  */
  #define PROFILE PROFILE_SCOPE(__func__)

}

#else

  /// @brief Disabled profiling scope macro when profiling is not enabled.
  #define PROFILE_SCOPE(name) 

  /// @brief Disabled profiling macro when profiling is not enabled.
  #define PROFILE 

#endif
