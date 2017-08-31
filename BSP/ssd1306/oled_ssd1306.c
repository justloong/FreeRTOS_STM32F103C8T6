/******************************************************************************
* File Name     : oled_I2C.c
* Version       : 1.0
* Device(s)     : 
* Tool-Chain    : 
* Description   : 
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.03.2014 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> 
******************************************************************************/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "iwdg.h"
#include "i2c.h"
#include "ssd1306/oled_ssd1306.h"
#include "ssd1306/oled_const.h"
#include "string.h"
#include "math.h"




/******************************************************************************
Imported global variables (from other files)
******************************************************************************/
#ifdef USE_I2C_PERIPHERAL
//uint8_t tempDataBuffer[1+(SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8)];

#define SSD1306_CMD(data)   ssd1306SendByte_I2C(data, SSD1306_WRITE_CMD);

#define SSD1306_DATA(data)  ssd1306SendByte_I2C(data, SSD1306_WRITE_DATA);
#else
#define SSD1306_CMD(data)   do { HAL_GPIO_WritePin( GPIOB, OLED_DC_Pin, GPIO_PIN_RESET); \
                                 ssd1306SendByte( data ); \
                               } while (0);

#define SSD1306_DATA(data)  do { HAL_GPIO_WritePin( GPIOB, OLED_DC_Pin, GPIO_PIN_SET); \
                                 ssd1306SendByte( data ); \
                               } while (0); 
#endif


#define swap(a, b)       { int16_t t = a; a = b; b = t; }

#if (USE_IO_SIMULATE_I2C != 0)
#define SCL_HIGH HAL_GPIO_WritePin(GPIOB,OLED_SCL_Pin,GPIO_PIN_SET)
#define SCL_LOW  HAL_GPIO_WritePin(GPIOB,OLED_SCL_Pin,GPIO_PIN_RESET)
#define SDA_HIGH HAL_GPIO_WritePin(GPIOB,OLED_SDA_Pin,GPIO_PIN_SET)
#define SDA_LOW  HAL_GPIO_WritePin(GPIOB,OLED_SDA_Pin,GPIO_PIN_RESET)

#define RST_HIGH HAL_GPIO_WritePin(GPIOB,OLED_RST_Pin,GPIO_PIN_SET)
#define RST_LOW  HAL_GPIO_WritePin(GPIOB,OLED_RST_Pin,GPIO_PIN_RESET)
#endif

/***********************Picture Code**************************/
//OLED的显存
static uint8_t buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];

#ifdef MULTILAYER
static uint8_t buffer_ol[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
#endif


static uint8_t   _width    = SSD1306_LCDWIDTH;
static uint8_t   _height   = SSD1306_LCDHEIGHT;



/**********************************************************************************************************************
基本功能函数
根据不同接口，基本函数不同
**********************************************************************************************************************/
void ssd1306SendByte(uint8_t data)
{
    #ifdef SSD1306_SPI_INTERFACE
	uint8_t i;			  

	for(i=0;i<8;i++)
	{			  
		SCL_LOW;
		if(data&0x80)
		   SDA_HIGH;
		else 
		   SDA_LOW;
		SCL_HIGH;
		data<<=1;   
	}		
    #endif
}

void ssd1306SendByte_I2C(uint8_t data, uint8_t cmdOrData)
{
    HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDRESS, cmdOrData, I2C_MEMADD_SIZE_8BIT, &data, 1, 10000);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**************************************************************************/
/**
 *  \brief Initialises the SSD1306 LCD display
 *  
 *  \param [in] vccstate Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void  ssd1306Init(uint8_t vccstate)
{

    #ifdef SSD1306_SPI_INTERFACE
    SSD1306MSDELAY(100);
    RST_LOW;
    SSD1306MSDELAY(100);
    RST_HIGH; 
    #endif

    SSD1306MSDELAY  (100);
    // Initialisation sequence
    SSD1306_CMD(SSD1306_DISPLAYOFF);                    // 0xAE
    SSD1306_CMD(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    SSD1306_CMD(0x80);                                  // the suggested ratio 0x80
    SSD1306_CMD(SSD1306_SETMULTIPLEX);                  // 0xA8
    SSD1306_CMD(0x3F);
    SSD1306_CMD(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    SSD1306_CMD(0x0);                                   // no offset
    SSD1306_CMD(SSD1306_SETSTARTLINE | 0x0);            // line #0
    SSD1306_CMD(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
    { SSD1306_CMD(0x10); }
    else 
    { SSD1306_CMD(0x14); }
    SSD1306_CMD(SSD1306_MEMORYMODE);                    // 0x20
    SSD1306_CMD(0x02);//(0x00);                                  // 0x0 act like ks0108
    SSD1306_CMD(SSD1306_SEGREMAP | 0x1);
    SSD1306_CMD(SSD1306_COMSCANDEC);
    SSD1306_CMD(SSD1306_SETCOMPINS);                    // 0xDA
    SSD1306_CMD(0x12);
    SSD1306_CMD(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) 
    { SSD1306_CMD(0x9F); }
    else 
    { SSD1306_CMD(0xEF); }
    SSD1306_CMD(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
    { SSD1306_CMD(0x22); }
    else 
    { SSD1306_CMD(0xF1); }
    SSD1306_CMD(SSD1306_SETVCOMDETECT);                 // 0xDB
    SSD1306_CMD(0x40);
    SSD1306_CMD(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    SSD1306_CMD(SSD1306_NORMALDISPLAY);                 // 0xA6
    SSD1306MSDELAY  (100);
    ssd1306TurnOn();
}

/**
 *  \brief Renders the contents of the pixel buffer on the LCD
 *  
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306Refresh(void) 
{
    
    #ifdef SSD1306_SPI_INTERFACE
    uint16_t i;
    uint16_t j;
    #endif

    uint8_t freshTimes = 0;

    while(freshTimes<8)
    {   

    SSD1306_CMD(SSD1306_SETLOWCOLUMN  | 0x00);  // low col = 0
    SSD1306_CMD(SSD1306_SETHIGHCOLUMN | 0x00);  // hi col = 0
    //SSD1306_CMD(SSD1306_SETSTARTLINE  | (freshTimes*8));  // line #0
    
    SSD1306_CMD(0XB0  | freshTimes);  // line #0

#ifdef MULTILAYER
    ssd1306MixFrameBuffer();

    #ifdef SSD1306_I2C_INTERFACE
        HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDRESS, SSD1306_WRITE_DATA, I2C_MEMADD_SIZE_8BIT, 
        (buffer_ol+(freshTimes*128)), 128, 10000);
        //HAL_I2C_Mem_Write_IT(&hi2c1, SSD1306_ADDRESS, SSD1306_WRITE_DATA, I2C_MEMADD_SIZE_8BIT, buffer_ol, 1024);
    #else
        j = SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8;
        for(i=0;i<j;i++)
        {
            ssd1306SendByte(buffer_ol[i]);
        }
    #endif
#else
    #ifdef SSD1306_I2C_INTERFACE
        HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDRESS, SSD1306_WRITE_DATA, I2C_MEMADD_SIZE_8BIT, buffer, sizeof(buffer), 10000);
    #else
        j = SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8;
        for(i=0;i<j;i++)
        {
            ssd1306SendByte(buffer[i]);
        }
    #endif
#endif

        freshTimes++;
        }
}

/**
 *  \brief Enable the OLED panel
 *  
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306TurnOn(void)
{
    SSD1306_CMD(SSD1306_DISPLAYON);
}
/**
 *  \brief Disable the OLED panel
 *  
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306TurnOff(void)
{
    SSD1306_CMD(SSD1306_DISPLAYOFF);
}

/**************************************************************************/
/*! 
    @brief Draws a single pixel in image buffer
    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)
*/
/**************************************************************************/
void   ssd1306DrawPixel(int16_t x, int16_t y, uint16_t color, uint16_t layer) 
{
  if ((x >= SSD1306_LCDWIDTH) || (x < 0) || (y >= SSD1306_LCDHEIGHT) || (y < 0))
  return;

  if (layer & LAYER0)
  switch (color) 
  {
  case WHITE:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
  case BLACK:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break; 
  case INVERSE: buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break; 
  }
#ifdef MULTILAYER  
  if (layer & LAYER1)
  switch (color) 
  {
  case WHITE:   buffer_ol[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
  case BLACK:   buffer_ol[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break; 
  case INVERSE: buffer_ol[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break; 
  }
#endif
//	ssd1306Refresh();
}

/**************************************************************************/
/*! 
    @brief Clears a single pixel in image buffer
    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)
*/
/**************************************************************************/
//void ssd1306ClearPixel(int16_t x, int16_t y) 
//{
//  if ((x >= SSD1306_LCDWIDTH) || (x < 0) || (y >= SSD1306_LCDHEIGHT) || (y < 0))
//    return;

//  buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << y%8); 
//}

/**************************************************************************/
/*! 
    @brief Gets the value (1 or 0) of the specified pixel from the buffer
    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)
    @return     1 if the pixel is enabled, 0 if disabled
*/
/**************************************************************************/
uint8_t ssd1306GetPixel(int16_t x, int16_t y)
{
  if ((x >= SSD1306_LCDWIDTH) || (x < 0) || (y >= SSD1306_LCDHEIGHT) || (y < 0)) return 0;

  return buffer[x+ (y/8)*SSD1306_LCDWIDTH] & (1 << y%8) ? 1 : 0;
}

/**
 *  \brief Clears the screen
 *  
 *  \param [in] layer Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306ClearScreen(uint16_t layer) 
{
  if (layer & 0x1)
  memset(buffer, 0x00, 1024);
#ifdef MULTILAYER  
  if (layer & 0x2)
  memset(buffer_ol, 0x00, 1024);
#endif
}

/**
 *  \brief Brief
 *  
 *  \param [in] x0 Parameter_Description
 *  \param [in] y0 Parameter_Description
 *  \param [in] x1 Parameter_Description
 *  \param [in] y1 Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint16_t layer) {
  int16_t steep = (abs(y1 - y0) > abs(x1 - x0));
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx            = x1 - x0;
  dy            = abs(y1 - y0);

  int16_t err   = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep       = 1;
  } else {
    ystep       = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      ssd1306DrawPixel(y0, x0, color, layer);
    } else {
      ssd1306DrawPixel(x0, y0, color, layer);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
/**
 *  \brief Brief
 *  
 *  \param [in] SSD1306_point Parameter_Description
 *  \param [in] count Parameter_Description
 *  \param [in] x Parameter_Description
 *  \param [in] y Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 *  
 *  \code
 *  \endcode
 */
void ssd1306_DrawPolygon(SSD1306_polyTypeDef * SSD1306_poly, int16_t x, int16_t y, uint16_t color, uint16_t layer)
{
	int16_t i;
	
	if (SSD1306_poly->SSD1306_points_number < 2) return;
	for(i = 0; i < (SSD1306_poly->SSD1306_points_number - 1); i++)
	{
		ssd1306DrawLine(round(SSD1306_poly->SSD1306_points_pointer[i].x + x), round(SSD1306_poly->SSD1306_points_pointer[i].y + y), round(SSD1306_poly->SSD1306_points_pointer[i+1].x + x), round(SSD1306_poly->SSD1306_points_pointer[i+1].y + y), color, layer);
	}
	ssd1306DrawLine(round(SSD1306_poly->SSD1306_points_pointer[i].x + x), round(SSD1306_poly->SSD1306_points_pointer[i].y + y), round(SSD1306_poly->SSD1306_points_pointer[0].x + x), round(SSD1306_poly->SSD1306_points_pointer[0].y + y), color, layer);
}

/**
 *  \brief Fill poly http://alienryderflex.com/polygon_fill/
 *  
 *  \param [in] SSD1306_point Parameter_Description
 *  \param [in] count Parameter_Description
 *  \param [in] x Parameter_Description
 *  \param [in] y Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 *  
 *  \code
 *  \endcode
 */
void ssd1306_FillPolygon(SSD1306_polyTypeDef * SSD1306_poly, double x, double y, uint16_t color, uint16_t layer)
{
	int     nodes, nodeX[MAX_POLY_CORNERS], i, j, swap ;
  double  polyX[MAX_POLY_CORNERS], polyY[MAX_POLY_CORNERS], pixelX, pixelY;
	
	// draw shape
	ssd1306_DrawPolygon(SSD1306_poly, x, y, color, layer);
	// copy poly 
	i = SSD1306_poly->SSD1306_points_number - 1;
	while (i >= 0)
	{
		polyX[i] = round(SSD1306_poly->SSD1306_points_pointer[i].x + x);
		polyY[i] = round(SSD1306_poly->SSD1306_points_pointer[i].y + y);
		i--;
	}
	//  Loop through the rows of the image.
	for (pixelY = 0; pixelY < SSD1306_LCDHEIGHT; pixelY++) {
		//  Build a list of nodes.
		nodes = 0;
		j = SSD1306_poly->SSD1306_points_number - 1;
		for (i=0; i<SSD1306_poly->SSD1306_points_number; i++) {
			if ((polyY[i]<(double) pixelY && polyY[j]>=(double) pixelY)	
			||  (polyY[j]<(double) pixelY && polyY[i]>=(double) pixelY)) {
				nodeX[nodes++]=(int)round(polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i]) * (polyX[j]-polyX[i])); 
			}
			j=i; 
		}
		//  Sort the nodes, via a simple �Bubble� sort.
		i=0;
		while (i<nodes-1) {
			if (nodeX[i]>nodeX[i+1]) {
				swap=nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--; 
			}	else {
				i++; 
			}
		}
		//  Fill the pixels between node pairs.
		for (i=0; i<nodes; i+=2) {
			if   (nodeX[i  ]>=SSD1306_LCDWIDTH) break;
			if   (nodeX[i+1]> 0 ) {
				if (nodeX[i  ]< 0 ) nodeX[i  ]=0 ;
				if (nodeX[i+1]> SSD1306_LCDWIDTH) nodeX[i+1]=SSD1306_LCDWIDTH;
				for (pixelX=nodeX[i]; pixelX<=nodeX[i+1]; pixelX++) {
					ssd1306DrawPixel((pixelX), (pixelY), color, layer);
				} 
			}
		}
	}
}


/**
 *  \brief Brief
 *  
 *  \param [in] x Parameter_Description
 *  \param [in] y Parameter_Description
 *  \param [in] h Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306DrawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint16_t color, uint16_t layer) {

  ssd1306DrawLine(x, y, x, y+h-1, color, layer);
}

/**
 *  \brief Brief
 *  
 *  \param [in] x Parameter_Description
 *  \param [in] y Parameter_Description
 *  \param [in] w Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306DrawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color, uint16_t layer) {
					 
  ssd1306DrawLine(x, y, x+w-1, y, color, layer);
}

/**
 *  \brief Brief
 *  
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306FillScreen(uint16_t color) {
	
  ssd1306FillRect(0, 0, _width, _height, color, LAYER0);
}

/**
 *  \brief Draws a circle, midpoint circle algorithm
 *  
 *  \param [in] x0 Parameter_Description
 *  \param [in] y0 Parameter_Description
 *  \param [in] radius Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color) {
  int x = radius;
  int y = 0;
  int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

  if ( x < 0) return;
  while( y <= x )
  {
    ssd1306DrawPixel( x + x0,  y + y0, color, LAYER0); // Octant 1
    ssd1306DrawPixel( y + x0,  x + y0, color, LAYER0); // Octant 2
    ssd1306DrawPixel(-x + x0,  y + y0, color, LAYER0); // Octant 4
    ssd1306DrawPixel(-y + x0,  x + y0, color, LAYER0); // Octant 3
    ssd1306DrawPixel(-x + x0, -y + y0, color, LAYER0); // Octant 5
    ssd1306DrawPixel(-y + x0, -x + y0, color, LAYER0); // Octant 6
    ssd1306DrawPixel( x + x0, -y + y0, color, LAYER0); // Octant 8
    ssd1306DrawPixel( y + x0, -x + y0, color, LAYER0); // Octant 7
    y++;
    if (decisionOver2<=0)
    {
      decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
    }
    else
    {
      x--;
      decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
    }
  }
}

/**
 *  \brief Draw circle helper
 *  
 *  \param [in] x0 Parameter_Description
 *  \param [in] y0 Parameter_Description
 *  \param [in] r Parameter_Description
 *  \param [in] cornername Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void ssd1306DrawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, uint16_t layer) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      ssd1306DrawPixel(x0 + x, y0 + y, color, layer);
      ssd1306DrawPixel(x0 + y, y0 + x, color, layer);
    } 
    if (cornername & 0x2) {
      ssd1306DrawPixel(x0 + x, y0 - y, color, layer);
      ssd1306DrawPixel(x0 + y, y0 - x, color, layer);
    }
    if (cornername & 0x8) {
      ssd1306DrawPixel(x0 - y, y0 + x, color, layer);
      ssd1306DrawPixel(x0 - x, y0 + y, color, layer);
    }
    if (cornername & 0x1) {
      ssd1306DrawPixel(x0 - y, y0 - x, color, layer);
      ssd1306DrawPixel(x0 - x, y0 - y, color, layer);
    }
  }
}

/**
 *  \brief Brief
 *  
 *  \param [in] x0 Parameter_Description
 *  \param [in] y0 Parameter_Description
 *  \param [in] r Parameter_Description
 *  \param [in] color Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void    ssd1306FillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color, uint16_t layer) {
  ssd1306DrawFastVLine(x0, y0-r, 2*r+1, color, layer);
  ssd1306FillCircleHelper(x0, y0, r, 3, 0, color, layer);
}

/**************************************************************************/
/*!
    @brief Used to do circles and roundrects
*/
/**************************************************************************/
void ssd1306FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, uint16_t layer) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      ssd1306DrawFastVLine(x0+x, y0-y, 2*y+1+delta, color, layer);
      ssd1306DrawFastVLine(x0+y, y0-x, 2*x+1+delta, color, layer);
    }
    if (cornername & 0x2) {
      ssd1306DrawFastVLine(x0-x, y0-y, 2*y+1+delta, color, layer);
      ssd1306DrawFastVLine(x0-y, y0-x, 2*x+1+delta, color, layer);
    }
  }
}
/**************************************************************************/
/*!
    @brief Draws a filled rectangle
*/
/**************************************************************************/
void ssd1306FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color, uint16_t layer) {
	uint8_t x0, x1, y1;

	x0 = x;
	x1 = x + w;
	y1 = y + h;
	for(; y < y1; y++)
			for(x = x0; x < x1; x++)
					ssd1306DrawPixel( x, y, color, layer);
}

/**************************************************************************/
/*!
    @brief Draws a rectangle
*/
/**************************************************************************/
void ssd1306DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t layer) {
	int16_t x1, y1;
	if ( (w == 0) | (h == 0)) return;
	x1 = x + w - 1;
	y1 = y + h - 1;
	
	if((h > 2 ) | (w > 2)) {
		ssd1306DrawLine(	 x,    y,   x1,  y, color, layer);
		ssd1306DrawLine(	 x,   y1,   x1, y1, color, layer);
		ssd1306DrawLine(	 x,  y+1,  x, y1-1, color, layer);
		ssd1306DrawLine(  x1,  y+1, x1, y1-1, color, layer);
	} else {
		ssd1306DrawLine(	 x,    y,   x1,  y, color, layer);
		ssd1306DrawLine(	 x,   y1,   x1, y1, color, layer);
	}
}

/**************************************************************************/
/*!
    @brief Draws a filled rectangle
*/
/**************************************************************************/
void ssd1306DrawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  uint8_t *pbitmap;
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      pbitmap = bitmap + j * byteWidth + i / 8;
      if((uint8_t)(*pbitmap) & (128 >> (i & 7))) {
        ssd1306DrawPixel(x+i, y+j, color, LAYER0);
      }
    }
  }
}

/**************************************************************************/
/*! 
    @brief Draw a character
*/
/**************************************************************************/
void  ssd1306DrawChar(int16_t x, int16_t y, uint8_t c, uint8_t size, 
uint16_t color, uint16_t layer) 
{

    if( (x >= _width)               || // Clip right
        (y >= _height)              || // Clip bottom
        ((x + 6 * size - 1) < 0)    || // Clip left
        ((y + 8 * size - 1) < 0))      // Clip top
    return;

    for (int8_t i=0; i<6; i++ ) 
    {
        int8_t line;
        if (i == 5) 
        {
            line = 0x0;
        }
        else 
        {
            line = (int8_t)*(font5x7+(c*5)+i);
        }
        
        for (int8_t j = 0; j<8; j++) 
        {
            if (line & 0x1) 
            {
                if (size == 1) // default size
                {
                    ssd1306DrawPixel(x+i, y+j, color, layer);
                }
                else 
                {  // big size
                    ssd1306DrawRect(x+(i*size), y+(j*size), size, size, color, layer);
                } 
            } 
            line >>= 1;
        }
    }
}

/**************************************************************************/
/*!
    @brief  Draws a string using the supplied font data.
    @param[in]  x
                Starting x co-ordinate
    @param[in]  y
                Starting y co-ordinate
    @param[in]  text
                The string to render
    @param[in]  font
                Pointer to the FONT_DEF to use when drawing the string
    @section Example
    @code 
    #include "drivers/lcd/bitmap/ssd1306/ssd1306.h"
    #include "drivers/lcd/smallfonts.h"
    
    // Configure the pins and initialise the LCD screen
    ssd1306Init();
    // Render some text on the screen
    ssd1306DrawString(1, 10, "5x8 System", Font_System5x8);
    ssd1306DrawString(1, 20, "7x8 System", Font_System7x8);
    // Refresh the screen to see the results
    ssd1306Refresh();
    @endcode
*/
/**************************************************************************/
void  ssd1306DrawString(int16_t x, int16_t y, const char *text, uint8_t size, 
                        uint16_t color, uint16_t layer)
{
  uint8_t l;
  for (l = 0; l < strlen(text); l++)
  {
    ssd1306DrawChar(x + (l * (5*size + 1)), y, text[l], size, color, layer);
  }
}

/**************************************************************************/
/*!
    @brief  Shifts the contents of the frame buffer up the specified
            number of pixels
    @param[in]  height
                The number of pixels to shift the frame buffer up, leaving
                a blank space at the bottom of the frame buffer x pixels
                high
*/
/**************************************************************************/
void ssd1306ShiftFrameBuffer( uint16_t height, uint16_t direction)
{  
  int16_t y, x;
  if (height == 0) return;
//while (height > 0)
//{
  switch (direction)
  {	
    case 0:
    // This is horribly inefficient, but at least easy to understand
    // In a production environment, this should be significantly optimised
    if (height >= SSD1306_LCDHEIGHT)
    {
      // Clear the entire frame buffer
      ssd1306ClearScreen( LAYER0 );
      return;
    }
    for (y = 0; y < SSD1306_LCDHEIGHT; y++)
    {
      for (x = 0; x < SSD1306_LCDWIDTH; x++)
      {
        if ((SSD1306_LCDHEIGHT ) - y > 1)
        {
          // Shift height from further ahead in the buffer
          ssd1306GetPixel(x, y + 1) ? ssd1306DrawPixel(x, y, WHITE, LAYER0) : ssd1306DrawPixel(x, y, BLACK, LAYER0);
        }
        else
        {
          // Clear the entire line
          ssd1306DrawPixel(x, y, BLACK, LAYER0);
        }
      }
    }
    break;
    case 1:
    {
    //static uint8_t    buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];
      y = SSD1306_COUNTOF(buffer);
      x = y;
      for (x = SSD1306_COUNTOF(buffer) - 1; x >= 0 /*SSD1306_COUNTOF(buffer)*/; x--)
      {
        buffer[x] = buffer[x] << height;
        if (((x - (int16_t)SSD1306_COUNTOF(buffer)/8) >= 0) && ((buffer[x - SSD1306_COUNTOF(buffer)/8] & 0x80) == 0x80))
        {
          buffer[x] = buffer[x] | 0x01;
        }
      }
    }
    break;
    case 2:
    {
      for(y = 0; y <= 7; y++)
      {
        for (x = 0; x < (SSD1306_COUNTOF(buffer)/8 - 1); x++)
        {
          buffer[x + (128 * y)] = buffer[x + (128 * y) + 1];
        }
        buffer[127 + (128 * y)] = 0;
      }
    }
    break;
    case 3:
    {
      for(y = 0; y <= 7; y++)
      {
        for (x = (SSD1306_COUNTOF(buffer)/8 - 1); x > 0; x--)
        {
          buffer[x + (128 * y)] = buffer[x - 1 + (128 * y)];
        }
        buffer[128 * y] = 0;
      }
    }
    break;
    default: //return;
    break;
  }
//  height--;
//  }
}

/**
 *  \brief Brief
 *  
 *  \return Return_Description
 *  
 *  \details Details
 */
#ifdef MULTILAYER  
void ssd1306MixFrameBuffer(void)
{
  uint16_t i;
  for(i = 0; i < (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8); i++)
  {
    buffer_ol[i] |= buffer[i];
  }  
}
#endif  


/******************************************************************************
* Function Name : screenScrollShowMessage
* Description   : 
* Argument      : none
* Return Value  : none
******************************************************************************/
void screenScrollShowMessage(const char *text, uint16_t color)
{      
    static int currentlineNum = 0; 
    uint16_t m,n;
    
    for (m = 0; m< strlen(text); m++)
    {
        if(n>=21)
        {
            n=0;
            currentlineNum++;
        }

        if(currentlineNum>=8)
        {            
            memset(buffer_ol,0x00,1204);
            for(uint16_t i=0; i<896; i++)
            {
                buffer[i] = buffer[i+128];
            }
            for(uint16_t i=0; i<127; i++)
            {
                buffer[i+896] = 0x00;
            }
           currentlineNum = 7;
            
           ssd1306Refresh();
  
        }
        
        ssd1306DrawChar( (m % 21) * (5 + 1), currentlineNum * 8, text[m], 1, color, LAYER0);
        n++;  
    }
    currentlineNum++;
    ssd1306Refresh();
}













































