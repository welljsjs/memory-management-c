/**
 * Copyright (C) 2021 Julius Schmidt
 * 
 * This file is part of libmem.
 * 
 * "libmem" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * "libmem" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with "libmem".  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "assert.h"
#include "thread.h"
#include "sem.h"

void _MONITOR(void) {}
extern void _ENDMONITOR(void);

#define T Thread_T
#define isempty(q) ((q) == NULL)
struct T
{
  unsigned long *sp; /* must be first */
  T link;
  T *inqueue;
};

static T ready = NULL;

static void put(T t, T *q)
{
  assert(t);
  assert(t->inqueue == NULL && t->link == NULL);
  if (*q)
  {
    t->link = (*q)->link;
    (*q)->link = t;
  }
  else
    t->link = t;
  *q = t;
  t->inqueue = q;
}
static T get(T *q)
{
  T t;

  assert(!isempty(*q));
  t = (*q)->link;
  if (t == *q)
    *q = NULL;
  else
    (*q)->link = t->link;
  assert(t->inqueue == q);
  t->link = NULL;
  t->inqueue = NULL;
  return t;
}

#undef T

#define T Sem_T

#undef T
