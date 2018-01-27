#include <SPI.h>

/*
AT93C46 1Kbit (128 x 8 or 64 x 16) - 2 MHz > 4.5V
AT93C56 2Kbit (256 x 8 or 128 x 16) - 2 MHz > 4.5V 
AT93C66 4Kbit (512 x 8 or 256 x 16) - 2 MHz > 4.5V 
Write by Alireza Noubrakhsh
Thanks From Hossein Boustani
*/

//CS 		53  //connect to pin 1 of 93C46 - Chip select pin
//CLK       52  //connect to pin 2 of 93C46
//DATAOUT   51  //connect to pin 3 of 93C46 
//DATAIN    50  //connect to pin 4 of 93C46

typedef enum
{
  emAT93C46        = (1),   
  emAT93C56        = (2),
  emAT93C66        = (3)
} eeprom_type_def;


class AT93CXX {
 public:
  int16_t MaxAddr;
  AT93CXX(eeprom_type_def eeprom_type = emAT93C46,bool org_enabled = false,uint8_t cspin = 53);
  void Begin();
  void End();
  void EWEN();
  void EWDS();
  void ERAL();
  bool ERASE(int16_t loc);
  word READ(int16_t loc);
  bool WRITE(int16_t loc, word data);
 private:
  uint8_t _cspin,_org_enabled;
  uint16_t _maxaddr;
  eeprom_type_def  _eeprom_type;
};

