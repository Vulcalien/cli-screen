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
 */
#include "screen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "private/terminal.h"

#define PRINTF_TMP_SIZE (256)

struct screen {
    u32 w;
    u32 h;

    u32 buffer_size;
    char *buffer;
    const char **colors;
};

// static u32 last_term_w;
// static u32 last_term_h;
static struct screen_terminal_size last_term_size;

struct screen *screen_create(u32 w, u32 h) {
    struct screen *scr = malloc(sizeof(struct screen));

    u32 buffer_size = w * h;

    *scr = (struct screen) {
        .w = w,
        .h = h,

        .buffer_size = buffer_size,

        .buffer = calloc(buffer_size, sizeof(char)),
        .colors = calloc(buffer_size, sizeof(const char *))
    };
    return scr;
}

void screen_destroy(struct screen **scr) {
    free((*scr)->buffer);
    free((*scr)->colors);
    free(*scr);

    *scr = NULL;
}

void screen_render(struct screen *scr) {
    struct screen_terminal_size term_size = screen_terminal_size();

    if(term_size.w != last_term_size.w
       || term_size.h != last_term_size.h) {
        last_term_size = term_size;

        // if the terminal dimension changed, clear the screen
        fputs("\033[H", stdout); // move to top left corner
        fputs("\033[J", stdout); // clear (delete from cursor to end of screen)
    }

    const char *last_color = NULL;

    // top-left is 1;1
    u32 x0 = 1 + (term_size.w - scr->w) / 2;
    u32 y0 = 1 + (term_size.h - scr->h) / 2;

    for(u32 y = 0; y < scr->h; y++) {
        fprintf(stdout, "\033[%d;%dH", y0 + y, x0);

        for(u32 x = 0; x < scr->w; x++) {
            u32 i = x + y * scr->w;

            char chr = scr->buffer[i];
            const char *col = scr->colors[i];

            // if color is different, reset and print the new one
            if(col != last_color) {
                fputs("\033[m", stdout); // reset color
                if(col != NULL) {
                    fputs(col, stdout);
                }

                last_color = col;
            }

            fputc(chr, stdout);
        }
    }
    fflush(stdout);
}

void screen_clear(struct screen *scr,
                  char c, const char *color) {
    for(u32 i = 0; i < scr->buffer_size; i++) {
        scr->buffer[i] = c;
        scr->colors[i] = color;
    }
}

void screen_setchar(struct screen *scr, u32 x, u32 y,
                    char c, const char *color) {
    if(x < 0 || x >= scr->w) return;
    if(y < 0 || y >= scr->h) return;

    scr->buffer[x + y * scr->w] = c;
    scr->colors[x + y * scr->w] = color;
}

void screen_puts(struct screen *scr, u32 x, u32 y,
                 const char *str, const char *color) {
    u32 len = strlen(str);

    u32 xoff = 0;
    u32 yoff = 0;
    for(u32 i = 0; i < len; i++) {
        char c = str[i];

        if(c == '\n') {
            xoff = 0;
            yoff++;
        } else {
            screen_setchar(scr, x + xoff, y + yoff, c, color);
            xoff++;
        }
    }
}

void screen_printf(struct screen *scr, u32 x, u32 y,
                   const char *color,
                   const char *format, ...) {
    va_list args;
    va_start(args, format);

    char tmp[PRINTF_TMP_SIZE] = {0};
    vsnprintf(tmp, PRINTF_TMP_SIZE, format, args);

    va_end(args);

    screen_puts(scr, x, y, tmp, color);
}
