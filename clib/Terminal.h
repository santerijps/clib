#ifndef TERMINAL_H
#define TERMINAL_H

/**
 * Reference:
 *  - Escape sequences cheat sheet: https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
 *  - Microsoft: https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
 *  - Box drawing: https://theasciicode.com.ar/extended-ascii-code/graphic-character-low-density-dotted-ascii-code-176.html
 */

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_WINDOWS
#include <windows.h>
#endif

#define term_run(Cmd, ...) printf("\033[" Cmd, ##__VA_ARGS__)

// GENERAL

#define term_play_bell_sound() term_run("\a")

#define term_backspace() printf("\b")
#define term_newline() printf("\n")
#define term_formfeed() printf("\f")
#define term_carriage_return() printf("\r")

#define term_tab_horizontal() printf("\t")
#define term_tab_vertical() printf("\v")

// CURSOR POSITION

#define term_cursor_pos_set_home() term_run("H")
#define term_cursor_pos_set(Line, Col) term_run("%d;%dH", Line, Col)

#define term_cursor_move_up(N) term_run("%dA", N)
#define term_cursor_move_down(N) term_run("%dB", N)
#define term_cursor_move_right(N) term_run("%dC", N)
#define term_cursor_move_left(N) term_run("%dD", N)

#define term_cursor_move_down_home(N) term_run("%dE", N)
#define term_cursor_move_up_home(N) term_run("%dF", N)

#define term_cursor_col_set(Col) term_run("%dG", Col)

#define term_cursor_pos_request() term_run("6n")

#define term_cursor_scroll_up() term_run("M")

#define term_cursor_save() term_run("s")
#define term_cursor_restore() term_run("u")

// ERASE

#define term_screen_erase_after_cursor() term_run("0J")
#define term_screen_erase_before_cursor() term_run("1J")
#define term_screen_erase() term_run("2J")

#define term_saved_lines_erase() term_run("3J")

#define term_line_erase_after_cursor() term_run("0K")
#define term_line_erase_before_cursor() term_run("1K")
#define term_line_erase() term_run("2K")

// PRIVATE MODES

#define term_cursor_hide() term_run("?25l")
#define term_cursor_show() term_run("?25h")

#define term_screen_restore() term_run("?47l")
#define term_screen_save() term_run("?47h")

#define term_alt_buffer_disable() term_run("?1049l")
#define term_alt_buffer_enable() term_run("?1049h")

// GRAPHICS ESC[1;34;{...}m

#define term_graphics_reset() term_run("0m");

#define term_graphics_bold_set() term_run("1m");
#define term_graphics_bold_reset() term_run("22m");

#define term_graphics_dim_set() term_run("2m");
#define term_graphics_dim_reset() term_run("22m");

#define term_graphics_italic_set() term_run("3m");
#define term_graphics_italic_reset() term_run("23m");

#define term_graphics_underline_set() term_run("4m");
#define term_graphics_underline_reset() term_run("24m");

#define term_graphics_blinking_set() term_run("5m");
#define term_graphics_blinking_reset() term_run("25m");

#define term_graphics_inverse_set() term_run("7m");
#define term_graphics_inverse_reset() term_run("27m");

#define term_graphics_hidden_set() term_run("8m");
#define term_graphics_hidden_reset() term_run("28m");

#define term_graphics_strikethrough_set() term_run("9m");
#define term_graphics_strikethrough_reset() term_run("29m");

// COLORS

#define TERM_COLOR_FG_BLACK 30
#define TERM_COLOR_BG_BLACK 40

#define TERM_COLOR_FG_RED 31
#define TERM_COLOR_BG_RED 41

#define TERM_COLOR_FG_GREEN 32
#define TERM_COLOR_BG_GREEN 42

#define TERM_COLOR_FG_YELLOW 33
#define TERM_COLOR_BG_YELLOW 43

#define TERM_COLOR_FG_BLUE 34
#define TERM_COLOR_BG_BLUE 44

#define TERM_COLOR_FG_MAGENTA 35
#define TERM_COLOR_BG_MAGENTA 45

#define TERM_COLOR_FG_CYAN 36
#define TERM_COLOR_BG_CYAN 46

#define TERM_COLOR_FG_WHITE 37
#define TERM_COLOR_BG_WHITE 47

#define TERM_COLOR_FG_DEFAULT 39
#define TERM_COLOR_BG_DEFAULT 49

#define term_color_set(Color) term_run("%dm", Color)
#define term_color_reset() term_graphics_reset()

// WINDOW

#define term_window_title_set(Title) printf("\033]0;%s\x07", Title)

typedef struct TerminalDimensions {
    short rows, cols;
} TerminalDimensions;

TerminalDimensions term_get_dimensions(void) {
    TerminalDimensions td;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    td.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    td.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return td;
}

void term_load_dimensions(TerminalDimensions *td) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    td->cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    td->rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#endif
