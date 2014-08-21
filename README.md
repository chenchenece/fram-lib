fram-lib
========

FRAM library for Fujitsu IC

The following functions are available in this library:

* FRAM(uint8_t chipSelect, framAvailSize framSize);
* int8_t Read(uint16_t addr, uint8_t* buf, uint16_t count);
* int8_t Write(uint16_t addr, uint8_t* buf, uint16_t count);
* int8_t Present(void);
* int16_t MaxAdd(void);
* void init(void);

You will be able to better understand how they can be used in this  [example](/firmware/examples/FRAM-RW-validate.ino).

Updated on: 21 August 2014
