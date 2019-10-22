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
#include "constants.h"
#include "spi.h"

#include "utils.hpp"
#include "Timer.hpp"
extern volatile Timer timer;

#include "View.hpp"

static constexpr uint8_t bits_to_7s[] = { SEVEN_SEGMENTS_HEX };
STATIC_ARRAY_SIZE_CHECK( bits_to_7s, 1 + EXT_ADC_BITRES + SWAPS_SIZE );

static class SevenSegments: public TimerIface {
public:
	void set_value( const uint8_t v ) { _value = v; }
	void set_dot( const bool d )      { _dot   = d; }
	void set_freq( const uint8_t f )  { _freq  = f; }
	void increments_one_cs( void )
	{
		_cs = (_cs+1) % 100;

		if( !_freq )
			return;

		if( _cs % (100/_freq) )
			return;

		// blink on multiples
		_blink = !_blink;
	}
	virtual void increments_one_ms( void )
	{
		if( ++_ms < 10 )
			return;

		_ms = 0;
		increments_one_cs();
	}
	void update( void )
	{
		uint8_t value = 0xff;
		if( !_freq || !_blink )
			value = ~(_dot<<7) & bits_to_7s[_value];
		spi_shiftreg_update( value );
	}

private:
	volatile uint8_t _value;
	volatile uint8_t _freq;
	volatile bool    _dot;
	volatile uint8_t _cs;  // centiseconds (10^-2s.)
	volatile uint8_t _ms;  // miliiseconds (10^-3s.)
	volatile bool    _blink;
} ss;

static void _ss_set( uint8_t n, bool selected, uint8_t freq ) {
	ss.set_value( n );
	ss.set_dot( selected );
	ss.set_freq( freq );
}

void View::init( void ) {
	timer.register_ms( &ss );
}
void View::update( void ) {
	ss.update();
}
void View::display_normal_bit( uint8_t n, bool selected ) {
	_ss_set( n, selected, FREQ_7S_NORMAL );
}
void View::display_mute_bit( uint8_t n, bool selected ) {
	_ss_set( n, selected , FREQ_7S_MUTE );
}
void View::display_flip_bit( uint8_t n, bool selected ) {
	_ss_set( n, selected , FREQ_7S_FLIP );
}
void View::display_swap_state( uint8_t n, bool selected ) {
	_ss_set( 1+EXT_ADC_BITRES+n, selected , FREQ_7S_SWAP );
}
