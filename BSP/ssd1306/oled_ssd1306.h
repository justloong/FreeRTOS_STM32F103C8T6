#ifndef	__OLDE_SSD1306_H
#define	__OLDE_SSD1306_H

// Config HardWare Peripheral
//#define SSD1306_SPI_INTERFACE                   1
#define SSD1306_I2C_INTERFACE                   1
#if SSD1306_I2C_INTERFACE
#define USE_I2C_PERIPHERAL                      1
#define USE_IO_SIMULATE_I2C                     0
#define SSD1306_ADDRESS                         0x78
#define SSD1306_WRITE_DATA                      0x40
#define SSD1306_WRITE_CMD                       0x00
#endif



#define     SSD1306_COUNTOF(a)          (sizeof(a) / sizeof(a[0]))
#define     SSD1306_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define     SSD1306_MAX(a,b)            (((a) > (b)) ? (a) : (b))

// Converts degrees to radians to degrees macro
#define     M_PI                                3.141593
#define     degreesToRadians(angleDegrees)      (double)(angleDegrees * M_PI / 180.0)
#define     radiansToDegrees(angleRadians)      (double)(angleRadians * 180.0 / M_PI)
//#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


#define     BLACK                                 0
#define     WHITE                                 1
#define     INVERSE                               2

#define     MULTILAYER                            1
#define     LAYER0                                1
#define     LAYER1                                2

#define     SSD1306_LCDWIDTH                    128
#define     SSD1306_LCDHEIGHT                   64

#define     MAX_POLY_CORNERS                    10
// Commands
#define SSD1306_SETCONTRAST               0x81
#define SSD1306_DISPLAYALLON_RESUME       0xA4
#define SSD1306_DISPLAYALLON              0xA5
#define SSD1306_NORMALDISPLAY             0xA6
#define SSD1306_INVERTDISPLAY             0xA7
#define SSD1306_DISPLAYOFF                0xAE
#define SSD1306_DISPLAYON                 0xAF
#define SSD1306_SETDISPLAYOFFSET          0xD3
#define SSD1306_SETCOMPINS                0xDA
#define SSD1306_SETVCOMDETECT             0xDB
#define SSD1306_SETDISPLAYCLOCKDIV        0xD5
#define SSD1306_SETPRECHARGE              0xD9
#define SSD1306_SETMULTIPLEX              0xA8
#define SSD1306_SETLOWCOLUMN              0x00
#define SSD1306_SETHIGHCOLUMN             0x10
#define SSD1306_SETSTARTLINE              0x40
#define SSD1306_MEMORYMODE                0x20
#define SSD1306_COMSCANINC                0xC0
#define SSD1306_COMSCANDEC                0xC8
#define SSD1306_SEGREMAP                  0xA0
#define SSD1306_CHARGEPUMP                0x8D
#define SSD1306_EXTERNALVCC               0x1
#define SSD1306_SWITCHCAPVCC              0x2
// macros
#define SSD1306MSDELAY(c)      osDelay(c);// HAL_Delay( c );
	
	
//
typedef struct
{
  uint8_t  cmdOrData;  
  uint8_t  *sendData;
}SSD1306_I2C_Data;

typedef struct {
    double x;
    double y;
} SSD1306_pointTypeDef;

typedef struct {
    SSD1306_pointTypeDef * SSD1306_points_pointer;
    int SSD1306_points_number;
} SSD1306_polyTypeDef;

	
// Initialisation/Config Prototypes
void    ssd1306SendByte_I2C(uint8_t data, uint8_t cmdOrData);
void    ssd1306SendByte(uint8_t);
void    ssd1306Init (uint8_t);
void    ssd1306Refresh ( void );
void    ssd1306TurnOn(void);
void    ssd1306TurnOff(void);
void    ssd1306DrawPixel(int16_t, int16_t, uint16_t, uint16_t) ;
void    ssd1306ClearPixel(int16_t, int16_t);
uint8_t ssd1306GetPixel(int16_t, int16_t);
void    ssd1306ClearScreen ( uint16_t );
void    ssd1306DrawLine(int16_t, int16_t, int16_t, int16_t, uint16_t, uint16_t);
void    ssd1306_DrawPolygon(SSD1306_polyTypeDef * SSD1306_poly, int16_t x, int16_t y, uint16_t color, uint16_t layer);
void    ssd1306_FillPolygon(SSD1306_polyTypeDef * SSD1306_poly, double x, double y, uint16_t color, uint16_t layer);
void    ssd1306DrawFastVLine(int16_t, int16_t, int16_t, uint16_t, uint16_t layer);
void    ssd1306DrawFastHLine(int16_t, int16_t, int16_t, uint16_t, uint16_t layer);
void    ssd1306FillScreen(uint16_t);
void    ssd1306DrawCircle(uint8_t, uint8_t, uint8_t, uint16_t);
void    ssd1306DrawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, uint16_t layer);
void    ssd1306FillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color, uint16_t layer);
void    ssd1306FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, uint16_t layer);
void    ssd1306FillRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
void    ssd1306DrawRect(int16_t, int16_t, int16_t, int16_t, uint16_t, uint16_t);
void    ssd1306DrawBitmap(int16_t, int16_t, uint8_t*, int16_t, int16_t, uint16_t);
void    ssd1306DrawChar(int16_t, int16_t, uint8_t, uint8_t, uint16_t, uint16_t);
void    ssd1306DrawString( int16_t, int16_t, const char * , uint8_t, uint16_t, uint16_t);
void    ssd1306ShiftFrameBuffer( uint16_t height, uint16_t direction);
void    ssd1306MixFrameBuffer(void);



















#endif
