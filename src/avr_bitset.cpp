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
#include "Model.hpp"
#include "avr_bitset.hpp"

// init static members
template<> avr_bitset<EXT_ADC_NBITS, 0>::value_type EEMEM avr_bitset<EXT_ADC_NBITS, 0>::_eeprom_bits[] = {0xffffffff};
template<> avr_bitset<EXT_ADC_NBITS, 1>::value_type EEMEM avr_bitset<EXT_ADC_NBITS, 1>::_eeprom_bits[] = {0};
template<> avr_bitset<SWAPS_SIZE,    2>::value_type EEMEM avr_bitset<SWAPS_SIZE,    2>::_eeprom_bits[] = {0};
