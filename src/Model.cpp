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
#ifdef AVR
#include "avr_log.hpp"
#else
#include "log.hpp"
#endif

#include "Model.hpp"

// init static members
uint8_t Model::_s_idx = 0;
uint8_t Model::_curr_bit[] = {0};
uint8_t Model::_curr_swap = 0;
const static StateNormal _sn;
const static StateMute _sm;
const static StateFlip _sf;
const static StateSwap _ss;
const State * const Model::_s[STATES_SIZE] = {&_sn, &_sm, &_sf, &_ss};
SwapContainer Model::_swaps;
avr_bitset<EXT_ADC_NBITS, 0> Model::_bits_mute;
avr_bitset<EXT_ADC_NBITS, 1> Model::_bits_flip;


#include "View.hpp"
#include "Controller.hpp"

void Model::init( void )
{
	View::init();
	Controller::init();
}

void Model::next_mode( void )
{
	_s_idx = (_s_idx + 1) % STATES_SIZE;
	_update();
}

void Model::set_bits_normal( const uint8_t n )
{
	_curr_bit[CURR_BIT_NORMAL] = n;

	_update();
}

void Model::set_value( const uint8_t n )
{
	//if( dynamic_cast<const StateNormal*>(_get_mode()) )
	if( _s_idx == 0 )
		return;

	_get_mode()->save_value( n );
	_update();
}

void Model::_save_value_mute( const uint8_t n )
{
	_curr_bit[CURR_BIT_MUTE] = n;
}

void Model::_save_value_flip( const uint8_t n )
{
	_curr_bit[CURR_BIT_FLIP] = n;
}

void Model::_save_value_swap( const uint8_t n )
{
	_curr_swap = n;
}

void Model::_reset_mode_mute( void ) {
	_bits_mute.set( );
}

void Model::_reset_mode_flip( void ) {
	_bits_flip.reset( );
}

void Model::_reset_mode_swap( void ) {
	_swaps.reset( );
}

void Model::_reset_all( void )
{
	for( const State * s: _s)
		s->reset_value();
	_s_idx = 0;
	_update();
}

void Model::reset( void )
{
	//if( dynamic_cast<const StateNormal*>(_get_mode()) )
	if( _s_idx == 0 )
		_reset_all();
	else
		_get_mode()->reset_value();
	_update();
}


void Model::toggle_value( void )
{
	_get_mode()->toggle_value();
	_update();
}

void Model::_toggle_value_mute( void ) {
	_bits_mute.flip( _curr_bit[CURR_BIT_MUTE] );

	//DBG( "mute mask: " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY( get_mute_mask() ));
}

void Model::_toggle_value_flip( void ) {
	_bits_flip.flip( _curr_bit[CURR_BIT_FLIP] );
}

void Model::_toggle_value_swap( void ) {
	_swaps.flip( _curr_swap );
}

void Model::_display_normal_bit( void )
{
	View::display_normal_bit( _curr_bit[CURR_BIT_NORMAL], !_bits_mute.all() | _bits_flip.any() | _swaps.any() );
}

void Model::_display_flip_bit( void )
{
	View::display_flip_bit( _curr_bit[CURR_BIT_FLIP], _bits_flip[_curr_bit[CURR_BIT_FLIP]]);
}

void Model::_display_mute_bit( void )
{
	View::display_mute_bit( _curr_bit[CURR_BIT_MUTE], !_bits_mute[_curr_bit[CURR_BIT_MUTE]]);
}

void Model::_display_swap_bit( void )
{
	View::display_swap_state( _curr_swap, _swaps.test( _curr_swap ) );
}

void Model::_update( void )
{
	_get_mode()->display_value();

	DBG( "mute mask %d\n", !_bits_mute.all() ); 
	DBG( "flip mask %d\n",  _bits_flip.any() ); 
	DBG( "swap mask %d\n",  _swaps.any() ); 
}

uint16_t Model::get_mute_mask( void ) {
#ifdef AVR
	return _bits_mute.to_value();
#else
	return _bits_mute.to_ulong();
#endif
}

uint16_t Model::get_flip_mask( void ) {
#ifdef AVR
	return _bits_flip.to_value();
#else
	return _bits_flip.to_ulong();
#endif
}

uint16_t Model::_bitswap( uint16_t value, const uint8_t a, const uint8_t b )
{
	const uint16_t a_bit = value & (1<<a);
	const uint16_t b_bit = value & (1<<b);

	value = (a_bit) ? value | (1<<b) : value & (~(1<<b));
	value = (b_bit) ? value | (1<<a) : value & (~(1<<a));

	return value;
}

uint16_t Model::process_sample( uint16_t sample )
{
	sample &= get_mute_mask();
	sample ^= get_flip_mask();
	for( swap_type s : get_active_swaps() )
		while( !(s->first<0) ) {
			sample = _bitswap( sample, s->first, s->second );
			s++;
		}

	return sample;
}
