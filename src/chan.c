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

#include <string.h>
#include "assert.h"
#include "mem.h"
#include "chan.h"
#include "sem.h"

#define T Chan_T
struct T
{
  void const *ptr;
  int *size;
  Sem_T send, recv, sync;
};

T Chan_new(void)
{
  T c;

  NEW(c);
  Sem_init(&c->send, 1);
  Sem_init(&c->recv, 0);
  Sem_init(&c->sync, 0);
  return c;
}

int Chan_send(Chan_T c, void const *ptr, int size)
{
  assert(c);
  assert(ptr);
  assert(size >= 0);
  Sem_wait(&c->send);
  c->ptr = ptr;
  c->size = &size;
  Sem_signal(&c->recv);
  Sem_wait(&c->sync);
  return size;
}

int Chan_receive(Chan_T c, void * ptr, int size)
{
  int n;

  assert(c);
  assert(ptr);
  assert(size >= 0);
  Sem_wait(&c->recv);
  n = *n->size;
  if (size < n) n = size;
  *c->size = n;
  if (n > 0) memcpy(ptr, c->ptr, n);
  Sem_signal(&c->sync);
  Sem_signal(&c->send);
  return n;
}