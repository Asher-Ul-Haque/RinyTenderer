/**
 * @file : signal.h
 * @brief : Detects signals from the os
*/

#pragma once
#include <justLibrary.h>


/**
 * SIGINT : Signal Interrupt - Stop what you are doing
 * Generated : Cntrl + C
 * Intent : tell the renderer that you want to quit politely
 */
JUST_LOCAL void signalHandleSIGINT   (void);

/**
 * SIGTERM : Signal Terminate - Shut down cleanly
 * Generated : kill <pid>
 * Intent : do a clean shut down
 */
JUST_LOCAL void signalHandleSIGTERM  (void);

/**
 * SIGWINCH : Singal window resize
 */
JUST_LOCAL void signalHandleSIGWINCH (void);

/**
 * @brief : Handle all the signals
 * @param SIGNAL : The OS level signal
 */
JUST_LOCAL void signalCatchAll(int32_t SIGNAL);

/**
 * @brief : Registers all signal handlers with the OS
 * @return : True if signal handlers were accepted by OS, else crash without returning
 */
JUST_LOCAL bool signalBootHandlers(void);
