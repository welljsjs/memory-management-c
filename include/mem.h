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

#ifndef MEM_INCLUDED
#define MEM_INCLUDED
#include "except.h"

extern const Except_T Mem_Failed;

extern void *Mem_alloc(long nbytes,
                       char const *file, int line);
extern void *Mem_calloc(long count, long nbytes,
                        char const *file, int line);
extern void Mem_free(void *ptr,
                     char const *file, int line);
extern void *Mem_resize(void *ptr, long nbytes,
                        char const *file, int line);

#define ALLOC(nbytes) \
  Mem_alloc((nbytes), __FILE__, __LINE__)
#define CALLOC(count, nbytes) \
  Mem_calloc((count), (nbytes), __FILE__, __LINE__)

#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))

#define FREE(ptr) ((void)(Mem_free((ptr),               \
                                   __FILE__, __LINE__), \
                          (ptr) = 0))

#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), (nbytes), \
                                                __FILE__, __LINE__))

#endif /* MEM_INCLUDED */
