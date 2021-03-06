



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

#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED
#include "except.h"

#define T Arena_T
typedef struct T *T;

extern const Except_T Arena_NewFailed;
extern const Except_T Arena_Failed;

extern T Arena_new(void);
extern void Arena_dispose(T *ap);
extern void *Arena_alloc(T arena, long nbytes,
                         char const *file, int line);
extern void *Arena_calloc(T arena, long count, long nbytes,
                          char const *file, int line);
extern void Arena_free(T arena);

#undef T
#endif /* ARENA_INCLUDED */
