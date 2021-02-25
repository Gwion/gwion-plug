// c-utils: term.h
// Copyright (C) 2021 Ethan Uppal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "ints.h"

typedef enum {
    TerminalFlagSupportsUTF8   = 1 << 0,
    TerminalFlagSupportsColor  = 1 << 1,
    TerminalFlagIsBlocking     = 1 << 2,
    TerminalFlagIsLineBuffered = 1 << 3,
} terminal_flags_t;

typedef struct {
    usize cols;
    usize rows;
    usize width;
    usize height;
} terminal_size_t;

typedef struct {
    terminal_size_t size;
    terminal_flags_t flags;
} terminal_attr_t;

// Initializes the struct with the attributes of the terminal.
// On success: EXIT_SUCCESS
// On failure: EXIT_FAILURE
int terminal_get_attr(terminal_attr_t* attr);

// Returns a 1 or a 0 depending on whether the flag is set
int terminal_get_flag(terminal_attr_t* attr, terminal_flags_t flag);
