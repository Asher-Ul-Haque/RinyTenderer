#include <utils/asserts.hpp>
#include <utils/testManager.hpp>
#include <iostream>
#include <sstream>

#ifndef _WIN32
  #include <unistd.h>
  #include <sys/wait.h>
#endif

#ifndef _WIN32
  #define COL_RESET        "\033[0m"
  #define COL_RED          "\033[31m"
  #define COL_GREEN        "\033[32m"
  #define COL_YELLOW       "\033[33m"
  #define COL_WHITE_ON_RED "\033[41;97m"
#else
  #define COL_RESET        ""
  #define COL_RED          ""
  #define COL_GREEN        ""
  #define COL_YELLOW       ""
  #define COL_WHITE_ON_RED ""
#endif

namespace utils
{
  // - - - Internal state

  static std::vector<TestGroup> groups;


  void registerTest(TEST FUNCTION, const std::string& DESCRIPTION, u8 GROUP_ID)
  {
    if (groups.size() <= GROUP_ID) groups.resize(GROUP_ID + 1);
    groups[GROUP_ID].push_back({ FUNCTION, DESCRIPTION });
  }

  typedef struct TestStats
  {
    u32 total   = 0;
    u32 passed  = 0;
    u32 skipped = 0;
    u32 failed  = 0;
    u32 crashed = 0;

    std::vector<std::string> failedTests;
    std::vector<std::string> crashedTests;
    std::vector<std::string> skippedTests;
  } TestStats;


  // - - - no fork exec on windows, sadly
  #ifndef _WIN32
  static bool runTestForked(
    const TestEntry&  TEST, 
    std::string&      OUTPUT, 
    u8&               RESULT)
  {
    i32 pipefd[2];
    RUNTIME_ASSERT_MESSAGE(pipe(pipefd) == 0, "[TEST MANAGER] : Failed to pipe");

    pid_t pid = fork();
    if (pid == 0)
    {
      // - - - child
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      close(pipefd[1]);

      RESULT = TEST.func();
      std::exit(RESULT);
    }

    // - - - parent
    close(pipefd[1]);

    char buffer[32 * 1024];
    u64 n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) OUTPUT.append(buffer, buffer + n);

    close(pipefd[0]);

    i32 status = 0;
    waitpid(pid, &status, 0);

    if (WIFSIGNALED(status))    return false;

    RESULT = WEXITSTATUS(status);
    return true;
  }
  #endif


  void runTests()
  {
    TestStats          stats;
    std::ostringstream fullLog;

    for (const auto& group : groups)
    {
      for (const auto& test : group)
      {
        stats.total++;

        std::string output;
        u8 result = 0;

        #ifndef _WIN32
          bool ok = runTestForked(test, output, result);
          if (!ok)
          {
            stats.crashed++;
            stats.crashedTests.push_back(test.description);
            std::cout << COL_WHITE_ON_RED << test.description << " : CRASHED" << COL_RESET;
            fullLog << "[CRASHED] " << test.description << "\n";
            continue;
          }
        #else

          // - - - WARNING: Windows: no crash isolation
          std::streambuf*     old = std::cout.rdbuf();
          std::ostringstream  capture;
          std::cout.rdbuf(capture.rdbuf());

          result = test.func();

          std::cout.rdbuf(old);
          output = capture.str();
        #endif

        fullLog << output;

        if (result == PASS_TEST)  
        {
          stats.passed++;
          std::cout << COL_GREEN << test.description << " : SUCCESS" << COL_RESET << std::endl;
          fullLog << "[SUCCESS] " << test.description << "\n";
        }
        else if (result == SKIP_TEST) 
        {
          stats.skipped++;
          stats.skippedTests.push_back(test.description);
          std::cout << COL_YELLOW << test.description << " : SKIPPED" << COL_RESET << std::endl;
          fullLog << "[SKIPPED] " << test.description << "\n";
        }
        else  
        {
          stats.failed++;
          stats.failedTests.push_back(test.description);
          std::cout << COL_RED << test.description << " : FAILED" << COL_RESET << std::endl;
          fullLog << "[FAILED] " << test.description << "\n";
        }
      }
    }

    // - - - Summary

    std::cout << "\nTotal tests: " << stats.total << "\n"
              << COL_GREEN  << "Passed   : " << stats.passed  << "\n"
              << COL_YELLOW << "Skipped  : " << stats.skipped << "\n"
              << COL_RED    << "Failed   : " << stats.failed  << "\n";
    #ifndef _WIN32
      std::cout << COL_WHITE_ON_RED <<"Crashed  : "     << stats.crashed << COL_RESET << "\n";
    #endif

    
    if (stats.failed > 0)
    {
      std::cout << "\n" << COL_RED << "Failed Tests:\n" << COL_RESET;
      for (const auto& t : stats.failedTests)
        std::cout << COL_RED << "  - " << t << COL_RESET << "\n";
    }

    if (stats.skipped > 0)
    {
      std::cout << "\n" << COL_YELLOW << "Skipped Tests:\n" << COL_RESET;
      for (const auto& t : stats.skippedTests)
      std::cout << COL_YELLOW << "  - " << t << COL_RESET << "\n";
    }

    #ifndef _WIN32
      if (stats.crashed > 0)
      {
        std::cout << "\n" << COL_WHITE_ON_RED << "Crashed Tests:\n" << COL_RESET;
        for (const auto& t : stats.crashedTests)
          std::cout << COL_WHITE_ON_RED << "  - " << t << COL_RESET << "\n";
      }
    #endif
    
    std::cout << "\n\n\n- - - FULL LOG - - - \n\n" << fullLog.str();
  }
}
