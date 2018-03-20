#include "SSD1306_I2C.h"

unsigned char SSD1306_Address;

unsigned char LastCache[16][8];
unsigned char Cache[16][8];

unsigned char UseCache = 0;
unsigned char Cache_X = 0,Cache_Y = 0;

unsigned char Pos = 0;
unsigned char *bitmaparrayCache[MAX_bitmaparrayCache];

void SSD1306_I2C::init(unsigned char _Address)
{
  SSD1306_Address = _Address;
  sendCommand(0xAE);            //display off
  sendCommand(0xA6);            //Set Normal Display (default)
  sendCommand(0xAE);            //DISPLAYOFF
  sendCommand(0xD5);            //SETDISPLAYCLOCKDIV
  sendCommand(0x80);            // the suggested ratio 0x80
  sendCommand(0xA8);            //SSD1306_SETMULTIPLEX
  sendCommand(0x3F);
  sendCommand(0xD3);            //SETDISPLAYOFFSET
  sendCommand(0x0);             //no offset
  sendCommand(0x40|0x0);        //SETSTARTLINE
  sendCommand(0x8D);            //CHARGEPUMP
  sendCommand(0x14);
  sendCommand(0x20);            //MEMORYMODE
  sendCommand(0x00);            //0x0 act like ks0108
  sendCommand(0xA1);            //SEGREMAP   Mirror screen horizontally (A0)
  sendCommand(0xC8);            //COMSCANDEC Rotate screen vertically (C0)
  sendCommand(0xDA);            //0xDA
  sendCommand(0x12);            //COMSCANDEC
  sendCommand(0x81);            //SETCONTRAST
  sendCommand(0xCF);            //
  sendCommand(0xd9);            //SETPRECHARGE 
  sendCommand(0xF1); 
  sendCommand(0xDB);            //SETVCOMDETECT                
  sendCommand(0x40);
  sendCommand(0xA4);            //DISPLAYALLON_RESUME        
  sendCommand(0xA6);            //NORMALDISPLAY             
  clear();
  sendCommand(0x2E);            // stop scroll
  sendCommand(0x20);            //Set Memory Addressing Mode
  sendCommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
}

void SSD1306_I2C::setCacheMode(unsigned char chs)
{
	UseCache = chs;
}

size_t SSD1306_I2C::write(uint8_t ch)
{
	if(UseCache)CachePutChar(ch);
	else putChar(ch);
	return ch;
}

void SSD1306_I2C::sendCommand(unsigned char command)
{
  Wire.beginTransmission(SSD1306_Address);    // begin I2C communication
  Wire.write(SSD1306_Command_Mode);           // Set OLED Command mode
  Wire.write(command);
  Wire.endTransmission();                       // End I2C communication
}

void SSD1306_I2C::setBrightness(unsigned char Brightness)
{
   sendCommand(SSD1306_Set_Brightness_Cmd);
   sendCommand(Brightness);
}

void SSD1306_I2C::setHorizontalMode()
{
    addressingMode = HORIZONTAL_MODE;
    sendCommand(0x20);                      //set addressing mode
    sendCommand(0x00);                      //set horizontal addressing mode
}

void SSD1306_I2C::setPageMode()
{
    addressingMode = PAGE_MODE;
    sendCommand(0x20);                      //set addressing mode
    sendCommand(0x02);                      //set page addressing mode
}

void SSD1306_I2C::setCursor(unsigned char Column, unsigned char Row)
{
	if(UseCache)
	{
		if(Column>15)Column=15;
		if(Row>7)Row=7;
		Cache_X = Column;
		Cache_Y = Row;
	}
	else
	{
    sendCommand(0xB0 + Row);                    //set page address
    sendCommand(0x00 + (8*Column & 0x0F));      //set column lower address
    sendCommand(0x10 + ((8*Column>>4)&0x0F));   //set column higher address
	}
}

void SSD1306_I2C::clear()
{
	if(UseCache)
	{
		unsigned char i,j;
		for(j=0;j<8;j++)
		{    
			for(i=0;i<16;i++)  //clear all columns
			{
				Cache[i][j]=' ';    
			}
		}
	}
	else
	{
		sendCommand(SSD1306_Display_Off_Cmd);     //display off
		setCursor(0,0);
		for(int i=0;i<128;i++)  //clear all columns
		{
			putChar(' ');    
		} 
		sendCommand(SSD1306_Display_On_Cmd);     //display on    
		setCursor(0,0);
	}		
}

void SSD1306_I2C::sendData(unsigned char Data)
{
     Wire.beginTransmission(SSD1306_Address); // begin I2C transmission
     Wire.write(SSD1306_Data_Mode);            // data mode
     Wire.write(Data);
     Wire.endTransmission();                    // stop I2C transmission
}

void SSD1306_I2C::putChar(unsigned char C)
{
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
    	C=' '; //Space
    }  
    for(unsigned char i=0;i<8;i++)
    {
       //read bytes from code memory
       sendData(BasicFont[C-32][i]); //font array starts at 0, ASCII starts at 32. Hence the translation
    }
}

void SSD1306_I2C::CachePutChar(unsigned char C)
{
	if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
  {
  	C=' '; //Space
  }
  Cache[Cache_X][Cache_Y] = C;
  Cache_X++;
  if(Cache_X>15)
  {
  	Cache_X=0;
  	Cache_Y++;
  }
  if(Cache_Y>7)
  {
  	Cache_X=0;
  	Cache_Y=0;
  }
}

void SSD1306_I2C::putCache()
{
	unsigned char i,j;
	UseCache = 0;
	setCursor(0,0);
	for(j=0;j<8;j++)
  {    
    for(i=0;i<16;i++)  //clear all columns
		{
		  if(LastCache[i][j] != Cache[i][j])
		  {
				setCursor(i,j);
				if(Cache[i][j]>=130)
				{
					drawBitmap(bitmaparrayCache[Cache[i][j]-130]);
				}
		  	else putChar(Cache[i][j]);
		  }
		  LastCache[i][j] = Cache[i][j];
		}
  }
	UseCache = 1;
}

void SSD1306_I2C::drawBitmap(unsigned char *bitmaparray)
{
	if(UseCache)
	{
		byte Haved = 0, Found_Pos = 0;
		for(byte i = 0; i < Pos + 1; i++)
		{
			if(bitmaparray == bitmaparrayCache[i])
			{
				Haved = 1;
				Found_Pos = i;
				break;
			}
		}
		
		if(Haved)
		{
			Cache[Cache_X][Cache_Y] = 130 + Found_Pos;
		}
		else
		{
			bitmaparrayCache[Pos] = bitmaparray;
			Cache[Cache_X][Cache_Y] = 130 + Pos;
			if(Pos < MAX_bitmaparrayCache-1)Pos++;
		}
		
		Cache_X++;
		if(Cache_X>15)
		{
			Cache_X=0;
			Cache_Y++;
		}
		if(Cache_Y>7)
		{
			Cache_X=0;
			Cache_Y=0;
		}
	}
	else
	{
		char localAddressMode = addressingMode;
		if(addressingMode != HORIZONTAL_MODE)
		{
				//Bitmap is drawn in horizontal mode     
				setHorizontalMode();
		}

		for(int i=0;i<8;i++)
		{
				sendData(bitmaparray[i]);
		}

		if(localAddressMode == PAGE_MODE)
		{
			 //If pageMode was used earlier, restore it.
			 setPageMode(); 
		}
	}
}

void SSD1306_I2C::drawFullScreen(const unsigned char *PictureArray,int bytes)
{
	char localAddressMode = addressingMode;
	unsigned char UseCache_BK = UseCache;
	
	UseCache = 0;
	setCursor(0,0);
	if(addressingMode != HORIZONTAL_MODE)
	{
			//Bitmap is drawn in horizontal mode     
			setHorizontalMode();
	}

	for(int i=0;i<bytes;i++)
	{
		sendData(PictureArray[i]);
	}

	if(localAddressMode == PAGE_MODE)
	{
		 //If pageMode was used earlier, restore it.
		 setPageMode(); 
	}
	UseCache = UseCache_BK;
}

void SSD1306_I2C::setHorizontalScrollProperties(bool direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed)
{
/*
Use the following defines for 'direction' :

 Scroll_Left            
 Scroll_Right            

Use the following defines for 'scrollSpeed' :

 Scroll_2Frames        
 Scroll_3Frames
 Scroll_4Frames
 Scroll_5Frames    
 Scroll_25Frames
 Scroll_64Frames
 Scroll_128Frames
 Scroll_256Frames

*/

   if(Scroll_Right == direction)
   {
        //Scroll Right
        sendCommand(0x26);
   }
   else
   {
        //Scroll Left  
        sendCommand(0x27);

   }
    sendCommand(0x00);
    sendCommand(startPage);
    sendCommand(scrollSpeed);
    sendCommand(endPage);
    sendCommand(0x00);
    sendCommand(0xFF);
}

void SSD1306_I2C::activateScroll()
{
    sendCommand(SSD1306_Activate_Scroll_Cmd);
}

void SSD1306_I2C::deactivateScroll()
{
    sendCommand(SSD1306_Dectivate_Scroll_Cmd);
}

void SSD1306_I2C::setNormalDisplay()
{
    sendCommand(SSD1306_Normal_Display_Cmd);
}

void SSD1306_I2C::setInverseDisplay()
{
    sendCommand(SSD1306_Inverse_Display_Cmd);
}
