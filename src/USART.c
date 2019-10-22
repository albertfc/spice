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
#include <avr/io.h>
#include <stdio.h>

#define BAUD_PRESCALE(_f_cpu_, _baudrate_ ) \
  ((( _f_cpu_ / (_baudrate_ * 8UL))) - 1)

void USART_Init( unsigned long int f_cpu, unsigned int baud )
{
  baud = BAUD_PRESCALE( f_cpu, baud );

  /* Enable double transmission speed */
  UCSR0A = (1<<U2X0);
  /* Set baud rate */
  UBRR0H = (unsigned char)(baud>>8);
  UBRR0L = (unsigned char)baud;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}

static void USART_Transmit( unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) )
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

static unsigned char USART_Receive( void )
{
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)) )
    ;
  /* Get and return received data from buffer */
  return UDR0;
}

static int USART_printf(char var, FILE *stream)
{
  /* translate \n to \r for br@y++ terminal */
  if( var == '\n' )
    USART_Transmit( '\r' );
  USART_Transmit( var );
  return 0;
}

FILE usart_file = FDEV_SETUP_STREAM( USART_printf, NULL, _FDEV_SETUP_WRITE );

