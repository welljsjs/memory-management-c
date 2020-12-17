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

#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED
#include "except.h"

#define T Thread_T
typedef struct T *T;

extern const Except_T Thread_Failed;
extern const Except_T Thread_Alerted;

extern int Thread_init(int preempt, ...);
extern T Thread_new(int apply(void *),
                    void *args, int nbytes, ...);
extern void Thread_exit(int code);
extern void Thread_alert(T t);
extern T Thread_self(void);
extern int Thread_join(T t);
extern void Thread_pause(void);

#undef T
#endif /* THREAD_INCLUDED */
