/* Copyright 2021-2023 Vulcalien
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3 only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * version: 0.5.0
 */
#ifndef VULC_CLISCREEN_CORE
#define VULC_CLISCREEN_CORE

#define CLISCREEN_ALIGN_X_LEFT   (0)
#define CLISCREEN_ALIGN_X_CENTER (1)
#define CLISCREEN_ALIGN_X_RIGHT  (2)

#define CLISCREEN_ALIGN_Y_TOP    (0)
#define CLISCREEN_ALIGN_Y_MIDDLE (1)
#define CLISCREEN_ALIGN_Y_BOTTOM (2)

/* Create the screen.
 * It must be destroyed by calling cliscreen_destroy.
 * Returns 0 on success, a nonzero value otherwise. */
extern int cliscreen_create(void);

/* Destroy the screen object and free the memory used by it. */
extern void cliscreen_destroy(void);

/* Set the screen's size. */
extern void cliscreen_setsize(int w, int h);

/* Set the screen's alignment. */
extern void cliscreen_setalign(int align_x, int align_y);

/* Draw the screen. */
extern void cliscreen_render(void);

/* Set the character ignored by the screen.
 * The ignored character is considered transparent,
 * so it is not rendered.
 * Set to '\0' to disable the ignored character. */
extern void cliscreen_ignored_char(char c);

/* Clear the screen. */
extern void cliscreen_clear(char c, const char *color);
/* Set a character at the given position in the screen. */
extern void cliscreen_setchar(int x, int y, char c, const char *color);

/* Fill from (x0,y0) to (x1,y1) */
extern void cliscreen_fill(int x0, int y0, int x1, int y1,
                           char c, const char *color);

/* Put a string at the given position in the screen. */
extern void cliscreen_puts(int x, int y,
                           const char *str, const char *color);
/* Put a formatted string at the given position in the screen. */
extern void cliscreen_printf(int x, int y,
                             const char *color,
                             const char *format, ...);

//
// TERMINAL functions
//

/* Prepare the terminal for the rendering.
 *
 * Operations:
 *   hide cursor,
 *   clear screen
 *   For Unix:
 *     disable ICANON
 *     disable ECHO
 *   For Windows:
 *     disable 'line input'
 *     disable 'echo input'
 *     enable  'virtual terminal'
 */
extern void cliscreen_terminal_prepare(void);

/* Undo cliscreen_terminal_prepare. */
extern void cliscreen_terminal_reset(void);

/* Return the terminal's current width. */
extern int cliscreen_terminal_width(void);

/* Return the terminal's current height. */
extern int cliscreen_terminal_height(void);

#endif // VULC_CLISCREEN_CORE
