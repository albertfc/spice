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

#ifndef AVR
#include <cstdint> // uint8_t
#endif

template <typename ViewImpl>
struct ViewIface {
	static void init( void ) { // Safe to use model *
		ViewImpl::init(); }
	static void update( void ) {
		ViewImpl::update(); }
	static void display_normal_bit( uint8_t n, bool selected ) {
		ViewImpl::display_normal_bit( n, selected ); }
	static void display_mute_bit( uint8_t n, bool selected ) {
		ViewImpl::display_mute_bit( n, selected ); }
	static void display_flip_bit( uint8_t n, bool selected ) {
		ViewImpl::display_flip_bit( n ,selected ); }
	static void display_swap_state( uint8_t n, bool selected ) {
		ViewImpl::display_swap_state( n ,selected ); }
};
