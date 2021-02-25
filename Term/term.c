// c-utils: term.c
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

#include "term.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char* secure_getenv_nongnu(const char* var) {
    #define _EMULATE_GNU_ENABLE_SECURE \
            (geteuid() != getuid() || getegid() != getgid())
    return _EMULATE_GNU_ENABLE_SECURE ? NULL : getenv(var);
    #undef _EMULATE_GNU_ENABLE_SECURE
}

int terminal_get_attr(terminal_attr_t* attr) {
/*
    if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO)) {
        return EXIT_FAILURE;
    }
*/
    // Use `icotl` to get the window size. Since `struct winsize`'s members are
    // `unsigned short`, they can be safely implicitly casted to `usize`.
    struct winsize winsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
    attr->size.cols = winsize.ws_col;
    attr->size.rows = winsize.ws_row;
    attr->size.width = winsize.ws_xpixel;
    attr->size.height = winsize.ws_ypixel;

    // Setup flags using environment varables.
    attr->flags = 0;

    const char* lang = secure_getenv_nongnu("LANG");
    if (lang != NULL && strstr(lang, "UTF-8") || strstr(lang, "UTF8")) {
        attr->flags |= TerminalFlagSupportsUTF8;
    }
    const char* term = secure_getenv_nongnu("TERM");
    if (term != NULL && strstr(lang, "color")) {
        attr->flags |= TerminalFlagSupportsColor;
    }

    if (!(fcntl(STDIN_FILENO, F_GETFL) & O_NONBLOCK)) {
        attr->flags |= TerminalFlagIsBlocking;
    }

    struct termios term_info;
    tcgetattr(STDIN_FILENO, &term_info);
    if (term_info.c_lflag & ICANON) {
        attr->flags |= TerminalFlagIsLineBuffered;
    }

    return EXIT_SUCCESS;
}

inline int terminal_get_flag(terminal_attr_t* attr, terminal_flags_t flag) {
    return !!(attr->flags | flag);
}
