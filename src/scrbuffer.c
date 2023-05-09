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
#include "private/scrbuffer.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef __unix__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
    #include "private/terminal.h"
#endif

#define PRINTF_TMP_SIZE (256)

/* Check if the buffer has enought space. If not, expand it. */
static void check_buffer(struct scrbuffer *buf, int requested_space);

struct scrbuffer *scrbuffer_create(int raster_size) {
    struct scrbuffer *buf = malloc(sizeof(struct scrbuffer));

    /* since there will be ANSI codes in the buffer, other
     * than the characters in the raster, 1 * raster_size is
     * surely not enought */
    int initial_size = 2 * raster_size;

    *buf = (struct scrbuffer) {
        .size = initial_size,
        .used = 0,
        .chr_buf = malloc(initial_size * sizeof(char)),

        .inc_step = raster_size
    };
    return buf;
}

void scrbuffer_destroy(struct scrbuffer **buf) {
    free((*buf)->chr_buf);
    free(*buf);

    *buf = NULL;
}

void scrbuffer_putc(struct scrbuffer *buf, char chr) {
    check_buffer(buf, 1);
    buf->chr_buf[buf->used] = chr;
    buf->used++;
}

void scrbuffer_puts(struct scrbuffer *buf, const char *str) {
    int len = strlen(str);

    check_buffer(buf, len);
    for(int i = 0; i < len; i++)
        buf->chr_buf[buf->used + i] = str[i];
    buf->used += len;
}

void scrbuffer_printf(struct scrbuffer *buf,
                      const char *format, ...) {
    va_list args;
    va_start(args, format);

    char tmp[PRINTF_TMP_SIZE] = { 0 };
    vsnprintf(tmp, PRINTF_TMP_SIZE, format, args);

    va_end(args);

    scrbuffer_puts(buf, tmp);
}

void scrbuffer_flush(struct scrbuffer *buf) {
    #ifdef __unix__
        write(STDOUT_FILENO, buf->chr_buf, buf->used);
    #elif _WIN32
        DWORD written;
        WriteConsoleA(h_out, buf->chr_buf, buf->used, &written, NULL);
    #endif

    buf->used = 0;
}

// FIXME Am I seriously reallocating every time a character is written?
static void check_buffer(struct scrbuffer *buf, int requested_space) {
    // I could write some complex code that uses an 'if' and math, but
    // since the buffer should expand rarely, this 'while' is acceptable
    while(buf->used + requested_space > buf->size) {
        buf->size += buf->inc_step;
    }
    buf->chr_buf = realloc(buf->chr_buf, buf->size * sizeof(char));
}
