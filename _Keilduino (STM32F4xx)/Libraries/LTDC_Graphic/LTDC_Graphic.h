#ifndef __LTDC_GRAPHIC_H
#define __LTDC_GRAPHIC_H

#include "Arduino.h"
#include "Print.h"
#include "stm32f429i_discovery_lcd.h"

class LTDC_Graphic : public Print
{
public:
	LTDC_Graphic(uint16_t HIGHT, uint16_t WIDTH);
	uint16_t height();
	uint16_t width();
	void     deInit(void);   
	void     init(void);
	void     layerInit(void);
	void     chipSelect(FunctionalState NewState);
	void     setLayer(uint32_t Layerx);
	void     setColors(uint16_t _TextColor, uint16_t _BackColor); 
	void     getColors(uint16_t *_TextColor, uint16_t *_BackColor);
	void     setTextColor(uint16_t Color);
	void     setBackColor(uint16_t Color);
	void     setTransparency(uint8_t transparency);
	void     clearLine(uint16_t Line);
	void     clear(uint16_t Color);
	void		 fillScreen(uint16_t Color);
	uint32_t setCursor(uint16_t Xpos, uint16_t Ypos);
	void     setColorKeying(uint32_t RGBValue);
	void     reSetColorKeying(void);
	void 		 drawPixel(int16_t x, int16_t y);
	void     drawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
	void     displayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
	void     setFont(sFONT *fonts);
	void		 setTextSize(uint8_t Size);
	sFONT *  getFont(void);
	void     displayStringLine(uint16_t Line, uint8_t *ptr);
	void     setDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
	void     windowModeDisable(void);
	void     drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void     drawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
	void     drawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
	void     drawEllipse(int Xpos, int Ypos, int Radius, int Radius2);
	void     drawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2);
	void     drawMonoPict(const uint32_t *Pict);
	void     writeBMP(uint32_t BmpAddress);
	void     drawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void     drawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
	void     drawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
	void     polyLine(pPoint Points, uint16_t PointCount);
	void     polyLineRelative(pPoint Points, uint16_t PointCount);
	void     closedPolyLine(pPoint Points, uint16_t PointCount);
	void     closedPolyLineRelative(pPoint Points, uint16_t PointCount);
	void     fillPolyLine(pPoint Points, uint16_t PointCount);
	void     drawTriangle(pPoint Points, uint16_t PointCount);
	void		 drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
	void     drawFillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
	void     writeCommand(uint8_t LCD_Reg);
	void     writeData(uint8_t value);
	void     powerOn(void);
	void     displayOn(void);
	void     displayOff(void);
	void     ctrlLinesConfig(void);
	void     ctrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal);
	void     SPIConfig(void);
	
	virtual size_t write(uint8_t n);
	using Print::write;
	
private:
	uint32_t currentLayer;
	uint16_t LCD_HEIGHT;
	uint16_t LCD_WIDTH;
	uint16_t X_Pos,Y_Pos;
	
	void 		 clockInit();
	void		 layer_Init();
	void		 putPixel(int16_t x, int16_t y);
	void		 polyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
	void		 AF_GPIOConfig(void);
};

#endif
