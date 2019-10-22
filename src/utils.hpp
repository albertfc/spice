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
#pragma once

#define STATIC_ARRAY_SIZE_CHECK( _array, _size ) \
	static_assert( _size == (sizeof _array / sizeof _array[0]), "wrong size!" );

#ifndef HAVE_CXXSTDLIB

#define assert

__extension__ typedef int __guard __attribute__((mode (__DI__)));

#endif //HAVE_CXXSTDLIB
