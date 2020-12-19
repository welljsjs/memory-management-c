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

#ifndef HEADER_ATOM
#define HEADER_ATOM

extern int Atom_length(char const *str);
extern char const *Atom_new(char const *str, int len);
extern char const *Atom_string(char const *str);
extern char const *Atom_int(long n);

#endif /* HEADER_ATOM */
