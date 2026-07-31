/**
  * @file profiler
  * @brief Implementation of profiling utilities.
*/

#ifdef PROFILE_ENABLED
#include <algorithm>
#include <chrono>
#include <string>
#include <thread>
#include <utils/profiler.hpp>
#include <utils/logger.hpp>

namespace utils 
{
  // - - - Timer Implementation - - -

  #define NOW               std::chrono::high_resolution_clock::now() 
  #define NOW_LONG          std::chrono::time_point_cast<std::chrono::microseconds>(NOW) \
                              .time_since_epoch()                                        \
                              .count()
  #define THIS_THREAD_ID    std::this_thread::get_id()
  #define THIS_THREAD_LONG  std::hash<std::thread::id>{}(THIS_THREAD_ID)

  Timer::Timer(const std::string& NAME) : name(NAME) { start(); }

  Timer::~Timer() 
  {
    Profiler::Instance().writeProfile(elapsedResult());
  }

  void Timer::start()   { this->startTime = NOW_LONG; }
  u64  Timer::elapsed() { return NOW_LONG - this->startTime; }

  ProfileResult Timer::elapsedResult() 
  {
    this->endTime = NOW_LONG;

    return 
      {
        .name     = this->name,
        .start    = this->startTime,
        .end      = endTime,
        .threadId = THIS_THREAD_LONG
      };
  }

  // - - - Profiler Implementation - - -

  Profiler::Profiler() : firstProfile(true) 
  {
    resultJsonFile.open("result.json");
    writeHeader();
    LOG_INFO("[Profiler] Started profiling, outputting to result.json");
  }

  Profiler::~Profiler() 
  {
    writeFooter();
    resultJsonFile.close();
    LOG_INFO("[Profiler] Profiling data written to result.json");
  }

  Profiler& Profiler::Instance() 
  {
    static Profiler me;
    return me;
  }

  void Profiler::writeHeader() { resultJsonFile << "{\"otherData\": {}, \"traceEvents\": ["; }

  void Profiler::writeFooter() { resultJsonFile << "]}"; }

  void Profiler::writeProfile(const ProfileResult& RESULT) 
  {
    std::lock_guard<std::mutex> lock(writeMutex);
    if (!firstProfile)       resultJsonFile << ",";
    firstProfile = false;

    std::string cleanName = RESULT.name;
    std::replace(cleanName.begin(), cleanName.end(), '"', '\'');

    resultJsonFile << "{";
    resultJsonFile << "\"cat\":\"function\",";
    resultJsonFile << "\"dur\":" << (RESULT.end - RESULT.start) << ",";
    resultJsonFile << "\"name\":\"" << cleanName << "\",";
    resultJsonFile << "\"ph\":\"X\",";
    resultJsonFile << "\"pid\":0,";
    resultJsonFile << "\"tid\":" << RESULT.threadId << ",";
    resultJsonFile << "\"ts\":" << RESULT.start;
    resultJsonFile << "}";

    resultJsonFile.flush();
  }

  #undef NOW
  #undef THIS_THREAD_ID
  #undef THIS_THREAD_LONG
  #undef NOW_LONG
}

#endif
