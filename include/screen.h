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

#define SCREEN_ALIGN_X_LEFT   (0)
#define SCREEN_ALIGN_X_CENTER (1)
#define SCREEN_ALIGN_X_RIGHT  (2)

#define SCREEN_ALIGN_Y_TOP    (0)
#define SCREEN_ALIGN_Y_MIDDLE (1)
#define SCREEN_ALIGN_Y_BOTTOM (2)

/* Create the screen.
 * It must be destroyed by calling screen_destroy. */
extern void screen_create(void);

/* Destroy the screen object and free the memory used by it. */
extern void screen_destroy(void);

/* Set the screen's size. */
extern void screen_setsize(u32 w, u32 h);

/* Set the screen's alignment. */
extern void screen_setalign(u32 align_x, u32 align_y);

/* Draw the screen. */
extern void screen_render(void);

/* Set the character ignored by the screen.
 * The ignored character is considered transparent,
 * so it is not rendered.
 * Set to '\0' to disable the ignored character. */
extern void screen_ignored_char(char c);

/* Clear the screen. */
extern void screen_clear  (char c, const char *color);
/* Set a character at the given position in the screen. */
extern void screen_setchar(u32 x, u32 y, char c, const char *color);
/* Put a string at the given position in the screen. */
extern void screen_puts   (u32 x, u32 y,
                           const char *str, const char *color);
/* Put a formatted string at the given position in the screen. */
extern void screen_printf (u32 x, u32 y,
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
extern void screen_terminal_prepare(void);

/* Undo screen_terminal_prepare. */
extern void screen_terminal_reset(void);

/* Return the terminal's current width. */
extern u32 screen_terminal_width(void);

/* Return the terminal's current height. */
extern u32 screen_terminal_height(void);

#endif // VULC_CLISCREEN_CORE
