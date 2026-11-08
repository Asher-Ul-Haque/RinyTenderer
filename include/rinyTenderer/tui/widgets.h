#include <rinyTenderer/window.h>
#include <rinyTenderer/_structs.h>
#include <justLibrary.h>
#include <stdint.h>

typedef enum tuiBoxType
{
  ROUNDED,    ///< Rounded borders 
  SHARP,      ///< Sharp borders
  HEAVY,      ///< Bold borders
  DOUBLE,     ///< Double border
  ASCII,      ///< ASCII only
  NONE        ///< Do not draw              
} TuiBoxType;

typedef enum tuiOrientation
{
  TUI_HORIZONTAL = 0,
  TUI_VERTICAL
} TuiOrientation;

typedef struct tuiBox
{
  FramePos        topLeft;      ///< The top left corner of the box
  FramePos        bottomRight;  ///< The bottom right corner of the box
  TerminalColor   color;        ///< The color of the box
  TuiBoxType      boxType;      ///< The border type of the box
} TuiBox;

typedef struct tuiText
{
  FramePos      position; ///< Top left of the text 
  const char*   text;     ///< The text as c str
  size_t        maxLen;   ///< Max length to read
  TerminalColor fgColor;  ///< Foreground color
  TerminalColor bgColor;  ///< Background color
} TuiText;

typedef struct tuiTextCentered
{
  FramePos      position; ///< Top left of the text 
  uint16_t      width;    ///< The width                        
  const char*   text;     ///< The text as c str
  size_t        maxLen;   ///< Max length to read
  TerminalColor fgColor;  ///< Foreground color
  TerminalColor bgColor;  ///< Background color
} TuiTextCentered;

typedef struct tuiProgressBar
{
  TerminalColor   fillColor;      ///< Which color to fill progres
  TerminalColor   emptyColor;     ///< Which color to fill the rest
  TerminalColor   textColor;      ///< If show percentage is enabled
  FramePos        position;       ///< The top left position
  TuiOrientation  orientation;    ///< The direction
  uint16_t        length;         ///< How long in pixels
  float           progress;       ///< What is the progress (0.0f to 1.0f)
  bool            showPercentage; ///< Whether to write the percentage
} TuiProgressBar;

typedef struct tuiButton
{
  TerminalColor fillColor;    ///< Fill color
  TerminalColor accentColor;  ///< Color for border, text, and press
  FramePos      topLeft;      ///< Top left pos
  FramePos      bottomRight;  ///< Bottom right pos
  TuiBoxType    borderType;   ///< Border type
  const char*   text;         ///< Button Label
  size_t        maxLen;       ///< How long to read the text
} TuiButton;
