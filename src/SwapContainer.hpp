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

//Based on --> https://lorenzotoso.wordpress.com/2016/01/13/defining-a-custom-iterator-in-c/

#ifdef AVR
#include "avr_bitset.hpp"
#include <avr/eeprom.h>
#else
#include <vector>
#include <bitset>
#endif

#include "constants.h"

typedef std::pair<const int8_t, const int8_t> swap_unit;
typedef const swap_unit* swap_type;

class avr_swap_idx_t {
public:
	operator size_t() const { return _idx; }
	void operator=(const size_t other) {
		_idx = other;
		_eeprom_update();
	}
	avr_swap_idx_t( ) {
		_eeprom_load();
	} 

private:
	size_t _idx;
	static size_t EEMEM _eeprom_idx;

	void _eeprom_save( void ) {
		eeprom_busy_wait();
		eeprom_write_block( &_idx, &_eeprom_idx, sizeof( size_t ) );
	}
	void _eeprom_load( void ) {
		eeprom_busy_wait();
		eeprom_read_block(  &_idx, &_eeprom_idx, sizeof( size_t ) );
	}
	void _eeprom_update( void ) {
		eeprom_busy_wait();
		eeprom_update_block( &_idx, &_eeprom_idx, sizeof( size_t ) );
	}
};

class SwapContainer
{
	class iterator; // Forward declaration

public:
	SwapContainer() = default;
	~SwapContainer() = default;

	size_t size() const;

	iterator begin();
	iterator end();

	void set( const size_t idx );
	void reset( const size_t idx );
	void reset( void );
	bool test( size_t pos ) const;
	void flip( const size_t idx );
	bool any( void ) const;

	static const swap_type _swaps[]; // Cannot be private because have to init outside class definition :(

private:
	avr_bitset<SWAPS_SIZE,2> _swaps_active;
	avr_swap_idx_t           _swaps_active_begin_idx;

	class iterator : public std::iterator<std::output_iterator_tag, int>
	{
	public:
		explicit iterator(SwapContainer & Container, size_t index = 0);
		const swap_type & operator*() const;
		iterator & operator++();
		//iterator & operator++(int);
		bool operator!=(const iterator &) const;
	private:
		SwapContainer & Container;
		size_t _idx = 0;
	};

};


