/* PDCurses */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <curspriv.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <ctype.h>

#include <sys/types.h>

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <Intrinsic.h>
#include <StringDefs.h>
#include <Shell.h>

#ifdef USE_XAW3D
# include <Xaw3d/Box.h>
# include <Xaw3d/Scrollbar.h>
#elif defined(USE_NEXTAW)
# include <neXtaw/Box.h>
# include <neXtaw/Scrollbar.h>
#else
# include <Xaw/Box.h>
# include <Xaw/Scrollbar.h>
#endif

#include "scrlbox.h"

#include "Xmu/StdSel.h"
#include "Xmu/Atoms.h"

#include <keysym.h>
#include <Xatom.h>

typedef struct
{
    int lines;
    int cols;
    Pixel cursorColor;
    Pixel colorBlack;
    Pixel colorRed;
    Pixel colorGreen;
    Pixel colorYellow;
    Pixel colorBlue;
    Pixel colorMagenta;
    Pixel colorCyan;
    Pixel colorWhite;
    Pixel colorBoldBlack;
    Pixel colorBoldRed;
    Pixel colorBoldGreen;
    Pixel colorBoldYellow;
    Pixel colorBoldBlue;
    Pixel colorBoldMagenta;
    Pixel colorBoldCyan;
    Pixel colorBoldWhite;
    Pixel pointerForeColor;
    Pixel pointerBackColor;
    XFontStruct *normalFont;
    XFontStruct *italicFont;
    XFontStruct *boldFont;
    char *bitmap;
    char *pixmap;
    Cursor pointer;
    int borderWidth;
    int borderColor;
    int clickPeriod;
    int doubleClickPeriod;
    int scrollbarWidth;
    int cursorBlinkRate;
    char *textCursor;
    int textBlinkRate;
} XCursesAppData;

extern XCursesAppData xc_app_data;

int PDC_display_cursor(int, int, int, int, int);

void XCursesCursor(int, int, int, int);
void XCursesDisplayCursor(void);
int XCursesInitscr(int, char **);
int XCursesSetupX(int, char **);
void XCursesExit(void);

void XC_blink_cursor(XtPointer, XtIntervalId *);
void XC_blink_text(XtPointer, XtIntervalId *);
void XC_draw_border(void);
void XC_redraw_cursor(void);
void XC_resize(void);
void XC_refresh_scrollbar(void);
void XC_scroll_up_down(Widget, XtPointer, XtPointer);
void XC_scroll_left_right(Widget, XtPointer, XtPointer);
void XC_thumb_up_down(Widget, XtPointer, XtPointer);
void XC_thumb_left_right(Widget, XtPointer, XtPointer);
void XC_set_blink(bool);

extern XtAppContext app_context;
extern Widget topLevel, drawing, scrollBox, scrollVert, scrollHoriz;

#define XCURSESDISPLAY (XtDisplay(drawing))
#define XCURSESWIN     (XtWindow(drawing))

extern Pixel colors[PDC_MAXCOL + 2];
extern GC normal_gc, rect_cursor_gc, italic_gc, bold_gc, border_gc;
extern int XCursesLINES;
extern int XCursesCOLS;
extern int font_height, font_width, font_ascent, font_descent;

#ifdef MOUSE_DEBUG
# define MOUSE_LOG(x) printf x
#else
# define MOUSE_LOG(x)
#endif

extern bool xc_resize_now;
extern XIM Xim;
extern XIC Xic;
extern bool blinked_off;
extern bool vertical_cursor;
extern bool visible_cursor;
extern bool window_entered;
