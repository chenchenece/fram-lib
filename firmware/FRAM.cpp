/**
 * Copyright 2014  Paul Kourany
 * Copyright 2014  Kenneth Lim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.


 Fujitsu FRAM Library for Spark Core
 Originally written by Paul Kourany

 Maintained by Kennethlimcp
 */

#if PLATFORM_ID == 0 // Core
 #define pinLO(_pin) (PIN_MAP[_pin].gpio_peripheral->BRR = PIN_MAP[_pin].gpio_pin)
 #define pinHI(_pin) (PIN_MAP[_pin].gpio_peripheral->BSRR = PIN_MAP[_pin].gpio_pin)
#elif PLATFORM_ID == 6 // Photon
 #include "pinmap_hal.h"
 STM32_Pin_Info* PIN_MAP = HAL_Pin_Map();
 #define pinLO(_pin) (PIN_MAP[_pin].gpio_peripheral->BSRRH = PIN_MAP[_pin].gpio_pin)
 #define pinHI(_pin) (PIN_MAP[_pin].gpio_peripheral->BSRRL = PIN_MAP[_pin].gpio_pin)
#else
 #error "*** PLATFORM_ID not supported by this library. PLATFORM should be Core or Photon ***"
#endif
// fast pin access
#define pinSet(_pin, _hilo) (_hilo ? pinHI(_pin) : pinLO(_pin))

#include "FRAM.h"

FRAM::FRAM(uint8_t chipSelect, framAvailSize framSize)
{
  _size = framSize;
	_cs = chipSelect;
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
}

void FRAM::_enable(void){
  pinLO(_cs);	//FRAM CS LOW
}

void FRAM::_disable(void){
  pinHI(_cs);	//FRAM CS HIGH
}

int8_t FRAM::_sendCMD(byte command){
  _enable();
  command = SPI.transfer(command); /// Send opcode
  _disable();
  return command;
}

int8_t FRAM::Read(uint16_t addr, uint8_t* buf, uint16_t count = 1)
{
  if (addr > MaxAdd())
	return -1;

  _enable();

  SPI.transfer(_CMD_READ);
  SPI.transfer((uint8_t)(addr >> 8));
  SPI.transfer((uint8_t)(addr & 0xFF));

  for (uint16_t i=0; i < count; i++)
		buf[i] = SPI.transfer(_DUMMY_BYTE);

  _disable();

  return 0;
}


int8_t FRAM::Write(uint16_t addr, uint8_t* buf, uint16_t count = 1)
{
  if (addr > MaxAdd())
	return -1;

  _sendCMD(_CMD_WREN);  // Enable write;

  _enable();

  SPI.transfer(_CMD_WRITE); //write command
  SPI.transfer((uint8_t)(addr >> 8));
  SPI.transfer((uint8_t)(addr & 0xFF));

  for (uint16_t i = 0;i < count;i++)
	   SPI.transfer(buf[i]);

  _disable();

  return 0;
}


// Tests that the unused status register bits can be read, inverted, written back and read again
int8_t FRAM::Present(void) {

	const byte srMask = 0x70; // Unused bits are bits 6..4
	byte registerValue = 0;
	byte newValue = 0;

	_sendCMD(_CMD_RDSR);  // Read current status register opcode

	registerValue = _sendCMD(_DUMMY_BYTE); // Read status register value

	// Invert current value
	newValue = registerValue ^ srMask;

	_sendCMD(_CMD_WREN); // Enable write

  _enable();
  SPI.transfer(_CMD_WRSR); 	// Write status register opcode
	SPI.transfer(newValue);  // Write new value to status register
  _disable();

	// Read again
  _enable();
	SPI.transfer(_CMD_RDSR); // Read current status register opcode
	registerValue = SPI.transfer(_DUMMY_BYTE); // Read status register value
  _disable();

	if ((registerValue & srMask) == (newValue & srMask))
		return 1;
	else
		return 0;
}

int16_t FRAM::MaxAdd(void){
  switch (_size)
  {
      case 16:
        return 0x07FF;
      case 64:
        return 0x1FFF;
      case 256:
        return 0x7FFF;
  }
}


void FRAM::init(void){
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}
