/**
 * Copyright (C) 2020 Julius Schmidt
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
#include <limits.h>
#include "assert.h"
#include "text.h"
#include "mem.h"

#define T Text_T

#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i)-1)

static char cset[] =
    "\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
    "\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
    "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
    "\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
    "\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
    "\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
    "\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
    "\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
    "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
    "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
    "\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
    "\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
    "\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
    "\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
    "\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
    "\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377";

const T Text_cset = {256, cset};
const T Text_ascii = {128, cset};
const T Text_ucase = {26, cset + 'A'};
const T Text_lcase = {26, cset + 'a'};
const T Text_digits = {10, cset + '0'};
const T Text_null = {0, cset};
static struct chunk
{
  struct chunk *link;
  char *avail;
  char *limit;
} head = {NULL, NULL, NULL}, *current = &head;

static char *alloc(int len)
{
  assert(len >= 0);
  if (current->avail + len > current->limit)
  {
    current = current->link = ALLOC(sizeof(*current) + 10 * 1024 + len);
    current->avail = (char *)(current + 1);
    current->limit = current->avail + 10 * 1024 + len;
    current->link = NULL;
  }
  current->avail += len;
  return current->avail - len;
}

int Text_pos(T s, int i)
{
  assert(s.len >= 0 && s.str);
  i = idx(i, s.len);
  assert(i >= 0 && i <= s.len);
  return i + 1;
}

T Text_box(char const *str, int len)
{
  T text;

  assert(str);
  assert(len >= 0);
  text.str = str;
  text.len = len;
  return text;
}

T Text_sub(T s, int i, int j)
{
  T text;

  assert(s.len >= 0 && s.str);
  i = idx(i, s.len);
  j = idx(j, s.len);
  if (i > j)
  {
    int t = i;
    i = j;
    j = t;
  }
  assert(i >= 0 && j <= s.len);

  text.len = j - i;
  text.str = s.str + i;
  return text;
}

T Text_put(char const *str)
{
  T text;

  assert(str);
  text.len = strlen(str);
  text.str = memcpy(alloc(text.len), str, text.len);
  return text;
}

char *Text_get(char *str, int size, T s)
{
  assert(s.len >= 0 && s.str);
  if (str == NULL)
    str = ALLOC(s.len + 1);
  else
    assert(size >= s.len + 1);
  memcpy(str, s.str, s.len);
  str[s.len] = '\0';
  return str;
}
