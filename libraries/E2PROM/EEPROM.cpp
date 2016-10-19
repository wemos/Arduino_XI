/*
  EEPROM.cpp - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <avr/eeprom.h>
#include "Arduino.h"
#include "EEPROM.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

uint8_t EEPROMClass::read(int address)
{
	 EEARL = address & 0xff;
	 EEARH = (address >> 8) & 0x1;
	 
	 EECR |= (1 << EERE);
	 asm("nop"); asm("nop");
	 
	 return EEDR;
	
}

void EEPROMClass::write(int address, uint8_t value)
{
 	 unsigned char tmp;
	 
	 // set address & data
	 EEARL = address & 0xff;
	 EEARH = (address >> 8) & 0x1;
	 EEDR = value;
	 
	 tmp = EECR;
	 tmp |= (1 << EEMPE);
	 tmp &= ~(1 << EEPE);
	 EECR = tmp;
	 EECR |= (1 << EEPE);
	 asm("nop"); asm("nop"); 
}

EEPROMClass EEPROM;
