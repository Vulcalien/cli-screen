/* Copyright 2021 Vulcalien
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * version: 0.3.0-WIP
 */
#ifndef VULC_CLISCREEN_CORE
#define VULC_CLISCREEN_CORE

#include "vulcalien/vulcalien.h"

struct screen;

/* Create the screen object.
 * Call screen_destroy to destroy it. */
extern struct screen *screen_create(void);

/* Destroy the screen object and set the pointer to NULL */
extern void screen_destroy(struct screen **scr);

/* Set the screen size. */
extern void screen_setsize(struct screen *scr, u32 w, u32 h);

extern void screen_render(struct screen *scr);

/* Set the ignored character.
 * The ignored character is not rendered.
 * Set to '\0' to disable ignored character. */
extern void screen_ignored_char(struct screen *scr, char c);

extern void screen_clear  (struct screen *scr,
                           char c, const char *color);
extern void screen_setchar(struct screen *scr, u32 x, u32 y,
                           char c, const char *color);
extern void screen_puts   (struct screen *scr, u32 x, u32 y,
                           const char *str, const char *color);
extern void screen_printf (struct screen *scr, u32 x, u32 y,
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

#endif // VULC_CLISCREEN_CORE
