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
 */
#include "cliscreen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "private/scrbuffer.h"
#include "private/terminal.h"

#define PRINTF_TMP_SIZE (256)

struct screen {
    int w;
    int h;

    int raster_size;

    int align_x;
    int align_y;

    char *raster;
    const char **colors;

    struct scrbuffer *buf;

    char ignored_char;
    bool should_clear_term;
};

static struct screen *scr = NULL;

static struct terminal_size last_term_size = { .w = 0, .h = 0 };

EXPORT int cliscreen_create(void) {
    // calloc sets everything to 0 or (if pointer) NULL
    scr = calloc(1, sizeof(struct screen));

    if(!scr)
        return -1;

    scr->align_x = CLISCREEN_ALIGN_X_CENTER;
    scr->align_y = CLISCREEN_ALIGN_Y_MIDDLE;

    return 0;
}

static void cliscreen_free_memory(void) {
    if(scr->raster) {
        free(scr->raster);
        scr->raster = NULL;
    }

    if(scr->colors) {
        free(scr->colors);
        scr->colors = NULL;
    }

    if(scr->buf) {
        scrbuffer_destroy(scr->buf);
        scr->buf = NULL;
    }
}

EXPORT void cliscreen_destroy(void) {
    if(scr) {
        cliscreen_free_memory();
        free(scr);
        scr = NULL;
    }
}

EXPORT void cliscreen_setsize(int w, int h) {
    cliscreen_free_memory();

    int raster_size = w * h;

    scr->w = w;
    scr->h = h;

    scr->raster_size = raster_size;

    scr->raster = calloc(raster_size, sizeof(char));
    scr->colors = calloc(raster_size, sizeof(const char *));

    scr->buf = scrbuffer_create(raster_size);

    scr->should_clear_term = true;
}

EXPORT void cliscreen_setalign(int align_x, int align_y) {
    scr->align_x = align_x;
    scr->align_y = align_y;

    scr->should_clear_term = true;
}

EXPORT void cliscreen_render(void) {
    struct terminal_size term_size = cliscreen_terminal_size();

    // if the terminal dimension changed, clear the terminal
    if(term_size.w != last_term_size.w
       || term_size.h != last_term_size.h) {
        last_term_size = term_size;

        scr->should_clear_term = true;
    }

    if(scr->should_clear_term) {
        scr->should_clear_term = false;

        // "\033[H" - move to top left corner
        // "\033[J" - clear (delete from cursor to end of screen)
        scrbuffer_puts(scr->buf, "\033[H" "\033[J");
    }

    const char *last_color = NULL;

    int x0;
    switch(scr->align_x) {
        case CLISCREEN_ALIGN_X_LEFT:
            x0 = 1;
            break;
        case CLISCREEN_ALIGN_X_CENTER:
            x0 = (term_size.w - scr->w) / 2;
            break;
        case CLISCREEN_ALIGN_X_RIGHT:
            x0 = term_size.w - scr->w;
            break;
    }

    int y0;
    switch(scr->align_y) {
        case CLISCREEN_ALIGN_Y_TOP:
            y0 = 1;
            break;
        case CLISCREEN_ALIGN_Y_MIDDLE:
            y0 = (term_size.h - scr->h) / 2;
            break;
        case CLISCREEN_ALIGN_Y_BOTTOM:
            y0 = term_size.h - scr->h;
            break;
    }

    for(int y = 0; y < scr->h; y++) {
        scrbuffer_printf(scr->buf, "\033[%d;%dH", y0 + y, x0);

        for(int x = 0; x < scr->w; x++) {
            int i = x + y * scr->w;

            char chr = scr->raster[i];
            const char *col = scr->colors[i];

            // if color is different, reset and print the new one
            if(col != last_color) {
                // "\033[m" - reset color
                scrbuffer_puts(scr->buf, "\033[m");
                if(col != NULL)
                    scrbuffer_puts(scr->buf, col);

                last_color = col;
            }

            scrbuffer_putc(scr->buf, chr);
        }
    }
    if(last_color != NULL) {
        // "\033[m" - reset color
        scrbuffer_puts(scr->buf, "\033[m");
    }
    scrbuffer_flush(scr->buf);
}

EXPORT void cliscreen_ignored_char(char c) {
    scr->ignored_char = c;
}

EXPORT void cliscreen_clear(char c, const char *color) {
    for(int i = 0; i < scr->raster_size; i++) {
        scr->raster[i] = c;
        scr->colors[i] = color;
    }
}

EXPORT void cliscreen_setchar(int x, int y, char c, const char *color) {
    if(x < 0 || x >= scr->w) return;
    if(y < 0 || y >= scr->h) return;

    scr->raster[x + y * scr->w] = c;
    scr->colors[x + y * scr->w] = color;
}

EXPORT void cliscreen_fill(int x0, int y0, int x1, int y1,
                           char c, const char *color) {
    for(int yi = y0; yi <= y1; yi++) {
        if(yi < 0 || yi >= scr->h) break;
        for(int xi = x0; xi <= x1; xi++) {
            if(xi < 0 || xi >= scr->w) break;

            cliscreen_setchar(xi, yi, c, color);
        }
    }
}

EXPORT void cliscreen_puts(int x, int y,
                           const char *str, const char *color) {
    int len = strlen(str);

    int xoff = 0;
    int yoff = 0;
    for(int i = 0; i < len; i++) {
        char c = str[i];

        if(c == '\n') {
            xoff = 0;
            yoff++;
        } else {
            if(c != scr->ignored_char)
                cliscreen_setchar(x + xoff, y + yoff, c, color);
            xoff++;
        }
    }
}

EXPORT void cliscreen_printf(int x, int y,
                             const char *color,
                             const char *format, ...) {
    va_list args;
    va_start(args, format);

    char tmp[PRINTF_TMP_SIZE] = {0};
    vsnprintf(tmp, PRINTF_TMP_SIZE, format, args);

    va_end(args);

    cliscreen_puts(x, y, tmp, color);
}
