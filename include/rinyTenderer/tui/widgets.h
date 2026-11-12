#include <rinyTenderer/window.h>
#include <rinyTenderer/_structs.h>
#include <justLibrary.h>
#include <stdint.h>


/// @brief : What type of border
typedef enum tuiBoxType
{
  TUI_BORDER_ROUNDED,    ///< Rounded borders 
  TUI_BORDER_SHARP,      ///< Sharp borders
  TUI_BORDER_HEAVY,      ///< Bold borders
  TUI_BORDER_DOUBLE,     ///< Double border
  TUI_BORDER_ASCII,      ///< ASCII only
  TUI_BORDER_NONE        ///< Do not draw              
} TuiBoxType;

/// @brief : The orientation of the text
typedef enum tuiOrientation
{
  TUI_HORIZONTAL = 0,
  TUI_VERTICAL
} TuiOrientation;

/// @brief : Representation of all the info needed for a tui box
typedef struct tuiBox
{
  FramePos        topLeft;      ///< The top left corner of the box
  FramePos        bottomRight;  ///< The bottom right corner of the box
  TerminalColor   color;        ///< The color of the box
  TuiBoxType      boxType;      ///< The border type of the box
} TuiBox;

/// @brief ; Representation of all the info needed for a tui text
typedef struct tuiText
{
  FramePos      position; ///< Top left of the text 
  const char*   text;     ///< The text as c str
  size_t        maxLen;   ///< Max length to read
  uint16_t      width;    ///< Maximum line width before auto-wrapping (0 = no wrap)
  TerminalColor fgColor;  ///< Foreground color
  TerminalColor bgColor;  ///< Background color
} TuiText;

/// @brief : Representation of all the info needed for a tui text centered 
typedef struct tuiTextCentered
{
  FramePos      position; ///< Top left of the text 
  uint16_t      width;    ///< The width                        
  const char*   text;     ///< The text as c str
  size_t        maxLen;   ///< Max length to read
  TerminalColor fgColor;  ///< Foreground color
  TerminalColor bgColor;  ///< Background color
} TuiTextCentered;

/// @brief : Representation of all the info needed for a tui progress bar
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

/// @brief : Representation of all the info needed for a tui button
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

/// @brief : Representation of all the info needed for a tui checkbox
typedef struct tuiCheckbox
{
  FramePos       position;    ///< Top-left sub-pixel position
  const char*    label;       ///< Text label rendered to the right
  bool*          checked;     ///< Pointer to persistent boolean state
  TerminalColor  boxColor;    ///< Color for the ballot box symbol
  TerminalColor  textColor;   ///< Color for the label text
} TuiCheckbox;

/// @brief : Representation of all the info needed for a tui slider
typedef struct tuiSlider
{
  FramePos       position;     ///< Top-left sub-pixel position
  uint16_t       length;       ///< Length of the slider track
  TuiOrientation orientation;  ///< Horizontal or Vertical
  const char*    label;        ///< Variable name / label
  size_t         maxLen;       ///< Maximum length for the label
  float*         value;        ///< Pointer to live float variable
  float          minValue;     ///< Minimum bound
  float          maxValue;     ///< Maximum bound
  TerminalColor  fillColor;    ///< Active bar color
  TerminalColor  emptyColor;   ///< Unfilled track color
  TerminalColor  accentColor;  ///< Border and text color
  bool           showValue;    ///< Whether to display the value and label
} TuiSlider;
