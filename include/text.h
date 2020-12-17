// Copyright (C) 2020 Julius Schmidt
//
// This file is part of libmem.
//
// "libmem" is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// "libmem" is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with "libmem".  If not, see <https://www.gnu.org/licenses/>.

#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED
#include <stdarg.h>

#define T Text_T

typedef struct T
{
  int len;
  char const *str;
} T;
typedef struct Text_save_T *Text_save_T;

extern const T Text_cset;
extern const T Text_ascii;
extern const T Text_ucase;
extern const T Text_lcase;
extern const T Text_digits;
extern const T Text_null;

extern T Text_put(char const *str);
extern char *Text_get(char *str, int size, T s);
extern T Text_box(char const *str, int len);
extern T Text_sub(T s, int i, int j);
extern int Text_pos(T s, int i);
extern T Text_cat(T s1, T s2);
extern T Text_dup(T s, int n);
extern T Text_reverse(T s);
extern T Text_map(T s, T const *from, T const *to);
extern int Text_cmp(T s1, T s2);
extern int Text_chr(T s, int i, int j, int c);
extern int Text_rchr(T s, int i, int j, int c);
extern int Text_upto(T s, int i, int j, T set);
extern int Text_rupto(T s, int i, int j, T set);
extern int Text_any(T s, int i, T set);
extern int Text_many(T s, int i, int j, T set);
extern int Text_rmany(T s, int i, int j, T set);
extern int Text_find(T s, int i, int j, T str);
extern int Text_rfind(T s, int i, int j, T str);
extern int Text_match(T s, int i, int j, T str);
extern int Text_rmatch(T s, int i, int j, T str);
extern void Text_fmt(int code, va_list *app,
                     int put(int c, void *cl), void *cl,
                     unsigned char flags[], int width, int precision);
extern Text_save_T Text_save(void);
extern void Text_restore(Text_save_T *save);

#undef T
#endif /* TEXT_INCLUDED */
