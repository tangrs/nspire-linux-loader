/*
    TI-NSPIRE Linux In-Place Bootloader
    Copyright (C) 2012  Daniel Tang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <os.h>

static void __attribute__((naked)) printc(const char c) {
    asm volatile ("mov r1, #0x90000000\n"
    "add r1, r1, #0x20000\n"
    "strb r0, [r1, #0]\n"
    "1: ldr  r0, [r1, #0x18]\n"
    "tst r0, #(1<<5)\n"
    "bne 1b\n"
    "bx lr");
}

void printk(const char * format, ...) {
    static char buffer[128];
    char *ptr = buffer;
    va_list ap;
    va_start(ap, format);
    vsprintf(ptr, format, ap);
    va_end(ap);
    while (*ptr) {
        printc(*ptr);
        ptr++;
    }
}