#include "LTDC_Graphic.h"

#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y)) 

#define HBP  29
#define VBP  3
#define HSW  9
#define VSW  1
#define HFP  10
#define VFP  4

LTDC_Graphic::LTDC_Graphic(uint16_t HIGHT, uint16_t WIDTH)
{
	LCD_HEIGHT = HIGHT;
	LCD_WIDTH = WIDTH;
	
	X_Pos = 0;
	Y_Pos = 0;
}

uint16_t LTDC_Graphic::height()
{
	return LCD_HEIGHT;
}
	
uint16_t LTDC_Graphic::width()
{
	return LCD_WIDTH;
}

void LTDC_Graphic::clockInit()
{
    LTDC_InitTypeDef LTDC_InitStruct;
    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
//    LTDC_InitStruct.LTDC_HorizontalSync       = HSW-1;
//    LTDC_InitStruct.LTDC_VerticalSync         = VSW-1;                
//    LTDC_InitStruct.LTDC_AccumulatedHBP       = HSW+HBP-1;            
//    LTDC_InitStruct.LTDC_AccumulatedVBP       = VSW+VBP-1;            
//    LTDC_InitStruct.LTDC_AccumulatedActiveW   = HSW+HBP+WIDTH-1;     
//    LTDC_InitStruct.LTDC_AccumulatedActiveH   = VSW+VBP+HEIGHT-1;    
//    LTDC_InitStruct.LTDC_TotalWidth           = HSW+HBP+WIDTH+HFP-1;  
//    LTDC_InitStruct.LTDC_TotalHeigh           = VSW+VBP+HEIGHT+VFP-1;
//    LTDC_InitStruct.LTDC_BackgroundRedValue   = 0;
//    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
//    LTDC_InitStruct.LTDC_BackgroundBlueValue  = 0;
		/* Timing configuration */  
  /* Configure horizontal synchronization width */     
  LTDC_InitStruct.LTDC_HorizontalSync = 9;
  /* Configure vertical synchronization height */
  LTDC_InitStruct.LTDC_VerticalSync = 1;
  /* Configure accumulated horizontal back porch */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
  /* Configure accumulated vertical back porch */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
  /* Configure accumulated active width */  
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
  /* Configure accumulated active height */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
  /* Configure total width */
  LTDC_InitStruct.LTDC_TotalWidth = 279; 
  /* Configure total height */
  LTDC_InitStruct.LTDC_TotalHeigh = 327;
  
  LTDC_Init(&LTDC_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC,ENABLE);
	RCC_PLLSAIConfig(256, 2, 4);
	RCC_PLLSAICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY)==RESET);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
	LTDC_Init(&LTDC_InitStruct);
	LTDC_Cmd(ENABLE);
}

void LTDC_Graphic::layer_Init()
{
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
	LTDC_Layer_InitStruct.LTDC_HorizontalStart  = HBP+1;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop   = LCD_WIDTH+HBP-1;
	LTDC_Layer_InitStruct.LTDC_VerticalStart    = VBP+1;
	LTDC_Layer_InitStruct.LTDC_VerticalStop     = LCD_HEIGHT+VBP;
	LTDC_Layer_InitStruct.LTDC_PixelFormat      = LTDC_Pixelformat_RGB565;
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha    = 255;
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress   = LCD_FRAME_BUFFER;
	LTDC_Layer_InitStruct.LTDC_CFBLineLength    = (LCD_WIDTH * 2)+3;
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber    = LCD_HEIGHT;
	LTDC_Layer_InitStruct.LTDC_CFBPitch         = LCD_WIDTH * 2;
	LTDC_Layer_InitStruct.LTDC_DefaultColorRed  = 0;
	LTDC_Layer_InitStruct.LTDC_DefaultColorGreen= 0;
	LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
	LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha= 0;

	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
	LTDC_ReloadConfig(LTDC_IMReload);
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	LTDC_ReloadConfig(LTDC_IMReload);
}

void LTDC_Graphic::deInit(void)
{ 
  LCD_DeInit();
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::init(void)
{ 
  LCD_Init();
	clockInit();
	//LCD_LayerInit();
	layer_Init();
}  

/**
  * @brief  Initializes the LCD Layers.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::layerInit(void)
{
  LCD_LayerInit();
}

/**
  * @brief  Controls LCD Chip Select (CS) pin
  * @param  NewState CS pin state
  * @retval None
  */
void LTDC_Graphic::chipSelect(FunctionalState NewState)
{
  LCD_ChipSelect(NewState);
}

/**
  * @brief  Sets the LCD Layer.
  * @param  Layerx: specifies the Layer foreground or background.
  * @retval None
  */
void LTDC_Graphic::setLayer(uint32_t Layerx)
{
  LCD_SetLayer(Layerx);
}  

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  TextColor: specifies the Text Color.
  * @param  BackColor: specifies the Background Color.
  * @retval None
  */
void LTDC_Graphic::setColors(uint16_t TextColor, uint16_t BackColor)
{
	LCD_SetColors(TextColor, BackColor);
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  TextColor: pointer to the variable that will contain the Text 
            Color.
  * @param  BackColor: pointer to the variable that will contain the Background 
            Color.
  * @retval None
  */
void LTDC_Graphic::getColors(uint16_t *TextColor, uint16_t *BackColor)
{
  LCD_GetColors(TextColor,BackColor);
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LTDC_Graphic::setTextColor(uint16_t Color)
{
	LCD_SetTextColor(Color);
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LTDC_Graphic::setBackColor(uint16_t Color)
{
	LCD_SetBackColor(Color);
}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LTDC_Graphic::setFont(sFONT *fonts)
{
	LCD_SetFont(fonts);
}

void LTDC_Graphic::setTextSize(uint8_t Size)
{
	if(Size > 3)Size=3;
	switch(Size)
	{
		case 0:setFont(&Font8x8);break;
		case 1:setFont(&Font8x12);break;
		case 2:setFont(&Font12x12);break;
		case 3:setFont(&Font16x24);break;
	}
}

/**
  * @brief  Configure the transparency.
  * @param  transparency: specifies the transparency, 
  *         This parameter must range from 0x00 to 0xFF.
  * @retval None
  */
void LTDC_Graphic::setTransparency(uint8_t transparency)
{
  LCD_SetTransparency(transparency);
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LTDC_Graphic::getFont(void)
{
  return LCD_GetFont();
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg LCD_LINE_x: where x can be: 0..13 if currentfonts is Font16x24
  *                                      0..26 if currentfonts is Font12x12 or Font8x12
  *                                      0..39 if currentfonts is Font8x8
  * @retval None
  */
void LTDC_Graphic::clearLine(uint16_t Line)
{
  LCD_ClearLine(Line);
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LTDC_Graphic::clear(uint16_t Color)
{
	X_Pos = 0;
	Y_Pos = 0;
  LCD_Clear(Color);
}

void LTDC_Graphic::fillScreen(uint16_t Color)
{
	X_Pos = 0;
	Y_Pos = 0;
  LCD_Clear(Color);
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval Display Address
  */
uint32_t LTDC_Graphic::setCursor(uint16_t Xpos, uint16_t Ypos)
{
	X_Pos = Xpos;
	Y_Pos = Ypos;
  return LCD_SetCursor(Xpos, Ypos);
}

/**
  * @brief  Config and Sets the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LTDC_Graphic::setColorKeying(uint32_t RGBValue)
{  
  LCD_SetColorKeying(RGBValue);
}

/**
  * @brief  Disable the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LTDC_Graphic::reSetColorKeying(void)
{
  LCD_ReSetColorKeying();
}

void LTDC_Graphic::drawPixel(int16_t x, int16_t y)
{
	if(x < 0 || x > 239 || y < 0 || y > 319)
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}

size_t LTDC_Graphic::write(uint8_t n)
{
	if((n != '\n') && (X_Pos < LCD_WIDTH) && ((LCD_WIDTH - X_Pos) >= LCD_GetFont()->Width))
	{
		displayChar(Y_Pos, X_Pos, n);
		if(n != '\r')
		{		
			X_Pos+=LCD_GetFont()->Width;
		}
	}
	else
	{
		if((Y_Pos < LCD_HEIGHT) && ((LCD_HEIGHT - Y_Pos) >= LCD_GetFont()->Height * 2))
		{
			X_Pos=0;
			Y_Pos += LCD_GetFont()->Height;
			
			LCD_SetCursor(X_Pos,Y_Pos);
			displayChar(Y_Pos, X_Pos, n);
			if(n != '\r' && n != '\n')
			{			
				X_Pos+=LCD_GetFont()->Width;
			}
		}
	}
	return n;
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LTDC_Graphic::drawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  LCD_DrawChar(Xpos, Ypos, c);
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..29
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LTDC_Graphic::displayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
	if(Ascii != '\r' && Ascii != '\n')LCD_DisplayChar(Line, Column,Ascii);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LTDC_Graphic::displayStringLine(uint16_t Line, uint8_t *ptr)
{  
  LCD_DisplayStringLine(Line,ptr);
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position from 0 to 240.
  * @param  Ypos: specifies the Y bottom left position from 0 to 320.
  * @param  Height: display window height, can be a value from 0 to 320.
  * @param  Width: display window width, can be a value from 0 to 240.
  * @retval None
  */
void LTDC_Graphic::setDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
	LCD_SetDisplayWindow(Xpos, Ypos, Height, Width);
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::windowModeDisable(void)
{
  LCD_SetDisplayWindow(0, 0, LCD_PIXEL_HEIGHT, LCD_WIDTH); 
}

/**
  * @brief  Displays a line.
  * @param Xpos: specifies the X position, can be a value from 0 to 240.
  * @param Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: LCD_DIR_HORIZONTAL or LCD_DIR_VERTICAL.
  * @retval None
  */
void LTDC_Graphic::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawUniLine(x1,y1,x2,y2);
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Height: display rectangle height, can be a value from 0 to 320.
  * @param  Width: display rectangle width, can be a value from 0 to 240.
  * @retval None
  */
void LTDC_Graphic::drawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	if(Height && Width)LCD_DrawRect(Xpos, Ypos, Height, Width);
}

/**
  * @brief  Draw a circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: radius of the circle.
  * @retval None
  */
void LTDC_Graphic::drawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	if(Xpos && Ypos)LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Draw a full ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: minor radius of ellipse.
  * @param  Radius2: major radius of ellipse.  
  * @retval None
  */
void LTDC_Graphic::drawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  LCD_DrawFullEllipse(Xpos, Ypos, Radius, Radius2);
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: specifies Radius.
  * @param  Radius2: specifies Radius2.
  * @retval None
  */
void LTDC_Graphic::drawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  LCD_DrawEllipse(Xpos, Ypos, Radius, Radius2);
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LTDC_Graphic::drawMonoPict(const uint32_t *Pict)
{
  LCD_DrawMonoPict(Pict);
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LTDC_Graphic::writeBMP(uint32_t BmpAddress)
{
  LCD_WriteBMP(BmpAddress);
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LTDC_Graphic::drawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  LCD_DrawFullRect(Xpos, Ypos, Width, Height);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius
  * @retval None
  */
void LTDC_Graphic::drawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  LCD_DrawFullCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LTDC_Graphic::drawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  LCD_DrawUniLine(x1, y1, x2, y2);
}

/**
  * @brief  Displays an triangle.
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LTDC_Graphic::drawTriangle(pPoint Points, uint16_t PointCount)
{
  LCD_Triangle(Points, PointCount);
}

void LTDC_Graphic::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
	LCD_DrawUniLine(x1,y1,x2,y2);
	LCD_DrawUniLine(x2,y2,x3,y3);
	LCD_DrawUniLine(x1,y1,x3,y3);
}

/**
  * @brief  Fill an triangle (between 3 points).
  * @param  x1..3: x position of triangle point 1..3.
  * @param  y1..3: y position of triangle point 1..3.
  * @retval None
  */
void LTDC_Graphic::drawFillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{ 
  LCD_FillTriangle(x1, x2, x3, y1, y2, y3);
}
/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LTDC_Graphic::polyLine(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
void LTDC_Graphic::polyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
//	LCD_PolyLineRelativeClosed();
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LTDC_Graphic::closedPolyLine(pPoint Points, uint16_t PointCount)
{
  LCD_ClosedPolyLine(Points, PointCount);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LTDC_Graphic::polyLineRelative(pPoint Points, uint16_t PointCount)
{
// LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LTDC_Graphic::closedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
//  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}

/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LTDC_Graphic::fillPolyLine(pPoint Points, uint16_t PointCount)
{
	LCD_FillPolyLine(Points, PointCount);
}

/**
  * @brief  Writes command to select the LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void LTDC_Graphic::writeCommand(uint8_t LCD_Reg)
{
  LCD_WriteCommand(LCD_Reg);
}

/**
  * @brief  Writes data to select the LCD register.
  *         This function must be used after LCD_WriteCommand() function
  * @param  value: data to write to the selected register.
  * @retval None
  */
void LTDC_Graphic::writeData(uint8_t value)
{
  LCD_WriteData(value);
}

/**
  * @brief  Configure the LCD controller (Power On sequence as described in ILI9341 Datasheet)
  * @param  None
  * @retval None
  */
void LTDC_Graphic::powerOn(void)
{
  LCD_PowerOn();
 }

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::displayOn(void)
{
  LCD_DisplayOn();
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::displayOff(void)
{
   LCD_DisplayOff();
}

/**
  * @brief  Configures LCD control lines in Output Push-Pull mode.
  * @note   The LCD_NCS line can be configured in Open Drain mode  
  *         when VDDIO is lower than required LCD supply.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::ctrlLinesConfig(void)
{
  LCD_CtrlLinesConfig();
}

/**
  * @brief  Sets or reset LCD control lines.
  * @param  GPIOx: where x can be B or D to select the GPIO peripheral.
  * @param  CtrlPins: the Control line.
  *   This parameter can be:
  *     @arg LCD_NCS_PIN: Chip Select pin
  *     @arg LCD_NWR_PIN: Read/Write Selection pin
  *     @arg LCD_RS_PIN: Register/RAM Selection pin
  * @param  BitVal: specifies the value to be written to the selected bit.
  *   This parameter can be:
  *     @arg Bit_RESET: to clear the port pin
  *     @arg Bit_SET: to set the port pin
  * @retval None
  */
void LTDC_Graphic::ctrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal)
{
  LCD_CtrlLinesWrite(GPIOx, CtrlPins, BitVal);
}

/**
  * @brief  Configures the LCD_SPI interface.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::SPIConfig(void)
{
  LCD_SPIConfig();
}

/**
  * @brief  GPIO config for LTDC.
  * @param  None
  * @retval None
  */
void LTDC_Graphic::AF_GPIOConfig(void)
{
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @retval None
  */
void LTDC_Graphic::putPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > 239 || y < 0 || y > 319)
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}
