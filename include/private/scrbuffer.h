/* Copyright 2021 Vulcalien
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 only.
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
#ifndef VULC_CLISCREEN_SCRBUFFER_P
#define VULC_CLISCREEN_SCRBUFFER_P

#include "private/privscreen.h"

struct scrbuffer {
    u32 size;
    u32 used;
    char *chr_buf;

    u32 inc_step;
};

/* Create a screen buffer. */
extern struct scrbuffer *scrbuffer_create(u32 raster_size);

/* Destroy a screen buffer and set the pointer to NULL. */
extern void scrbuffer_destroy(struct scrbuffer **buf);

/* Put a character inside a screen buffer. */
extern void scrbuffer_putc(struct scrbuffer *buf, char chr);

/* Put a string inside a screen buffer. */
extern void scrbuffer_puts(struct scrbuffer *buf, const char *str);

/* Put a formatted string with values inside a screen buffer. */
extern void scrbuffer_printf(struct scrbuffer *buf,
                             const char *format, ...);

/* Write to the output stream. */
extern void scrbuffer_flush(struct scrbuffer *buf);

#endif // VULC_CLISCREEN_SCRBUFFER_P
