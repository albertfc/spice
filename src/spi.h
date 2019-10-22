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

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
uint8_t spi_master_sndrcv( const uint8_t data );
void spi_dac_write( uint16_t sample, volatile uint8_t * const port, volatile uint8_t pin );
uint16_t spi_adc_read( volatile uint8_t * const port, volatile uint8_t pin );
void spi_shiftreg_update( const uint8_t value );
void spi_init( void );
#ifdef __cplusplus
}
#endif
