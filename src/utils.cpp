/*
 * This file is part of SPICE.
 * Copyright (C) 2019 Albert Farres

 * SPICE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SPICE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SPICE.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "utils.hpp"

#ifndef HAVE_CXXSTDLIB
// http://stackoverflow.com/questions/920500/what-is-the-purpose-of-cxa-pure-virtual
extern "C" void __cxa_pure_virtual() { while (1); }

// http://stackoverflow.com/questions/7015285/undefined-reference-to-operator-deletevoid
#include <stdlib.h>
void * operator new(size_t n)
{
	void * const p = malloc(n);
	// handle p == 0
	return p;
}

void operator delete(void * p)
{
	free(p);
}

void operator delete(void * p, size_t)
{
	free(p);
}

// http://www.avrfreaks.net/comment/341297#comment-341297
extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};
#endif //HAVE_CXXSTDLIB
