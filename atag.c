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
#include <stdint.h>

#include "atag_tags.h"
#include "atag.h"

int atagBegin(void **head, void **last) {
    struct atag * begin = malloc(2);
    if (!begin) return -1;

    begin->hdr.size = 2;
    begin->hdr.tag  = ATAG_CORE;

    *head = begin;
    *last = (char *)begin + (begin->hdr.size * sizeof(uint32_t));
    return 0;
}

int atagAdd(void **head, void **last, int tagid, ...) {
    va_list ap;
    va_start(ap, tagid);
    char *cmdline = NULL;
    struct atag tag;

    tag.hdr.tag = tagid;

    #define SET_NEXT_VARARG(type, element) tag.u.type.element = va_arg(ap, uint32_t)
    #define SET_SIZE(x) tag.hdr.size = 2 + (sizeof(tag.u.x) / sizeof(uint32_t))
    switch (tagid) {
        case ATAG_MEM:
            SET_SIZE(mem);
            SET_NEXT_VARARG(mem, size);
            SET_NEXT_VARARG(mem, start);
            break;
        case ATAG_VIDEOTEXT:
            SET_SIZE(videotext);
            SET_NEXT_VARARG(videotext, x);
            SET_NEXT_VARARG(videotext, y);
            SET_NEXT_VARARG(videotext, video_page);
            SET_NEXT_VARARG(videotext, video_mode);
            SET_NEXT_VARARG(videotext, video_cols);
            SET_NEXT_VARARG(videotext, video_ega_bx);
            SET_NEXT_VARARG(videotext, video_isvga);
            SET_NEXT_VARARG(videotext, video_points);
            break;
        case ATAG_RAMDISK:
            SET_SIZE(ramdisk);
            SET_NEXT_VARARG(ramdisk, flags);
            SET_NEXT_VARARG(ramdisk, size);
            SET_NEXT_VARARG(ramdisk, start);
            break;
        case ATAG_INITRD2:
            SET_SIZE(initrd2);
            SET_NEXT_VARARG(initrd2, start);
            SET_NEXT_VARARG(initrd2, size);
            break;
        case ATAG_SERIAL:
            SET_SIZE(serialnr);
            SET_NEXT_VARARG(serialnr, low);
            SET_NEXT_VARARG(serialnr, high);
            break;
        case ATAG_REVISION:
            SET_SIZE(revision);
            SET_NEXT_VARARG(revision, rev);
            break;
        case ATAG_VIDEOLFB:
            SET_SIZE(videolfb);
            SET_NEXT_VARARG(videolfb, lfb_width);
            SET_NEXT_VARARG(videolfb, lfb_height);
            SET_NEXT_VARARG(videolfb, lfb_depth);
            SET_NEXT_VARARG(videolfb, lfb_linelength);
            SET_NEXT_VARARG(videolfb, lfb_base);
            SET_NEXT_VARARG(videolfb, lfb_size);
            SET_NEXT_VARARG(videolfb, red_size);
            SET_NEXT_VARARG(videolfb, red_pos);
            SET_NEXT_VARARG(videolfb, green_size);
            SET_NEXT_VARARG(videolfb, green_pos);
            SET_NEXT_VARARG(videolfb, blue_size);
            SET_NEXT_VARARG(videolfb, blue_pos);
            SET_NEXT_VARARG(videolfb, rsvd_size);
            SET_NEXT_VARARG(videolfb, rsvd_pos);
            break;
        case ATAG_NONE:
            tag.hdr.size = 2;
            break;
        case ATAG_CMDLINE:
            cmdline = va_arg(ap, char*);
            tag.hdr.size = 2 + (strlen(cmdline) + sizeof(uint32_t)) / sizeof(uint32_t);
            break;
        default:
            return -1;
    }

    size_t oldSize = ((char*)*last - (char*)*head);
    size_t tagSize = (tag.hdr.size * sizeof(uint32_t));
    void *new = realloc(*head, oldSize + tagSize);
    if (!new) return -1;
    *head = new;
    *last = (char*)new + oldSize;

    memcpy(*last, &tag, tagSize);
    if (cmdline) {
        struct atag *t = *last;
        strcpy(t->u.cmdline.cmdline, cmdline);
    }

    *last = (char*)*last + tagSize;

    return 0;
}

int atagEnd(void **head, void **last) {
    return atagAdd(head, last, ATAG_NONE);
}