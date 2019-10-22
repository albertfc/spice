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
#include "register.h"
#include "constants.h"
#include "scaler.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include <stdint.h>
#include "ustl/ualgo.h"

#include "Model.hpp"
#include "Timer.hpp"
extern volatile Timer timer;

#include "Controller.hpp"

// Potentiometers
#define SRPOT   0
#define BRPOT   1
#define NPOTS   2
#define DEF_POT SRPOT

#if SR_POT_RESPONSE == LOG_TABLE
#include <avr/pgmspace.h>
#include "log_table.h"
const unsigned char to_log10[INT_ADC_NVAL] PROGMEM = { SEQ_BYTE_LOG10 };
#endif

#if BC_POT_RESPONSE == LINEAL
volatile uint16_t br_mask = 0xFFFF; // Bit crusher mask
#elif BC_POT_RESPONSE == FRACTIONAL_TABLE
#include <avr/pgmspace.h>
#include "log_table.h"
const unsigned short toname[INT_ADC_NVAL] PROGMEM = { SEQ_SHORT_12BITS }; // TODO: find var name!
volatile uint32_t br_values = EXT_ADC_NVAL;; // Number of diferent values for a sample to take.
static inline uint8_t log2(const uint16_t x){return (15-__builtin_clz (x));}
#endif
volatile uint8_t br_bits = 0; // Number of chrushed  bits

ISR( ADC_vect )
{
	uint8_t int_adc_sample = 0xff - ADCH;
	unsigned long int sr_freq;

	static uint8_t current_adc_channel = DEF_POT;
	static uint8_t next_adc_channel;

	timer.run();

	switch( current_adc_channel )
	{
		case BRPOT: /* BR pot */
#if BC_POT_RESPONSE == LINEAL
			br_bits = (int_adc_sample * EXT_ADC_BITRES) / INT_ADC_NVAL;
			br_mask = 0xFFFF << br_bits;
#elif BC_POT_RESPONSE == FRACTIONAL_TABLE
			br_values = pgm_read_word( &(toname[0xff-int_adc_sample]) );
			br_bits = EXT_ADC_BITRES - log2( br_values );
#endif
			next_adc_channel = SRPOT;
			break;

		case SRPOT: /* SRR pot */
#if SR_POT_RESPONSE == LINEAL
			sr_freq = SR_TIMER_FMAX - (int_adc_sample * (SR_TIMER_FMAX - SR_TIMER_FMIN)) / INT_ADC_NVAL;
			TIMER_SET_OCR_VALUE( OCR1B, SR_TIMER_SCALER, sr_freq );
#elif SR_POT_RESPONSE == LOG_TABLE
			sr_freq = SR_TIMER_FMAX - ( pgm_read_byte(&(to_log10[int_adc_sample])) * (SR_TIMER_FMAX - SR_TIMER_FMIN)) / INT_ADC_NVAL;
			TIMER_SET_OCR_VALUE( OCR1B, SR_TIMER_SCALER, sr_freq );
#endif
			next_adc_channel = BRPOT;

			break;
	}

	ADMUX = (ADMUX & 0xF8) | next_adc_channel; /* Set next channel to read from */
	current_adc_channel = next_adc_channel;
}

static class Button: public TimerIface {
public:
	virtual void increments_one_ms( void )
	{
		const uint8_t curr_value = !(READ_PIN( BUTTON ));
		_ms = _value == curr_value ? ustl::min(_ms, UINT16_MAX-1)+1 : 0;
		_value = curr_value;

		switch( _status ) {
			case bs_released:
				if( _value && _ms > BOUNCING_TIME )
					_status = bs_pushed;
				break;
			case bs_pushed:
				if( !_value )
					_status = bs_intermediate;
				else if( _ms > LONG_TIME ) {
					_status      = bs_long_pressed;
					_curr_action = _action[ba_long];
				}
				break;
			case bs_long_pressed:
				if( !_value )
					_status = bs_released;
				break;
			case bs_double_clicked:
				if( !_value )
					_status = bs_released;
				break;
			case bs_intermediate:
				if( !_value && _ms > SINGLE_TIME ) {
					_status      = bs_released;
					_curr_action = _action[ba_single];
				} else if( _value && _ms > BOUNCING_TIME ) {
					_status      = bs_double_clicked;
					_curr_action = _action[ba_double];
				}
				break;
		}
		_update();
	}
	Button( void (*action_single)(void), void (*action_double)(void), void (*action_long)(void) ):
		_action{action_single, action_double, action_long} {}
private:
	enum _ButtonStatus { bs_released, bs_pushed, bs_long_pressed, bs_double_clicked, bs_intermediate };
	enum _ButtonActions {ba_single, ba_double, ba_long, ba_size};
	volatile _ButtonStatus    _status;
	volatile uint16_t _ms;  // miliiseconds (10^-3s.)
	volatile bool _value;
	void (* _curr_action         )(void) = nullptr;
	void (*const _action[ba_size])(void);
	void _update( void )
	{
		if( !_curr_action )
			return;
		_curr_action();
		_curr_action = nullptr;
	}

} button(
		&Model::toggle_value ,
		&Model::next_mode,
		&Model::reset
);

struct EncoderIface {
	virtual ~EncoderIface() = default;
	virtual void left( void ) = 0;
	virtual void right( void ) = 0;
};

class Multiplexer: public EncoderIface, public TimerIface {
	private:
		uint8_t _status = 0;
		uint8_t _prev_status = 0;
		static uint8_t EEMEM _status_ptr;

		void _update( void ) {
			switch( _status ) {
				case 0:
					PORT( MPXA ) &= ~_BV( P( MPXA ) );
					PORT( MPXB ) &= ~_BV( P( MPXB ) );
					PORT( MPXC ) &= ~_BV( P( MPXC ) );
					PORT( FUZZCTRL ) |= _BV( P( FUZZCTRL ) );
					break;
				case 1:
					PORT( MPXA ) |=  _BV( P( MPXA ) );
					PORT( MPXB ) &= ~_BV( P( MPXB ) );
					PORT( MPXC ) &= ~_BV( P( MPXC ) );
					PORT( FUZZCTRL ) |= _BV( P( FUZZCTRL ) );
					break;
				case 2:
					PORT( MPXA ) &= ~_BV( P( MPXA ) );
					PORT( MPXB ) |=  _BV( P( MPXB ) );
					PORT( MPXC ) &= ~_BV( P( MPXC ) );
					PORT( FUZZCTRL ) |= _BV( P( FUZZCTRL ) );
					break;
				case 3:
					PORT( MPXA ) |=  _BV( P( MPXA ) );
					PORT( MPXB ) |=  _BV( P( MPXB ) );
					PORT( MPXC ) &= ~_BV( P( MPXC ) );
					PORT( FUZZCTRL ) |= _BV( P( FUZZCTRL ) );
					break;
				case 4:
					PORT( MPXA ) |=  _BV( P( MPXA ) );
					PORT( MPXB ) &= ~_BV( P( MPXB ) );
					PORT( MPXC ) |=  _BV( P( MPXC ) );
					PORT( FUZZCTRL ) &= ~_BV( P( FUZZCTRL ) );
					break;
				case 5:
					PORT( MPXA ) &= ~_BV( P( MPXA ) );
					PORT( MPXB ) &= ~_BV( P( MPXB ) );
					PORT( MPXC ) |=  _BV( P( MPXC ) );
					PORT( FUZZCTRL ) &= ~_BV( P( FUZZCTRL ) );
					break;
				case 6:
					PORT( MPXA ) &= ~_BV( P( MPXA ) );
					PORT( MPXB ) |=  _BV( P( MPXB ) );
					PORT( MPXC ) |=  _BV( P( MPXC ) );
					PORT( FUZZCTRL ) |= _BV( P( FUZZCTRL ) );
					break;
			}
		}

	public:
		Multiplexer( void ) {
			eeprom_busy_wait();
			_status = eeprom_read_byte( &_status_ptr );
			_update();
		}

		virtual void right( void ) {
			_status = (_status + 1) % 7;
			_update();
		}
		virtual void left( void ) {
			_status = _status ? _status - 1 : 6;
			_update();
		}
		virtual void increments_one_ms( void ) {
			if( _prev_status == _status )
				return;
			
			eeprom_busy_wait();
			eeprom_write_byte( &_status_ptr, _status );
			_prev_status = _status;
		}
} multiplexer;
uint8_t EEMEM Multiplexer::_status_ptr = 0;

template<int limit>
class BitEditor: public EncoderIface {
public:
	virtual void left(  void ) {
		_bit = _bit ? _bit - 1 : limit-1;
		Model::set_value( _bit );
	}
	virtual void right( void ) {
		_bit = (_bit + 1) % limit;
		Model::set_value( _bit );
	}
private:
	uint8_t _bit = 0;
};
static BitEditor<EXT_ADC_BITRES> mute_bit_editor;
static BitEditor<EXT_ADC_BITRES> flip_bit_editor;
static BitEditor<SWAPS_SIZE>     swap_bit_editor;

static class Encoder {
public:
	void left(  void ) {
		_encoder[Model::get_mode()]->left();
	}
	void right( void ) {
		_encoder[Model::get_mode()]->right();
	}
private:
	EncoderIface * _encoder[MODES_SIZE] = {
		&multiplexer,
		&mute_bit_editor,
		&flip_bit_editor,
		&swap_bit_editor };
} encoder;

ISR( PCINT0_vect )
{
	static uint8_t enc_counter = 0;

	enc_counter++;
	if( enc_counter < 1 )
		return;

	if( READ_PIN( ENCODERA ) )
		return;

	enc_counter = 0;
	if( READ_PIN( ENCODERB ) )
		encoder.right();
	else
		encoder.left();
}

void Controller::init( void ) {
	timer.register_ms( &button );
	timer.register_ms( &multiplexer );
}
