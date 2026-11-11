/**
 * @file : window.h
 * @brief : Handles terminal window related stuff
*/

#pragma once

#include <cglm/types-struct.h>
#include <justLibrary.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


/// @brief : terminal pos is a 2d vector 
typedef vec2s TerminalPos;

/// @brief : terminal size is a 2d vector
typedef vec2s TerminalSize;

/// @brief : terminal pos is a 2d vector 
typedef vec2s TerminalPosRel;

/// @brief : terminal size is a 2d vector
typedef vec2s TerminalSizeRel;

/// @brief : Enum for keys
typedef enum keyCode
{
  KEY_NONE  = 0,
  KEY_UNKNOWN,

  // - - - Alphabet - - -
  KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, 
  KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, 
  KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, 
  KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, 
  KEY_Y, KEY_Z,

  // - - - Top Row Numbers (0-9) - - -
  KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
  KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

  // - - - Function Keys (F1-F12) - - -
  KEY_F1, KEY_F2,  KEY_F3,  KEY_F4,
  KEY_F5, KEY_F6,  KEY_F7,  KEY_F8,
  KEY_F9, KEY_F10, KEY_F11, KEY_F12,

  // - - - Navigation & Editing - - -
  KEY_ENTER,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_ESCAPE,
  KEY_SPACE,
  KEY_INSERT,
  KEY_DELETE,
  KEY_HOME,
  KEY_END,
  KEY_PAGE_UP,
  KEY_PAGE_DOWN,

  // - - - Arrow Keys - - -
  KEY_UP,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_LEFT,

  // - - - Modifiers - - -
  KEY_LEFT_SHIFT,
  KEY_RIGHT_SHIFT,
  KEY_LEFT_CTRL,
  KEY_RIGHT_CTRL,
  KEY_LEFT_ALT,
  KEY_RIGHT_ALT,

  // - - - Punctuation & Symbols - - -
  KEY_MINUS,
  KEY_EQUALS,
  KEY_LEFT_BRACKET,
  KEY_RIGHT_BRACKET,
  KEY_BACKSLASH,
  KEY_SEMICOLON,
  KEY_APOSTROPHE,
  KEY_GRAVE,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_SLASH,

  KEY_CODE_COUNT
} KeyCode;

/// @brief : Enum for Mouse buttons 
typedef enum mouseButton
{
  MOUSE_BUTTON_LEFT = 0,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_SCROLL_UP,
  MOUSE_BUTTON_SCROLL_DOWN,
  MOUSE_BUTTON_COUNT
} MouseButton;

/// @brief : In a frame we are tracking at max 32 chars 
#define MAX_FRAME_CHARS 32

/// @brief : Terminal context global storage
typedef struct terminalContext
{
  struct termios  origTermios;  ///< original terminal config 
  TerminalSize    size;         ///< Size of the temrinal
  TerminalSize    cursorPos;    ///< Where is cursor

  JustBitset      keys;                            ///< Keys
  char            textBuffer[MAX_FRAME_CHARS + 1]; ///< Text
  size_t          textLength;                      ///< strlen(textBuffer)   

  TerminalPos     mousePos;         ///< Current mouse terminal position
  JustBitset      mouseButtons;     ///< Active mouse buttons this frame 
  int             scrollDelta;      ///< Scroll wheel delta this frame

#ifdef DEBUG
  const char*     logFilePath;  ///< Log file path for just logger 
  FILE*           logFile;      ///< Log file for just logger
#endif
} TerminalCTX;


// - - - Size - - -

/// @brief : Updates the terminal size, call whenever window is resized
JUST_API void terminalUpdateSize(void);

/**
 * @brief  : Return the terminal size as struct
 * @return : Terminal size as struct
*/
JUST_API TerminalSize terminalGetSize(void);

/**
 * @brief : Return width 
 * @return : width as unsigned short
 */
JUST_API uint16_t terminalGetWidth(void);

/**
 * @brief : Return height
 * @return : Height as unsigned short
 */
JUST_API uint16_t terminalGetHeight(void);


// - - - Input - - -

/// @brief : Checks key presses and mouse button clicks
JUST_API void terminalPollInput(void);

/**
 * @brief : Tells whether a given key is pressed or not
 * @param KEY : The key to check
 * @return : true if pressed, false otherwise
 */
JUST_API bool terminalIsKeyDown(KeyCode KEY);

/**
 * @brief : Returns the character sequence typed in this frame
 * @return : c string
 */
JUST_API const char* terminalGetText(void);

/**
 * @brief : Returns how many chars were typed
 * @return : c string length
 */
JUST_API size_t terminalGetTextLength(void);

/**
 * @brief : Tells whether a mouse button was pressed
 * @param BUTTON : WHich button?
 * @return : true if pressed, false otherwise
*/
JUST_API bool terminalIsMouseButtonDown(MouseButton BUTTON);

/**
 * @brief : Tells where the mouse position is
 * @warning : Updates only when mouse is clicked
*/
JUST_API TerminalPos terminalGetMousePos(void);

/**
 * @brief : Tells whether the scroll wheel is up, down or not moved
 * @return : 1 for up, -1 for down, 0 for same
*/
JUST_API int terminalGetScrollDelta(void);


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

/** 
 * @brief : Set cursor position
 * @param POSITION : The position of the terminal cursor
*/
JUST_API void terminalSetCursor(TerminalSize POSITION);

/// @brief : Clear the terminal
JUST_API static inline void terminalClear(void)
{ fputs("\033[0m\033[2J\033[H", stdout); }

/// @brief : return terminal terminalContext
JUST_LOCAL const TerminalCTX* terminalGetContext(void);
