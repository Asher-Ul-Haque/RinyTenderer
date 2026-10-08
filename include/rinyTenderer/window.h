/**
 * @file : window.h
 * @brief : Handles terminal window related stuff
*/

#pragma once

#include <justLibrary.h>
#include <stdint.h>


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
