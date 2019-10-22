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
#if !defined( __AVR_ATmega328P__ )
#warning "This code is intended to be built for an atmega328p"
#endif

#include "config.h"
#include "register.h"
#include "constants.h"
#include "scaler.h"
#include "spi.h"
#include "utils.hpp"

#include <avr/interrupt.h>

#include "View.hpp"
#include "Model.hpp"

// Global variables
#if BC_POT_RESPONSE == LINEAL
extern volatile uint16_t br_mask; // Bit crusher mask
#elif BC_POT_RESPONSE == FRACTIONAL_TABLE
extern volatile uint32_t br_values; // Number of diferent values for a sample to take.
#endif
extern volatile uint8_t br_bits; // Number of chrushed  bits

static void int_adc_init( void )
{
	ADMUX |= _BV( REFS0 ); /* Set ADC reference to AVCC */
	ADMUX |= _BV( ADLAR ); /* ADC left adjust result */
	ADMUX = (ADMUX & 0xF8);

	ADCSRA |= _BV( ADPS2 ) | _BV( ADPS1 ) | _BV( ADPS0 ); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
	ADCSRA |= _BV( ADATE );
	ADCSRB |= _BV( ADTS1 ) | _BV( ADTS0 ); /* Set ADC to Timer/Counter0 Compare Match */
	ADCSRA |= _BV( ADEN ); /* Enable ADC */
	ADCSRA |= _BV( ADIE ); /* Enable ADC Interrupt */
	ADCSRA |= _BV( ADSC ); /* Start A2D Conversions */
}

static void pot_timer_init( void )
{
	TCCR0A |= _BV( WGM01 ); /* Configure timer 0 for CTC mode */

	TIMER_SET_SCALER( TCCR0B, POT_TIMER_SCALER, 0 );
	TIMER_SET_OCR_VALUE( OCR0A, POT_TIMER_SCALER, POT_TIMER_FREQ );

	TIMSK0 |= _BV( OCIE0A ); /* Enable interrupt */
}
EMPTY_INTERRUPT( TIMER0_COMPA_vect );

static void sr_timer_init( void )
{
	TIMER_SET_SCALER( TCCR1B, SR_TIMER_SCALER, 1 );
	TIMER_SET_OCR_VALUE( OCR1B, SR_TIMER_SCALER, SR_TIMER_FMAX );
}

static inline void ddr_init( void )
{
	DDR( MPXA ) |= _BV( DD( MPXA ) );
	DDR( MPXB ) |= _BV( DD( MPXB ) );
	DDR( MPXC ) |= _BV( DD( MPXC ) );

	DDR( ENCODERA ) &= ~_BV( DD( ENCODERA ) );
	DDR( ENCODERB ) &= ~_BV( DD( ENCODERB ) );

	DDR( BUTTON ) &= ~_BV( DD( BUTTON ) );

	DDR( FUZZCTRL ) |= _BV( DD( FUZZCTRL ) );
}

int main( void )
{
	ddr_init();
	spi_init();
	int_adc_init();
	pot_timer_init();
	sr_timer_init();

	/**
	 * Pin Change Interrupt enable on PCINT0 (PB0)
	 */
	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT0);

	sei(); /* Enable Global Interrupts */

	spi_dac_write( 0, &PORT( DAC ), P( DAC ) ); // Reset DAC

	Model::init();

	while( 1 ) {
		View::update();

		Model::set_bits_normal( EXT_ADC_BITRES-br_bits );

		auto sample = spi_adc_read( &PORT( ADC ), P( ADC ) ); /* Get sample */
		sample = Model::process_sample( sample );
#if BC_POT_RESPONSE == LINEAL
		sample &= br_mask;
#elif BC_POT_RESPONSE == FRACTIONAL_TABLE
		sample = (EXT_ADC_NVAL / br_values) * ((sample * br_values) / EXT_ADC_NVAL);
#endif
		spi_dac_write( sample, &PORT( DAC ), P( DAC ) ); /* Write sample */

		while( !(TIFR1 & _BV( OCF1B )) ); /* Wait for SR timer interruption */
		TCNT1 = 0;                        /* Reset output timer */
		TIFR1 |= _BV( OCF1B );            /* Clear interrupt flag */
	}
}
