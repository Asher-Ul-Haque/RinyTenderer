/**
 * @file : window.h
 * @brief : Handles terminal window related stuff
*/

#pragma once

#include "cglm/types-struct.h"
#include "cglm/types.h"
#include <justLibrary.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


typedef vec2s TerminalPos;
typedef vec2s TerminalPosRel;
typedef vec2s TerminalSize;
typedef vec2s TerminalSizeRel;

/// @brief : Enum for keys
typedef enum keyCode
{
  KEY_NONE  = 0,
  KEY_UNKNOWN,

  KEY_QUIT,
  KEY_SPACE,
  KEY_W,
  KEY_A,
  KEY_S,
  KEY_D,

  KEY_UP,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_LEFT,

  KEY_CODE_COUNT
} KeyCode;


/// @brief : Terminal context global storage
typedef struct terminalContext
{
  struct termios  origTermios;  ///< original terminal config 
  TerminalSize    size;         ///< Size of the temrinal
  TerminalSize    cursorPos;    ///< Where is cursor

  JustBitset      keys;                 ///< Keys

#ifdef DEBUG
  const char*     logFilePath;  ///< Log file path for just logger 
  FILE*           logFile;      ///< Log file for just logger
#endif
} TerminalCTX;


// - - - Size - - -

/// @brief : Updates the terminal size
JUST_API void         terminalUpdateSize(void);

/**
 * @brief  : Return the terminal size as struct
 * @return : Terminal size as struct
*/
JUST_API TerminalSize terminalGetSize(void);

/**
 * @brief : Return width 
 * @return : width as unsigned short
 */
JUST_API uint16_t     terminalGetWidth  (void);

/**
 * @brief : Return height
 * @return : Height as unsigned short
 */
JUST_API uint16_t     terminalGetHeight (void);

/// @brief : Checks key presses
JUST_API void         terminalPollInput(void);

/**
 * @brief : Tells whether a given key is pressed or not
 * @param KEY : The key to check
 * @return : true if pressed, false otherwise
 */
JUST_API bool         terminalIsKeyDown(KeyCode KEY);


// - - - Initialization - - -

/// @brief : Initializes the terminal
JUST_API void         terminalInit(void);

/// @brief : Shuts the terminal renderer down
JUST_API void         terminalShutdown(void);


// - - - Cursor - - -

/// @brief : Turns off echo
JUST_API static inline void terminalHideCursor(void)
{ fputs("\033[?25l", stdout); }

/// @brief : Turns on echo
JUST_API static inline void terminalShowCursor(void)
{ fputs("\033[?25h", stdout); }

/** @brief : Set cursor position
 * @param POSITION : The position of the terminal
 */
JUST_API void terminalSetCursor(TerminalSize POSITION);

/// @brief : Clear the terminal
JUST_API static inline void terminalClear(void)
{ fputs("\033[0m\033[2J\033[H", stdout); }

/// @brief : return terminal terminalContext
JUST_LOCAL const TerminalCTX* terminalGetContext(void);
