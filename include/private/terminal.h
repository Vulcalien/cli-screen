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
 */
#ifndef VULC_CLISCREEN_TERMINAL_P
#define VULC_CLISCREEN_TERMINAL_P

#include "private/privscreen.h"

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

#endif // VULC_CLISCREEN_TERMINAL_P
