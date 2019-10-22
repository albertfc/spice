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

#include "SwapContainer.hpp"

#define MODES_SIZE 4
typedef uint8_t Mode;

template <typename ModelImpl>
struct ModelIface {
	static void init( void ) {
		ModelImpl::init(); }
	static void next_mode( void  ) {
		ModelImpl::next_mode(); }
	static Mode get_mode( void  ) {
		return ModelImpl::get_mode(); }
	static void set_mode_normal( void  ) {
		ModelImpl::set_mode_normal(); }
	static void set_bits_normal( const uint8_t n ) {
		ModelImpl::set_bits_normal( n ); }
	static void set_value( const uint8_t n ) {
		ModelImpl::set_value( n ); }
	static void reset( void ) {
		ModelImpl::reset(); }
	static void toggle_value( void ) {
		ModelImpl::toggle_value(); }
	static uint16_t process_sample( uint16_t sample ) {
		return ModelImpl::process_sample( sample ); }

	//private:
	//ModelIface() {} // Pure static class. Disallow creating an instance of this object
};
