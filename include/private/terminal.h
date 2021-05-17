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
#ifndef VULC_CLISCREEN_PRIV_TERMINAL
#define VULC_CLISCREEN_PRIV_TERMINAL

#include "screen.h"

struct terminal_size {
    u32 w;
    u32 h;
};

#ifdef _WIN32
    #include <windows.h>

    extern HANDLE h_in;
    extern HANDLE h_out;
#endif

extern struct terminal_size screen_terminal_size(void);

#endif // VULC_CLISCREEN_PRIV_TERMINAL
