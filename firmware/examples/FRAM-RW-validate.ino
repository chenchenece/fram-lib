#include "FRAM/FRAM.h"

/*
This example demonstrates how the FRAM library is used to:

1) Check if a FRAM is/are present
2) Read the byte from address 0x00 which stores the number of times
   the FRAM is written to during manufacturing
3) Write to ALL the memory space available and report any mismatch between
   the written and read back value

We will be using the Serial Terminal to output the results
*/


// Change accordingly to the FRAM size you have on the shield
FRAM fram_1(D2,MS64);
FRAM fram_2(D3,MS64);

void setup(void) {

  Serial.begin(9600); // Enumerate COM port

  fram_1.init();      // Only need to be called once to setup SPI settings by either fram_1 or fram_2

  SPI.begin();
}

void loop() {

  uint8_t fram_counter;
  unsigned long write_time;
  uint16_t add;

  while(!Serial.available()) Spark.process(); // PRESS ENTER TO CONTINUE
  Serial.println("Starting FRAM Validation test\n");

  if(fram_1.Present() == 1){
    Serial.print("Found SPI FRAM 1");
    Serial.print("\nThe entire memory has been written: ");     // Read the first byte
    fram_1.Read(0x0, &fram_counter, 1);
    Serial.print(fram_counter);
    Serial.println(" times!");

    // Increment the FRAM counter value stored in address 0x00
    fram_counter++;
    fram_1.Write(0x0, &fram_counter, 1);

    write_time = micros();

    Serial.println("Writing to FRAM 1 now..");
    // Write to Address and Read back with verification
    for (add = 1; add <= fram_1.MaxAdd(); add++) {
      uint8_t read;
      uint8_t data = (uint8_t)(add & 0xFF);
      fram_1.Write(add, &data, 1);
      fram_1.Read(add, &read,1);

      // Data checking
      if(read != data){
        Serial.print("RW mismatch at: ");
        Serial.print(add,HEX);
        Serial.print("\n");
        }
    }

    write_time = micros() - write_time;

    Serial.print("Last Address written to: ");
    Serial.print(add-1,HEX);
    Serial.print("\nTime taken: ");
    Serial.print(write_time);
    Serial.print("us\n");
  }
  else{
    Serial.println("\nSPI FRAM 1 NOT found ... check your connections\r\n");
  }

  if(fram_2.Present() == 1){
    Serial.print("\nFound SPI FRAM 2");
    Serial.print("\nThe entire memory has been written: ");     // Read the first byte
    fram_2.Read(0x0, &fram_counter, 1);
    Serial.print(fram_counter);
    Serial.println(" times!");


    // Increment the FRAM counter value stored in address 0x00
    fram_counter++;
    fram_2.Write(0x0, &fram_counter, 1);

    write_time = micros();

    Serial.println("Writing to FRAM 2 now..");
    // Write to Address and Read back with verification
    for (add = 1; add <= fram_2.MaxAdd(); add++) {
      uint8_t read;
      uint8_t data = (uint8_t)(add & 0xFF);
      fram_2.Write(add, &data, 1);
      fram_2.Read(add, &read,1);

      // Data checking
      if(read != data){
        Serial.print("RW mismatch at: ");
        Serial.print(add,HEX);
        Serial.print("\n");
        }
    }

    write_time = micros() - write_time;

    Serial.print("Last Address written to: ");
    Serial.print(add-1,HEX);
    Serial.print("\nTime taken: ");
    Serial.print(write_time);
    Serial.print("us\n");
  }
  else{
    Serial.println("\nSPI FRAM 2 NOT found ... check your connections\r\n");
  }


  Serial.println("\nPress any key to re-run the test!\n");
  while(Serial.available()) Serial.read();      // MAKESHIFT FLUSH
  while(!Serial.available()) Spark.proces(); // PRESS ENTER TO CONTINUE
}
