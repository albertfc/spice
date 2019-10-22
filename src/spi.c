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
#include "spi.h"
#include "register.h"
#include "constants.h"

#include <avr/io.h>

inline uint8_t spi_master_sndrcv( const uint8_t data )
{
	SPDR = data;                    /* Start transmission */
	while( !(SPSR & _BV( SPIF )) ); /* Wait for reception complete */
	return SPDR;                    /* Return Data Register */
}

inline void spi_dac_write( uint16_t sample, volatile uint8_t * const port, volatile uint8_t pin )
{
	/* Configuration bits */
	sample &= ~_BV( 15 ); /* Write DAC register */
	sample &= ~_BV( 14 ); /* Unbuffered input */
	sample |=  _BV( 13 ); /* 1x gain */
	sample |=  _BV( 12 ); /* Set Vout available */

	*port &=~_BV( pin ); /* Drive CV low  */
	spi_master_sndrcv( sample >> 8 );
	spi_master_sndrcv( sample      );
	*port |= _BV( pin ); /* Drive CV high */
}

inline uint16_t spi_adc_read( volatile uint8_t * const port, volatile uint8_t pin )
{
	uint8_t spi_data[2];

	*port &=~_BV( pin ); /* Drive CV low  */
	spi_data[0]= spi_master_sndrcv( 0 );
	spi_data[1]= spi_master_sndrcv( 0 );
	*port |= _BV( pin ); /* Drive CV high */

	return  ((uint16_t)spi_data[1] >> 1) + ((uint16_t)(spi_data[0] & 0x001F) << 7);
}

void spi_init( void )
{
	/* Set outputs */
	DDR_SPI     |= _BV( DD_SCK ) | _BV( DD_MOSI );
	DDR(  DAC ) |= _BV( DD( DAC ) );
	DDR(  ADC ) |= _BV( DD( ADC ) );
	DDR( SS  )  |= _BV( DD( SS  ) );
	PORT( DAC ) |= _BV( P( DAC ) ) ; /* Drive DAC high */
	PORT( ADC ) |= _BV( P( ADC ) ) ; /* Drive ADC high */
	PORT( SS  ) |= _BV( P( SS  ) ) ; /* Drive SS  high */

	SPCR |= _BV( SPE ) | _BV( MSTR ); /* Enable SPI, Master */
	SPCR &=~_BV( SPR0 );
	SPCR &=~_BV( SPR1 );
	SPSR |= _BV( SPI2X );
}

inline void spi_shiftreg_update( const uint8_t value )
{
	spi_master_sndrcv( value );
	spi_master_sndrcv( value );
	WRT0_PIN( SS );
	WRT1_PIN( SS );
}


