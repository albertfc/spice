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
#include "SwapContainer.hpp"

// init static members
size_t avr_swap_idx_t::_eeprom_idx = SWAPS_SIZE;
DEFINE_SWAPS
static_assert( SWAPS_SIZE == (sizeof(SwapContainer::_swaps)/sizeof(SwapContainer::_swaps[0])), "swap size incorrect!" );

void SwapContainer::set( const size_t idx ) {
	_swaps_active.set( idx );
	_swaps_active_begin_idx = std::min( idx, _swaps_active_begin_idx );
}

void SwapContainer::reset( const size_t idx ) {
	_swaps_active.reset( idx );
	if( idx != _swaps_active_begin_idx )
		return;

	// Search for new min
	size_t tmp_idx = _swaps_active_begin_idx;
	while( tmp_idx < SWAPS_SIZE && !_swaps_active[++tmp_idx] );
	_swaps_active_begin_idx = tmp_idx;
}

void SwapContainer::reset( void ) {
	_swaps_active.reset();
	_swaps_active_begin_idx = SWAPS_SIZE;
}

void SwapContainer::flip( const size_t idx ) {
	return test( idx ) ? reset( idx ) : set( idx );
}

bool SwapContainer::test( size_t pos ) const {
	return _swaps_active.test( pos );
}

bool SwapContainer::any( void ) const {
	return _swaps_active.any();
}

SwapContainer::iterator::iterator(SwapContainer & ContainerParam, size_t index)
	:Container(ContainerParam)
	,_idx(index)
{
}

const swap_type & SwapContainer::iterator::operator*() const
{
	return _swaps[_idx];
}


SwapContainer::iterator & SwapContainer::iterator::operator++()
{
	while( _idx < SWAPS_SIZE && ! Container._swaps_active[++_idx] ) ;
	return *this;
}

//SwapContainer::iterator & SwapContainer::iterator::operator++(int i)
//{
//return ++(*this);
//}

bool SwapContainer::iterator::operator!=(const iterator & rhs) const
{
	return _idx != rhs._idx;
}

size_t SwapContainer::size() const
{
	return _swaps_active.count();
}

SwapContainer::iterator SwapContainer::begin()
{
	//return SwapContainer::iterator(*this, __builtin_ctz( _swaps_active.to_ulong() ));
	return SwapContainer::iterator(*this, _swaps_active_begin_idx );
}
SwapContainer::iterator SwapContainer::end()
{
	return SwapContainer::iterator(*this, SWAPS_SIZE);
}


