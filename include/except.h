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

#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include <setjmp.h>

#define T Except_T
typedef struct T
{
	char const *reason;
} T;

typedef struct Except_Frame Except_Frame;
struct Except_Frame
{
	Except_Frame *prev;
	jmp_buf env;
	char const *file;
	int line;
	T const *exception;
};
enum
{
	Except_entered = 0,
	Except_raised,
	Except_handled,
	Except_finalized
};

extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;

extern void Except_raise(T const *e, char const *file, int line);

#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, \
														 Except_frame.file, Except_frame.line)

#define RETURN                                  \
	switch (Except_stack = Except_stack->prev, 0) \
	default:                                      \
		return;

#define TRY                                 \
	do                                        \
	{                                         \
		volatile int Except_flag;               \
		Except_Frame Except_frame;              \
		Except_frame.prev = Except_stack;       \
		Except_stack = &Except_frame;           \
		Except_flag = setjmp(Except_frame.env); \
		if (Except_flag == Except_entered)      \
		{

#define EXCEPT(e)                          \
	if (Except_flag == Except_entered)       \
		Except_stack = Except_stack->prev;     \
	}                                        \
	else if (Except_frame.exception == &(e)) \
	{                                        \
		Except_flag = Except_handled;

#define ELSE                           \
	if (Except_flag == Except_entered)   \
		Except_stack = Except_stack->prev; \
	}                                    \
	else                                 \
	{                                    \
		Except_flag = Except_handled;

#define FINALLY                        \
	if (Except_flag == Except_entered)   \
		Except_stack = Except_stack->prev; \
	}                                    \
	{                                    \
		if (Except_flag == Except_entered) \
			Except_flag = Except_finalized;

#define END_TRY                        \
	if (Except_flag == Except_entered)   \
		Except_stack = Except_stack->prev; \
	}                                    \
	if (Except_flag == Except_raised)    \
		RERAISE;                           \
	}                                    \
	while (0)

#undef T
#endif /* EXCEPT_INCLUDED */
