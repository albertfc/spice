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

// Main clock frequency
#define F_CPU 16000000UL

// SPI configuration
#define DDR_SPI  DDRB
#define DD_MOSI  DDB3
#define DD_SCK   DDB5

// Digital I/O
#define DAC_REG D
#define DAC_PIN 7
#define ADC_REG B
#define ADC_PIN 2

#define SS_REG D
#define SS_PIN 2

#define MPXA_REG D
#define MPXA_PIN 3
#define MPXB_REG D
#define MPXB_PIN 4
#define MPXC_REG D
#define MPXC_PIN 5

#define ENCODERA_REG B
#define ENCODERA_PIN 0
#define ENCODERB_REG B
#define ENCODERB_PIN 1

#define FUZZCTRL_REG D
#define FUZZCTRL_PIN 6

#define BUTTON_REG C
#define BUTTON_PIN 2

// ADCs resolution
#define INT_ADC_BITRES 8
#define INT_ADC_NVAL   (1 << INT_ADC_BITRES)
#define EXT_ADC_BITRES 12
#define EXT_ADC_NVAL   (1 << EXT_ADC_BITRES)
#define BR_NBITS (EXT_ADC_BITRES-1)
#if BR_NBITS > EXT_ADC_BITRES
#error "Crushed bits is greater than external ADC resolution"
#endif

// Sample Rate min and max frequencies
#define SR_TIMER_SCALER 1
#define SR_TIMER_FMAX 12000UL
#define SR_TIMER_FMIN 80UL

// Sample rate potentiometer response
#define LINEAL    0
#define LOG_TABLE 1
#define SR_POT_RESPONSE LINEAL

// Bit crusher potentiometer response
/*#define LINEAL    0*/
#define FRACTIONAL_TABLE 1
#define BC_POT_RESPONSE FRACTIONAL_TABLE

// Potentiometers sample frequency
#define POT_TIMER_SCALER 1024
#define POT_TIMER_FREQ   1000UL

// Button times in miliseconds
#define BOUNCING_TIME 0
#define LONG_TIME  1000
#define SINGLE_TIME 200 // Max time bettween clicks in a double click

// 7S blink frequencies
#define FREQ_7S_NORMAL  0
#define FREQ_7S_MUTE   10
#define FREQ_7S_FLIP   20
#define FREQ_7S_SWAP   40

// Max timer registered objects 
#define TIME_OBJS_MAX 3

// 7 segments hex values
// First 12 values are used for represeting bits indicators (from 0 to b)
// next values represent swap states 
#define SEVEN_SEGMENTS_HEX \
	0b11000000,\
	0b11111001,\
	0b10100100,\
	0b10110000,\
	0b10011001,\
	0b10010010,\
	0b10000010,\
	0b11111000,\
	0b10000000,\
	0b10010000,\
	0b10001000,\
	0b10000011,\
	0b11000110,\
	0b11111110,\
	0b11111101,\
	0b11111011,\
	0b11110111,\
	0b11101111,\
	0b11011111,\
	0b11110110,\
	0b11101101,\
	0b11011011,\
	0b10110110,\
	0b10100010,\
	0b10100011,\
	0b10011101,\
	0b10101011,\
	0b10011100,\
	0b10101010,\
	
// Bit swaps list
#define SWAPS_SIZE 16 // Number of bit swaps in the following list.
#define DEFINE_SWAPS \
	static const swap_unit swap00[] = { {0, 4}, {-1,-1} }; \
	static const swap_unit swap01[] = { {1, 3}, {-1,-1} }; \
	static const swap_unit swap02[] = { {2, 8}, {-1,-1} }; \
	static const swap_unit swap03[] = { {3, 6}, {-1,-1} }; \
	static const swap_unit swap04[] = { {4, 5}, {-1,-1} }; \
	static const swap_unit swap05[] = { {4, 9}, {-1,-1} }; \
	static const swap_unit swap06[] = { {7, 9}, {-1,-1} }; \
	static const swap_unit swap07[] = { {8, 10}, {-1,-1} }; \
	static const swap_unit swap08[] = { {6, 10}, {-1,-1} }; \
	static const swap_unit swap09[] = { {3, 11}, {-1,-1} }; \
	static const swap_unit swap10[] = { {7, 11}, {-1,-1} }; \
	static const swap_unit swap11[] = { {8, 11}, {-1,-1} }; \
	static const swap_unit swap12[] = { {9, 11}, {-1,-1} }; \
	static const swap_unit swap13[] = { {10, 11}, {-1,-1} }; \
	static const swap_unit swap14[] = { {5, 8}, {-1,-1} }; \
	static const swap_unit swap15[] = { {1, 10}, {-1,-1} }; \
	const swap_type SwapContainer::_swaps[] = { \
		swap00, swap01, swap02, swap03, swap04, swap05, swap06, swap07, swap08, swap09, swap10, swap11, swap12, swap13, swap14, swap15 \
	};
