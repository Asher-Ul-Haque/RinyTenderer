#include <justLibrary.h>
#include <rinyTenderer/sample.h>
#include <rinyTenderer/window.h>
#include <stdio.h>
#include <unistd.h>

int32_t main(void)
{
  boot();
  terminalInit();

  terminalSetCursor( (TerminalSize){.cols = 5, .rows = 10});
  fputs("Hello at Row 5, col 10!", stdout);
  fflush(stdout);
  sleep(2); // Wait 2 seconds

  terminalSetCursor( (TerminalSize){.cols = 5, .rows = 10});
  fputs("OVERWRITTEN!           ", stdout);
  fflush(stdout);
  sleep(2); // Wait 2 seconds

  terminalShutdown();
  return 0;
}
