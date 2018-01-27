/*
Example for working with AT93Cxx EEPROMs 
AT93C46 1Kbit (128 x 8 or 64 x 16) - 2 MHz > 4.5V
AT93C56 2Kbit (256 x 8 or 128 x 16) - 2 MHz > 4.5V 
AT93C66 4Kbit (512 x 8 or 256 x 16) - 2 MHz > 4.5V 
You can find spi pins for your arduino in here:
URL: http://www.arduino.cc/en/Reference/SPI
Write by Alireza Noubrakhsh
Thanks From Hossein Boustani
*/
#include <AT93CXX.h>

//CS        53 // connect to pin 1 of 93C46 - Chip select pin
//CLK       52  //connect to pin 2 of 93C46
//DATAOUT   51  //connect to pin 3 of 93C46 
//DATAIN    50  //connect to pin 4 of 93C46

AT93CXX AT93(emAT93C46,false,53);

void setup() {
  Serial.begin(9600);
  AT93.Begin();  
}


void loop() {
  AT93.EWEN(); //Write enable must precede all programming modes.
  AT93.ERAL(); //Erases all memory locations. Valid only at VCC = 4.5V to 5.5V.
  AT93.WRITE(0, 0xC0);//Write First Address
  AT93.WRITE(AT93.MaxAddr, 0xC1);//Write End Address
  word buf;
  for(int16_t i = 0; i <= AT93.MaxAddr; i++)
  {
    buf = AT93.READ(i);
    Serial.print(i, DEC);
    Serial.print(" : ");
    Serial.println(buf, HEX);
  }
  delay(10000);
}












