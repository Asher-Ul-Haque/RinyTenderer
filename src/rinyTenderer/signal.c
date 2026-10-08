#include <rinyTenderer/signal.h>
#include <signal.h>
#include <unistd.h>

JUST_LOCAL void singalHandleSIGINT(void)   { TODO }
JUST_LOCAL void signalHandleSIGQUIT(void)  { TODO }
JUST_LOCAL void signalHandleSIGTERM(void)  { TODO }
JUST_LOCAL void signalHandleSIGWINCH(void) { TODO }

JUST_LOCAL void signalCatchAll(int32_t SIGNAL)
{
  JUST_LOG_WARNING("[SIGNAL HANDLER] : OS sent a signal : %d", SIGNAL);

  switch (SIGNAL)
  {
    case SIGINT:
      JUST_LOG_DEBUG("[SIGNAL HANDLER] : Received : SIGINT");
      singalHandleSIGINT();
      break;

    case SIGTERM:
      JUST_LOG_DEBUG("[SIGNAL HANDLER] : Received : SIGTERM");
      signalHandleSIGTERM();
      break;

    case SIGWINCH:
      JUST_LOG_DEBUG("[SIGNAL HANDLER] : Received : SIGWINCH");
      singalHandleSIGINT();
      break;

    default: TODO
  }
}

JUST_LOCAL bool signalBootHandlers(void)
{
  JUST_LOG_DEBUG("[SIGNAL HANDLER] : Trying to boot handlers");

  // - - - get the action struct 
  struct sigaction sa;
  sa.sa_handler   = signalCatchAll; // - - - Register our function
  sa.sa_flags     = 0;              // - - - Standard behavior

  // - - - Register handles
  JUST_ASSERT_MESSAGE(sigaction(SIGINT, &sa, NULL)   != -1, "[SIGNAL HANDLER] : Failed to register SIGINT handler");
  JUST_ASSERT_MESSAGE(sigaction(SIGTERM, &sa, NULL)  != -1, "[SIGNAL HANDLER] : Failed to register SIGTERM handler");
  JUST_ASSERT_MESSAGE(sigaction(SIGWINCH, &sa, NULL) != -1, "[SIGNAL HANDLER] : Failed to register SIGWINCH handler");

  JUST_LOG_INFO("[SIGNAL HANDLER] : Handlers registered with OS");
  return true;
}
