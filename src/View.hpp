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

#include "ViewIface.hpp"

struct View: public ViewIface<View>{
	static void init( void );
	static void update( void );
	static void display_normal_bit( uint8_t n, bool selected );
	static void display_mute_bit( uint8_t n, bool selected );
	static void display_flip_bit( uint8_t n, bool selected );
	static void display_swap_state( uint8_t n, bool selected );
};


