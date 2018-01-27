#include <AT93CXX.h>

SPISettings AT93CXX_SPISettings(2000000, MSBFIRST, SPI_MODE0); //2 MHz Clock Rate = 2000000

AT93CXX::AT93CXX(eeprom_type_def eeprom_type,bool org_enabled,uint8_t cspin)
{
  _cspin = cspin;
  _eeprom_type = eeprom_type;
  _maxaddr = 128;
  switch (_eeprom_type) {
	   case emAT93C46: _maxaddr = 128; break;
	   case emAT93C56: _maxaddr = 256; break;
	   case emAT93C66: _maxaddr = 512; break;
  }
  _org_enabled = org_enabled;
  if (org_enabled) {_maxaddr = _maxaddr / 2;} //is Double Byte
  MaxAddr = _maxaddr - 1;
}

void AT93CXX::Begin()
{	
  pinMode(_cspin, OUTPUT);
  digitalWrite(_cspin, LOW);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); //2 MHz Clock Rate
  SPI.beginTransaction(AT93CXX_SPISettings);
  digitalWrite(_cspin, LOW);	
}

void AT93CXX::End()
{	
  SPI.end();
}

// Write enable must precede all programming modes
void AT93CXX::EWEN()
{
  digitalWrite(_cspin, HIGH);  
  //Address is 6 bit
  if (_maxaddr == 64) { 
	SPI.transfer16(0b100110000);    
  }
  //Address is 7 bit
  //SB(1) + OPCode(00) + Addr(11) + XXXXX -> 1+00+11+00000 
  if (_maxaddr == 128) { 
	SPI.transfer16(0b1001100000);    
  }
  //Address is 8 bit
  if (_maxaddr == 256) { 
	SPI.transfer16(0b10011000000);    
  }  
  //Address is 9 bit
  if (_maxaddr == 512) { 
	SPI.transfer16(0b100110000000);    
  }  
  digitalWrite(_cspin, LOW);
  delay(100);
}

// Disables all programming instructions
void AT93CXX::EWDS()
{
  digitalWrite(_cspin, HIGH);
  
  //Address is 6 bit
  if (_maxaddr == 64) { 
	SPI.transfer16(0b100000000);    
  }
  //Address is 7 bit
  //SB(1) + OPCode(00) + Addr(00) + XXXXX -> 1+00+00+00000 
  if (_maxaddr == 128) { 
	SPI.transfer16(0b1000000000);    
  }
  //Address is 8 bit
  if (_maxaddr == 256) { 
	SPI.transfer16(0b10000000000);    
  }  
  //Address is 9 bit
  if (_maxaddr == 512) { 
	SPI.transfer16(0b100000000000);    
  }   
  
  digitalWrite(_cspin, LOW);
  delay(100);
  return true;
}

// Erases all memory locations
void AT93CXX::ERAL()
{
  digitalWrite(_cspin, HIGH);

  //Address is 6 bit
  if (_maxaddr == 64) { 
	SPI.transfer16(0b100100000);    
  }
  //Address is 7 bit
  //SB(1) + OPCode(00) + Addr(10) + XXXXX -> 1+00+10+00000 
  if (_maxaddr == 128) { 
	SPI.transfer16(0b1001000000);    
  }
  //Address is 8 bit
  if (_maxaddr == 256) { 
	SPI.transfer16(0b10010000000);    
  }  
  //Address is 9 bit
  if (_maxaddr == 512) { 
	SPI.transfer16(0b100100000000);    
  }     
  
  digitalWrite(_cspin, LOW);
  delay(100);
}

// Erase memory location
bool AT93CXX::ERASE(int16_t loc)
{
  if(loc < _maxaddr)
  {
    digitalWrite(_cspin, HIGH);
	
	//Address is 6 bit
    //SB(1) + OPCode(11) + XXXXXX -> 1+11+000000 -> 1 + (Move Two Bit To Address) 11+000000
	if (_maxaddr == 64) { 
		SPI.transfer16(0x1C0|loc);    
	}
	
    //Address is 7 bit
    //SB(1) + OPCode(11) + XXXXXXX -> 1+11+0000000 
	if (_maxaddr == 128) { 
		SPI.transfer16(0x380|loc);      
	}
	
    //Address is 8 bit
    //SB(1) + OPCode(11) + XXXXXXXX -> 1+11+00000000 
	if (_maxaddr == 256) { 
		SPI.transfer16(0x700|loc); 
	}	
	
    //Address is 9 bit
    //SB(1) + OPCode(11) + XXXXXXXXX -> 1+11+000000000 
	if (_maxaddr == 512) { 
		SPI.transfer16(0xE00|loc); 
	}	
	
    digitalWrite(_cspin, LOW);
    delay(100);
    return true;
  }
  else
    return false;
}

// Reads data stored in memory, at specified address
word AT93CXX::READ(int16_t loc)
{
  if(loc < _maxaddr) //
  {
    digitalWrite(_cspin, HIGH);
	
	//Address is 6 bit
    //SB(1) + OPCode(10) + XXXXXX -> 1+10+000000 
	if (_maxaddr == 64) { 
		SPI.transfer16(0x180|loc);    
	}
	
    //Address is 7 bit
    //SB(1) + OPCode(10) + XXXXXXX -> 1+10+0000000 
	if (_maxaddr == 128) { 
		SPI.transfer16(0x300|loc);      
	}
	
    //Address is 8 bit
    //SB(1) + OPCode(10) + XXXXXXXX -> 1+10+00000000 
	if (_maxaddr == 256) { 
		SPI.transfer16(0x600|loc); 
	}	
	
    //Address is 9 bit
    //SB(1) + OPCode(10) + XXXXXXXXX -> 1+10+000000000 
	if (_maxaddr == 512) { 
		SPI.transfer16(0xC00|loc); 
	}		
	delay(5);
	if (_org_enabled)
	{
		word buf, buf1, buf2;
		buf1 = SPI.transfer16(0x00);
		buf2 = SPI.transfer16(0x00);
		digitalWrite(_cspin, LOW);
		buf1 = buf1 << 1; // 0xxxxxxx ->  xxxxxxx- ->  last bit is in the next read first bit
		buf2 = buf2 >> 15; // 1xxxxxxx ->  xxxxxxx1
		buf = buf1 | buf2; // Bitwise Or - Buf1 Or Buf2
		return buf;
	}
	else
	{	
		byte buf, buf1, buf2;
		buf1 = SPI.transfer(0x00);  // buf1 contains a FSB and the first 7 bits at the address
		buf2 = SPI.transfer(0x00); // buf12 contains the 8th bit and the LSB
		digitalWrite(_cspin, LOW);
		buf1 = buf1 << 1; // 0xxxxxxx ->  xxxxxxx- ->  last bit is in the next read first bit
		buf2 = buf2 >> 7; // 1xxxxxxx ->  xxxxxxx1
		buf = buf1 | buf2; // Bitwise Or - Buf1 Or Buf2
		return buf;
	}
  }
  return 0;
}

// Writes memory location
bool AT93CXX::WRITE(int16_t loc, word data)
{
  if(loc < _maxaddr) 
  {
    digitalWrite(_cspin, HIGH);
	
	//Address is 6 bit
    //SB(1) + OPCode(01) + XXXXXX -> 1+01+000000
	if (_maxaddr == 64) { 
		SPI.transfer16(0x140|loc);    
	}
	
    //Address is 7 bit
    //SB(1) + OPCode(01) + XXXXXXX -> 1+01+0000000 
	if (_maxaddr == 128) { 
		SPI.transfer16(0x280|loc);      
	}
	
    //Address is 8 bit
    //SB(1) + OPCode(01) + XXXXXXXX -> 1+01+00000000 
	if (_maxaddr == 256) { 
		SPI.transfer16(0x500|loc); 
	}	
	
    //Address is 9 bit
    //SB(1) + OPCode(01) + XXXXXXXXX -> 1+01+000000000 
	if (_maxaddr == 512) { 
		SPI.transfer16(0xA00|loc); 
	}			
	if (_org_enabled)
	{
		SPI.transfer16(data);
	}
	else
	{
		SPI.transfer(data);
	}
	digitalWrite(_cspin, LOW);
	delay(50);
    return true;
  }
  else
    return false;
}