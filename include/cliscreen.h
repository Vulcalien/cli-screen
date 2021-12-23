/* Copyright 2021 Vulcalien
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
 * version: 0.4.1-WIP
 */
#ifndef VULC_CLISCREEN_CORE
#define VULC_CLISCREEN_CORE

#include "vulcalien/vulcalien.h"

#define CLISCREEN_ALIGN_X_LEFT   (0)
#define CLISCREEN_ALIGN_X_CENTER (1)
#define CLISCREEN_ALIGN_X_RIGHT  (2)

#define CLISCREEN_ALIGN_Y_TOP    (0)
#define CLISCREEN_ALIGN_Y_MIDDLE (1)
#define CLISCREEN_ALIGN_Y_BOTTOM (2)

/* Create the screen.
 * It must be destroyed by calling cliscreen_destroy. */
extern void cliscreen_create(void);

/* Destroy the screen object and free the memory used by it. */
extern void cliscreen_destroy(void);

/* Set the screen's size. */
extern void cliscreen_setsize(u32 w, u32 h);

/* Set the screen's alignment. */
extern void cliscreen_setalign(u32 align_x, u32 align_y);

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
extern void cliscreen_setchar(u32 x, u32 y, char c, const char *color);

/* Fill from (x0,y0) to (x1,y1) */
extern void cliscreen_fill(u32 x0, u32 y0, u32 x1, u32 y1,
                           char c, const char *color);

/* Put a string at the given position in the screen. */
extern void cliscreen_puts(u32 x, u32 y,
                           const char *str, const char *color);
/* Put a formatted string at the given position in the screen. */
extern void cliscreen_printf(u32 x, u32 y,
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
extern u32 cliscreen_terminal_width(void);

/* Return the terminal's current height. */
extern u32 cliscreen_terminal_height(void);

#endif // VULC_CLISCREEN_CORE
