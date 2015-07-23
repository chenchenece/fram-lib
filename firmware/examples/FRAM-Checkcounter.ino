#include "FRAM/FRAM.h"


/*
This example demonstrates how the FRAM library is used to:

1) Check if a FRAM is/are present
2) Read the byte from address 0x00 which stores the number of times
   the FRAM is written to during manufacturing

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

  while(!Serial.available()) Spark.process(); // PRESS ENTER TO CONTINUE
  Serial.println("Starting FRAM READ test\n");

  if(fram_1.Present() == 1){
    Serial.print("Found SPI FRAM 1");
    Serial.print("\nThe entire memory has been written: ");     // Read the first byte
    fram_1.Read(0x0, &fram_counter, 1);
    Serial.print(fram_counter);
    Serial.println(" times!");
  }
  else{
    Serial.println("SPI FRAM 1 NOT found ... check your connections\r\n");
  }

  if(fram_2.Present() == 1){
    Serial.print("Found SPI FRAM 2");
    Serial.print("\nThe entire memory has been written: ");     // Read the first byte
    fram_2.Read(0x0, &fram_counter, 1);
    Serial.print(fram_counter);
    Serial.println(" times!");
  }
  else{
    Serial.println("SPI FRAM 2 NOT found ... check your connections\r\n");
  }


  Serial.println("\nPress any key to re-run the test!\n");
  while(Serial.available()) Serial.read();      // MAKESHIFT FLUSH
  while(!Serial.available()) Spark.proces(); // PRESS ENTER TO CONTINUE
}
