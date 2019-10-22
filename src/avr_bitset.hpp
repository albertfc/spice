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

#include "ustl/ubitset.h"
#define std ustl

#include <avr/eeprom.h>

template <size_t Size, uint8_t eeprom_idx>
class avr_bitset: std::bitset<Size> {
public:
	inline void set( void ) {
		std::bitset<Size>::set();
		_eeprom_update();
	}
	inline void set( size_t i ) {
		std::bitset<Size>::set(i);
		_eeprom_update();
	}
	inline void reset( void ) {
		std::bitset<Size>::reset();
		_eeprom_update();
	}
	inline void reset( size_t i ) {
		std::bitset<Size>::reset(i);
		_eeprom_update();
	}
	inline void flip( size_t i ) {
		std::bitset<Size>::flip(i);
		_eeprom_save();
	}
	inline bool all( void ) const { for( size_t i = 0; i < s_nWords-1; i++ ) if( _bits[i] != _bitset_ones ) return false; return (_bits[s_nWords-1] & s_last_ones) == s_last_ones; }

	using std::bitset<Size>::any;
	using std::bitset<Size>::test;
	using std::bitset<Size>::count;
	using std::bitset<Size>::to_value;
	using std::bitset<Size>::operator[];

	avr_bitset() {
		_eeprom_load();
	}

private:
	using typename std::bitset<Size>::value_type;
	using          std::bitset<Size>::s_nWords;
	using          std::bitset<Size>::s_WordBits;
	using          std::bitset<Size>::_bits;

	static constexpr value_type _bitset_zero = 0;
	static constexpr value_type _bitset_ones = ~_bitset_zero;
    static const size_t s_last_ones = ~(_bitset_ones<<(Size % s_WordBits));

	static value_type EEMEM _eeprom_bits[s_nWords];

	void _eeprom_save( void ) {
		eeprom_busy_wait();
		eeprom_write_block( _bits, _eeprom_bits, sizeof( value_type ) * s_nWords );
	}
	void _eeprom_load( void ) {
		eeprom_busy_wait();
		eeprom_read_block(  _bits, _eeprom_bits, sizeof( value_type ) * s_nWords );
	}
	void _eeprom_update( void ) {
		eeprom_busy_wait();
		eeprom_update_block(  _bits, _eeprom_bits, sizeof( value_type ) * s_nWords );
	}
};


