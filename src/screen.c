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

#include "private/scrbuffer.h"
#include "private/terminal.h"

#define PRINTF_TMP_SIZE (256)

struct screen {
    u32 w;
    u32 h;

    u32 raster_size;

    char *raster;
    const char **colors;

    char ignored_char;

    struct scrbuffer *buf;
};

static struct terminal_size last_term_size;

struct screen *screen_create(u32 w, u32 h) {
    struct screen *scr = malloc(sizeof(struct screen));

    u32 raster_size = w * h;

    /* since there will be ANSI codes in the buffer, other
     * than the characters in the raster, 1 * raster_size is
     * surely not enought */
    u32 initial_buffer_size = 2 * raster_size;

    *scr = (struct screen) {
        .w = w,
        .h = h,

        .raster_size = raster_size,

        .raster = calloc(raster_size, sizeof(char)),
        .colors = calloc(raster_size, sizeof(const char *)),

        // '\0' means: no character is ignored
        .ignored_char = '\0',
    };

    scr->buf = malloc(sizeof(struct scrbuffer));
    *(scr->buf) = (struct scrbuffer) {
        .size = initial_buffer_size,
        .used = 0,
        .chr_buf  = malloc(initial_buffer_size * sizeof(char)),

        .inc_step = raster_size
    };
    return scr;
}

void screen_destroy(struct screen **scr) {
    free((*scr)->raster);
    free((*scr)->colors);

    // scrbuffer
    free((*scr)->buf->chr_buf);
    free((*scr)->buf);

    free(*scr);

    *scr = NULL;
}

void screen_render(struct screen *scr) {
    struct terminal_size term_size = screen_terminal_size();

    // if the terminal dimension changed, clear the screen
    if(term_size.w != last_term_size.w
       || term_size.h != last_term_size.h) {
        last_term_size = term_size;

        // "\033[H" - move to top left corner
        // "\033[J" - clear (delete from cursor to end of screen)
        screen_scrbuffer_puts(scr->buf, "\033[H" "\033[J");
    }

    const char *last_color = NULL;

    // top-left is 1;1
    u32 x0 = 1 + (term_size.w - scr->w) / 2;
    u32 y0 = 1 + (term_size.h - scr->h) / 2;

    for(u32 y = 0; y < scr->h; y++) {
        screen_scrbuffer_printf(scr->buf, "\033[%d;%dH", y0 + y, x0);

        for(u32 x = 0; x < scr->w; x++) {
            u32 i = x + y * scr->w;

            char chr = scr->raster[i];
            const char *col = scr->colors[i];

            // if color is different, reset and print the new one
            if(col != last_color) {
                screen_scrbuffer_puts(scr->buf, "\033[m"); // reset color
                if(col != NULL) {
                    screen_scrbuffer_puts(scr->buf, col);
                }

                last_color = col;
            }

            screen_scrbuffer_putc(scr->buf, chr);
        }
    }
    screen_scrbuffer_flush(scr->buf);
}

void screen_ignored_char(struct screen *scr, char c) {
    scr->ignored_char = c;
}

void screen_clear(struct screen *scr,
                  char c, const char *color) {
    for(u32 i = 0; i < scr->raster_size; i++) {
        scr->raster[i] = c;
        scr->colors[i] = color;
    }
}

void screen_setchar(struct screen *scr, u32 x, u32 y,
                    char c, const char *color) {
    if(x < 0 || x >= scr->w) return;
    if(y < 0 || y >= scr->h) return;

    scr->raster[x + y * scr->w] = c;
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
            if(c != scr->ignored_char) {
                screen_setchar(scr, x + xoff, y + yoff, c, color);
            }
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
