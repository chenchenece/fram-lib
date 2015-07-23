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


#include "application.h"




// Possible FRAM sizes
enum framAvailSize
{
	MS16 = 16,
	MS64 = 64,
	MS256 = 256
};

class FRAM
{
private:

	uint8_t _cs;
	framAvailSize _size;

  static const byte _CMD_WREN	=	0x06;	  // 0000 0110 Set Write Enable Latch
  static const byte _CMD_WRDI	=	0x04;	  // 0000 0100 Write Disable
  static const byte _CMD_RDSR	=	0x05;	  // 0000 0101 Read Status Register
  static const byte _CMD_WRSR	=	0x01;	  // 0000 0001 Write Status Register
  static const byte _CMD_READ	=	0x03;	  // 0000 0011 Read Memory Data
  static const byte _CMD_WRITE = 0x02;	// 0000 0010 Write Memory Data
  static const byte _DUMMY_BYTE	= 0x7E;	// Dummy write value for SPI read

	void _enable();
	void _disable();
	int8_t _sendCMD(byte command);



public:

	FRAM(uint8_t chipSelect, framAvailSize framSize);
	int8_t Read(uint16_t addr, uint8_t* buf, uint16_t count);
	int8_t Write(uint16_t addr, uint8_t* buf, uint16_t count);
	int8_t Present(void);
	int16_t MaxAdd(void);
	void init(void);
};
