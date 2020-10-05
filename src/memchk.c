#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "config.h"
#ifdef HAVE_STDALIGN_H
#include <stdalign.h>
#endif
#include "assert.h"
#include "except.h"
#include "mem.h"

#ifndef HAVE_MAX_ALIGN_T
typedef union align
{
  int i;
  long l;
  long *lp;
  void *p;
  void (*fp)(void);
  float f;
  double d;
  long double ld;
} max_align_t;
#endif
#ifndef HAVE_UINTPTR_T
typedef unsigned long uintptr_t;
#endif
#ifndef HAVE_ALIGNOF
#define alignof(x) sizeof(x) // This obviously only works for primitive data types, not structs etc.
#endif

#define align_log2(x) (((x)&1) ? 0 : ((x)&2) ? 1 : ((x)&4) ? 2 : ((x)&8) ? 3 : ((x)&16) ? 4 : ((x)&32) ? 5 : -1)

/* 
This variant of the hash function is safe for the align of values on the stack 
in that it gets the required alignment size of the element pointed to by `p`.

This will, however, generally not be necessary for any call to malloc();
values on the stack may require less alignment than the result of a malloc()
call: malloc() must align for vector processing or an array of double, even
when you only store a char array within the allocation. 
But the compiler knows how big the individual variables are, what their
respective alignment requirements are, and can thus pack them closer.
*/
#define hash_safe(p, t) (((uintptr_t)(p) >> align_log2(alignof(*(p)))) & \
                         (sizeof(t) / sizeof((t)[0]) - 1))

#define hash(p, t) (((uintptr_t)(p) >> alignof(max_align_t)) & \
                    (sizeof(t) / sizeof((t)[0]) - 1))

#define NDESCRIPTORS 512

#define NALLOC ((4096 + sizeof(max_align_t) - 1) / \
                (sizeof(max_align_t))) *           \
                   (sizeof(max_align_t))

static struct descriptor
{
  struct descriptor *free;
  struct descriptor *link;
  void const *ptr;
  long size;
  char const *file;
  int line;
} * htab[2048];
static struct descriptor freelist = {&freelist};

static struct descriptor *find(void const *ptr)
{
  struct descriptor *bp = htab[hash(ptr, htab)];

  while (bp && bp->ptr != ptr)
    bp = bp->link;
  return bp;
}

void Mem_free(void *ptr, char const *file, int line)
{
  if (ptr)
  {
    struct descriptor *bp;
    if (((uintptr_t)ptr) % (sizeof(max_align_t)) != 0 || (bp = find(ptr)) == NULL || bp->free)
      Except_raise(&Assert_Failed, file, line);
    bp->free = freelist.free;
    freelist.free = bp;
  }
}

void *Mem_resize(void *ptr, long nbytes,
                 char const *file, int line)
{
  struct descriptor *bp;
  void *newptr;

  assert(ptr);
  assert(nbytes > 0);
  if (((uintptr_t)ptr) % (sizeof(max_align_t)) != 0 || (bp = find(ptr)) == NULL || bp->free)
    Except_raise(&Assert_Failed, file, line);
  newptr = Mem_alloc(nbytes, file, line);
  memcpy(newptr, ptr,
         nbytes < bp->size ? nbytes : bp->size);
  Mem_free(ptr, file, line);
  return newptr;
}

void *Mem_calloc(long count, long nbytes,
                 char const *file, int line)
{
  void *ptr;

  assert(count > 0);
  assert(nbytes > 0);
  ptr = Mem_alloc(count * nbytes, file, line);
  memset(ptr, '\0', count * nbytes);
  return ptr;
}

static struct descriptor *dalloc(void *ptr, long size,
                                 char const *file, int line)
{
  static struct descriptor *avail;
  static int nleft;

  if (nleft <= 0)
  {
    avail = malloc(NDESCRIPTORS * sizeof(*avail));
    if (avail == NULL)
      return NULL;
    nleft = NDESCRIPTORS;
  }
  avail->ptr = ptr;
  avail->size = size;
  avail->file = file;
  avail->line = line;
  avail->free = avail->link = NULL;
  nleft--;
  return avail++;
}

void *Mem_alloc(long nbytes, char const *file, int line)
{
  struct descriptor *bp;
  void *ptr;

  assert(nbytes > 0);
  nbytes = ((nbytes + sizeof(max_align_t) - 1) / (sizeof(max_align_t))) * (sizeof(max_align_t)); // might be reduced to: ((nbytes + sizeof(max_align_t) - 1) & ~alignof(max_align_t)
  for (bp = freelist.free; bp; bp = bp->free)
  {
    if (bp->size > nbytes)
    {
      bp->size -= nbytes;
      ptr = (char *)bp->ptr + bp->size;
      if ((bp = dalloc(ptr, nbytes, file, line)) != NULL)
      {
        unsigned h = hash(ptr, htab);
        bp->link = htab[h];
        htab[h] = bp;
        return ptr;
      }
      else
      {
        if (file == NULL)
          RAISE(Mem_Failed);
        else
          Except_raise(&Mem_Failed, file, line);
      }
    }
    if (bp == &freelist)
    {
      struct descriptor *newptr;
      if ((ptr = malloc(nbytes + NALLOC)) == NULL || (newptr = dalloc(ptr, nbytes + NALLOC,
                                                                      __FILE__, __LINE__)) == NULL)
      {
        if (file == NULL)
          RAISE(Mem_Failed);
        else
          Except_raise(&Mem_Failed, file, line);
      }
      newptr->free = freelist.free;
      freelist.free = newptr;
    }
  }
  assert(0);
  return NULL;
}
