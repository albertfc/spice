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

#include "ModelIface.hpp"
#include "SwapContainer.hpp"

#define EXT_ADC_NBITS    12
#define SWAP_STATES_SIZE 50

enum CURR_BIT {
	CURR_BIT_NORMAL,
	CURR_BIT_MUTE,
	CURR_BIT_FLIP,
	CURR_BIT_SIZE
};

#ifdef AVR
#include "avr_bitset.hpp"
#else
#include <bitset>
#endif

#define STATES_SIZE MODES_SIZE

template <typename StateImpl>
struct StateIface {
	static void _save_value_normal( const uint8_t n ) {
		StateImpl::_save_value_normal( n ); }
	static void _save_value_mute  ( const uint8_t n ) {
		StateImpl::_save_value_mute( n ); }
	static void _save_value_flip  ( const uint8_t n ) {
		StateImpl::_save_value_flip( n ); }
	static void _save_value_swap  ( const uint8_t n ) {
		StateImpl::_save_value_swap( n ); }
	static void _toggle_value_normal( void ) {
		StateImpl::_toggle_value_normal(); }
	static void _toggle_value_mute  ( void ) {
		StateImpl::_toggle_value_nute(); }
	static void _toggle_value_flip  ( void ) {
		StateImpl::_toggle_value_flip(); }
	static void _toggle_value_swap  ( void ) {
		StateImpl::_toggle_value_swap(); }
	static void _display_normal_bit( void ) {
		StateImpl::_display_normal_bit(); }
	static void _display_mute_bit  ( void ) {
		StateImpl::_display_mute_bit(); }
	static void _display_flip_bit  ( void ) {
		StateImpl::_display_flip_bit(); }
	static void _display_swap_bit  ( void ) {
		StateImpl::_display_swap_bit(); }
	static void _reset_mode_normal( void ) {
		StateImpl::_reset_mode_normal(); }
	static void _reset_mode_mute  ( void ) {
		StateImpl::_reset_mode_mute(); }
	static void _reset_mode_flip  ( void ) {
		StateImpl::_reset_mode_flip(); }
	static void _reset_mode_swap  ( void ) {
		StateImpl::_reset_mode_swap(); }
};

class State;
struct StateNormal;
struct StateMute;
struct StateFlip;
struct StateSwap;

class Model: public ModelIface<Model>, StateIface<Model> {

	friend State;
	friend StateNormal; friend StateMute; friend StateFlip; friend StateSwap;

	public:
		static void init( void );
		static void next_mode( void );
		static Mode get_mode( void ) { return _s_idx; }
		static void set_mode_normal( void ) { _s_idx = 0; }
		static void set_bits_normal( const uint8_t n );
		static void set_value( const uint8_t n );
		static void reset( void );
		static void toggle_value( void );
		static uint16_t process_sample( uint16_t sample );

		static uint16_t get_mute_mask( void );
		static uint16_t get_flip_mask( void );
		static SwapContainer & get_active_swaps( void ) { return _swaps;}

	private:
		static const State * const _s[STATES_SIZE];
		static uint8_t _s_idx;

		static uint8_t _curr_bit[CURR_BIT_SIZE];
		static avr_bitset<EXT_ADC_NBITS, 0> _bits_mute;
		static avr_bitset<EXT_ADC_NBITS, 1> _bits_flip;
		static uint8_t _curr_swap; // TODO: init to 0
		static SwapContainer _swaps;

		static const State * _get_mode( void ) { return _s[_s_idx]; }

		static void _save_value_normal( const uint8_t n ) {(void)n;}
		static void _save_value_mute( const uint8_t n );
		static void _save_value_flip( const uint8_t n );
		static void _save_value_swap( const uint8_t n );

		static void _toggle_value_normal( void ) {}
		static void _toggle_value_mute( void );
		static void _toggle_value_flip( void );
		static void _toggle_value_swap( void );

		static void _display_normal_bit( void );
		static void _display_flip_bit( void );
		static void _display_mute_bit( void );
		static void _display_swap_bit( void );
		
		static void _update( void );

		static void _reset_mode_normal( void ) {}
		static void _reset_mode_mute( void );
		static void _reset_mode_flip( void );
		static void _reset_mode_swap( void );
		static void _reset_all( void );

		static uint16_t _bitswap( uint16_t value, const uint8_t a, const uint8_t b );
};

class State {
public:
	virtual void save_value( const uint8_t n ) const = 0;
	virtual void toggle_value( void ) const = 0;
	virtual void display_value( void ) const = 0;
	virtual void reset_value( void ) const = 0;
};
struct StateNormal: public State {
	virtual void save_value( const uint8_t n ) const { Model::_save_value_normal( n ); }
	virtual void toggle_value( void )          const { Model::_toggle_value_normal(); }
	virtual void display_value( void )         const { Model::_display_normal_bit(); }
	virtual void reset_value( void )           const { Model::_reset_mode_normal(); }
};
struct StateMute  : public State {
	virtual void save_value( const uint8_t n ) const { Model::_save_value_mute( n ); }
	virtual void toggle_value( void )          const { Model::_toggle_value_mute(); }
	virtual void display_value( void )         const { Model::_display_mute_bit(); }
	virtual void reset_value( void )           const { Model::_reset_mode_mute(); }
};
struct StateFlip  : public State {
	virtual void save_value( const uint8_t n ) const { Model::_save_value_flip( n ); }
	virtual void toggle_value( void )          const { Model::_toggle_value_flip(); }
	virtual void display_value( void )         const { Model::_display_flip_bit(); }
	virtual void reset_value( void )           const { Model::_reset_mode_flip(); }
};
struct StateSwap  : public State {
	virtual void save_value( const uint8_t n ) const { Model::_save_value_swap( n ); }
	virtual void toggle_value( void )          const { Model::_toggle_value_swap(); }
	virtual void display_value( void )         const { Model::_display_swap_bit(); }
	virtual void reset_value( void )           const { Model::_reset_mode_swap(); }
};


