/**
 * @file : window.h
 * @brief : Handles terminal window related stuff
*/

#pragma once

#include <justLibrary.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


/// @brief : Represents size of the terminal window
typedef struct terminalSize
{
  union
  {
    uint16_t rows;    ///< Height in character cells
    uint16_t height;  ///< Height in character cells
  };

  union
  {
    uint16_t cols;    ///< Width in character cells
    uint16_t width;   ///< Widht in character cells
  };
} TerminalSize;



/// @brief : Terminal context global storage
typedef struct terminalContext
{
  struct termios  origTermios;  ///< original terminal config 
  TerminalSize    size;         ///< Size of the temrinal
  TerminalSize    cursorPos;    ///< Where is cursor

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
 * @brief : Return number of rows
 * @return : Number of rows as unsigned short
 */
JUST_API uint16_t     terminalGetRows   (void);

/**
 * @brief : Return number of cols
 * @return : Number of cols as unsigned short
 */
JUST_API uint16_t     terminalGetCols   (void);

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


// - - - Initialization - - -

/// @brief : Initializes the terminal
JUST_API void terminalInit(void);

/// @brief : Shuts the terminal renderer down
JUST_API void terminalShutdown(void);


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
{ fputs("\033[2J\033[H", stdout); }

/// @brief : return terminal terminalContext
JUST_LOCAL const TerminalCTX* terminalGetContext(void);
