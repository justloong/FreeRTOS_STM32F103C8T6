/**
  ******************************************************************************
  * File Name          : ssd1306libtest.c
  * Description        : 
  ******************************************************************************
  *
  * COPYRIGHT(c) 
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "ssd1306/oled_ssd1306.h"
#include "stdlib.h"
#include "ssd1306/thegame.h"
#include <math.h>

void Points_Copy (SSD1306_pointTypeDef * src, SSD1306_pointTypeDef * dst, int num);
void ShootMove(MobileUnitTypeDef * Shoot );
void ShootMake(MobileUnitTypeDef * BattleStar );


        MobileUnitTypeDef BattleStar;
        MobileUnitTypeDef Meteorite;
        SSD1306_polyTypeDef SpaceShip;
        
	SSD1306_pointTypeDef BattleStarShape[] = {
	 {  -5, -5},
	 {   4, -5},
	 {   1, -2},
	 {   7,  0},
	 {   1,  2},
	 {   4,  5},
	 {  -5,  5},
	 {   0,  0}
	};
//	 {   0,  0},
//	 {   9,  0},
//	 {   6,  3},
//	 {   8,  5},
//	 {   6,  7},
//	 {   9, 10},
//	 {   0, 10},
//	 {   5,  5}
//	};

  SSD1306_pointTypeDef MeteoriteShape[] = {
	 {  -2 *2,  -5 *2},
	 {   2 *2,  -5 *2},
	 {   5 *2,  -2 *2},
	 {   5 *2,   2 *2},
	 {   2 *2,   5 *2},
	 {  -2 *2,   5 *2},
	 {  -5 *2,   2 *2},
	 {  -6 *2,  -2 *2}
//	 {   -10,  10},
//	 {    10,  10},
//	 {    10, -10},
//	 {   -10, -10}
	};

// temporary var
	int  dfs;
	char str[30];
	
void SSD1306LibTest(void)
{
	int i,j,k;
	static double l, m;
  /*******************/
	/* Init BattleStar */
  /*******************/
  BattleStar.shape.SSD1306_points_pointer = BattleStarShape;
	BattleStar.shape.SSD1306_points_number = SSD1306_COUNTOF(BattleStarShape);
	for( i = 0; i < SHOOTS; i++)
	{
			BattleStar.shoot[i].speed = SHOOT_SPEED;
			BattleStar.shoot[i].alive = 0;
			BattleStar.shoot[i].lenth = 0;
			BattleStar.shoot[i].x0 = 0;
			BattleStar.shoot[i].y0 = 0;
			BattleStar.shoot[i].angle = 0;
	}
	BattleStar.angle =   0;
	BattleStar.posX  =  16;
	BattleStar.posY  =  28;
	BattleStar.speed =   0;
  /* End of BattleStar init */
  
  /******************/
  /* Init Meteorite */
  /******************/
  Meteorite.shape.SSD1306_points_pointer = MeteoriteShape;
  Meteorite.shape.SSD1306_points_number = SSD1306_COUNTOF(MeteoriteShape);
  Meteorite.posX =  96;
  Meteorite.posY =  50;
  Meteorite.angle =  0;
  /* End of BattleStar init */
  
  
  

	#if 1
	/* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen(LAYER0|LAYER1);
    for(i = 000; i < 400; i++)
    {
      ssd1306ClearScreen( LAYER1 );
      if (i >= 0 && i < 100)
      {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 127), (rand() % 5), WHITE, LAYER0) ;
        }
        ssd1306DrawString( 10,  0, "  HAPPY   ",  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "   NEW    ",  2, WHITE, LAYER1);
        ssd1306DrawString( 14, 32, "  YEAR    ",  3, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 1);
      } else if (i >= 100 && i < 200) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 127), (58 + rand() % 5), WHITE, LAYER0) ;
        }
        ssd1306DrawString( 10,  0, "Draw/Clear",  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "  pixel  " ,  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 32, "DualBuffer",  2, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 0);
      } else if (i >= 200 && i < 300) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 10) + 118, rand() % 64, WHITE, LAYER0) ;
        }
        ssd1306DrawString( 10,  0, "Draw/Clear",  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "  pixel  " ,  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 32, "DualBuffer",  2, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 2);
      }else if (i >= 300 && i < 400) {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 10) + 0, rand() % 64, WHITE, LAYER0) ;
        }
        ssd1306DrawString( 10,  0, "Draw/Clear",  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "  pixel  " ,  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 32, "DualBuffer",  2, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 3);
      }
      
      ssd1306Refresh();
      SSD1306MSDELAY(40);
    }

    
    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, "  DrawLine", 2, WHITE, LAYER0);
    ssd1306FillRect(32, 16,  64, 32, WHITE , LAYER0);
    ssd1306DrawLine(10, 20, 118, 20, WHITE , LAYER0);
    ssd1306DrawLine(10, 21, 118, 21, WHITE , LAYER0);
    ssd1306DrawLine(10, 20, 118, 36, BLACK , LAYER0);
    ssd1306DrawLine(10, 21, 118, 37, BLACK , LAYER0);
    ssd1306DrawLine(10, 20, 118, 44, INVERSE, LAYER0);
    ssd1306DrawLine(10, 21, 118, 45, INVERSE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);
    
    
    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(0, 16,  64, 47, WHITE, LAYER0);
    ssd1306DrawCircle(64, 39, 19, WHITE);
    ssd1306DrawCircle(64, 39, 22, BLACK);
    ssd1306DrawCircle(32, 39, 19, WHITE);
    ssd1306DrawCircle(32, 39, 22, BLACK);
    ssd1306DrawCircle(96, 39, 19, WHITE);
    ssd1306DrawCircle(96, 39, 22, BLACK);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " DrawCircle ", 2, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);
    
    
    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(0, 16,  127, 24, WHITE, LAYER0);
    ssd1306FillCircle(32, 40, 20, WHITE, LAYER0);
    ssd1306FillCircle(64, 40, 20, BLACK, LAYER0);
    ssd1306FillCircle(96, 40, 20, WHITE, LAYER0);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " FillCircle", 2, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);

    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(26, 24, 73, 24, WHITE, LAYER0);
    ssd1306DrawRect(28, 26, 73, 24, WHITE, LAYER0);
    ssd1306DrawString(30, 30, "Window", 2, INVERSE, LAYER0);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, "DrawFillRect", 2, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);

    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    ssd1306FillRect(32, 16,  64, 48, WHITE, LAYER0);
    ssd1306DrawString(0, 20, "     AaBbCcXxYyZz", 1, INVERSE, LAYER0);
    ssd1306DrawString(0, 32, "AaBbCcXx", 3, INVERSE, LAYER0);

    /* message */
    ssd1306FillRect(0, 0, 128, 16, BLACK, LAYER0);
    ssd1306DrawString(0, 0, " DrawString", 2, WHITE, LAYER0);
		ssd1306Refresh();
    /* */
    SSD1306MSDELAY(5000);
    #endif

    /* ---Test routine--- */
    /* take display buffer */
    ssd1306ClearScreen( LAYER0 | LAYER1 );
    /* message */
	/* Intro */
    ssd1306DrawString(3,  0, "Battle Star", 2, WHITE, LAYER0);
	ssd1306Refresh();
	for(i = 64; i>=-210; i-=1)
	{
		ssd1306FillRect(0, 16, 128, 48, BLACK, LAYER0 | LAYER1);        
		//ssd1306DrawString(i,  16, "The Game", 6, WHITE, LAYER0);  // SIZE 6 - FULL SCREEN 
		ssd1306DrawString(i,  16, "Hello World ", 5, WHITE, LAYER0);  // SIZE 6 - FULL SCREEN 
		ssd1306Refresh();
		SSD1306MSDELAY(3);
	}
	/* END OF INTRO */

    #if 1
	for(i = 0; i < 360; i++)
    {
        ssd1306FillRect(   0,  0, 128, 64, BLACK, LAYER0 | LAYER1 );
        ssd1306DrawString( 3,  0, "Battle Star", 2, WHITE, LAYER0 );
        /* draw Battle Star */

        /* draw Meteorite */

        RotateObject( &Meteorite,   1.0 );
        RotateObject( &BattleStar,  2 );

        DrawObject( &BattleStar, 0 );
        if ( dfs == 0 )
        {
            DrawObject( &Meteorite,  0 );
        }
        else
        {
            DrawObject( &Meteorite,  1 );
        }
        sprintf(str,"PiP:  %03d", dfs) ;
        ssd1306DrawString(0,  56, str, 1, WHITE, LAYER0);
        //  sprintf(str,"BSA:  %05.1f", BattleStar.angle) ;
        //	  ssd1306DrawString(0,  48, str, 1, WHITE, LAYER0);
        //  sprintf(str,"BSSA: %05.1f", BattleStar.shoot[0].angle) ;
        //	  ssd1306DrawString(0,  56, str, 1, WHITE, LAYER0);
        dfs =  0;
        if (i%5 ==0)
        {
            ShootMake(&BattleStar);
        }
        ShootMove(&BattleStar);

        ssd1306Refresh();
        SSD1306MSDELAY(10);
    }
    #endif
}	
/*
 *  Copy polygon to polygon
 */
void Points_Copy (SSD1306_pointTypeDef * src, SSD1306_pointTypeDef * dst, int num)
{
	while (num)
	{
		dst->x = src->x ;
		dst->y = src->y ;
		dst++; src++;
		num--;
	}
}

/**************************************************************
 *              Function draws polygonal ojects               * 
 **************************************************************/
void DrawObject(MobileUnitTypeDef * Object, int fill)
{
  if (fill)
  {
    ssd1306_FillPolygon( &Object->shape, round(Object->posX), round(Object->posY), WHITE, LAYER0 );
  }
  else
  {
    ssd1306_DrawPolygon( &Object->shape, round(Object->posX), round(Object->posY), WHITE, LAYER0 );
  }
}

/**************************************************************
 * Function rotates MobileUnitTypeDef object to angle degrees *
 **************************************************************/
void RotateObject( MobileUnitTypeDef * Object, double angle )
{
	int i;
  double  x, y ;
	double xsina, ysina, xcosa, ycosa;
	
	for ( i = 0; i < Object->shape.SSD1306_points_number; i++ )
	{
		x = Object->shape.SSD1306_points_pointer[i].x;
		y = Object->shape.SSD1306_points_pointer[i].y;
		xcosa = x * cos(degreesToRadians(angle));
		ysina = y * sin(degreesToRadians(angle));
		Object->shape.SSD1306_points_pointer[i].x = xcosa - ysina;
		ycosa = y * cos(degreesToRadians(angle));
		xsina = x * sin(degreesToRadians(angle));
		Object->shape.SSD1306_points_pointer[i].y = ycosa + xsina;
	}
  Object->angle = Object->angle + angle;
}

void ShootMove(MobileUnitTypeDef * BattleStar )
{
	int i;
	for( i = 0; i < SHOOTS; i++)
	{
		if (BattleStar->shoot[i].alive)
		{
			double scale_x = cos(degreesToRadians(BattleStar->shoot[i].angle));
			double scale_y = sin(degreesToRadians(BattleStar->shoot[i].angle));
			double velocity_x = (BattleStar->shoot[i].speed * scale_x);
			double velocity_y = (BattleStar->shoot[i].speed * scale_y);
			BattleStar->shoot[i].x0 += velocity_x;
			BattleStar->shoot[i].y0 += velocity_y;
			BattleStar->shoot[i].alive--;
			ssd1306DrawPixel( BattleStar->shoot[i].x0, BattleStar->shoot[i].y0 , WHITE, LAYER0) ;
			if( (BattleStar->shoot[i].x0 >= SSD1306_LCDWIDTH)            || // Clip right
					(BattleStar->shoot[i].y0 >= SSD1306_LCDHEIGHT)           || // Clip bottom
					(BattleStar->shoot[i].x0 < 0) || 														// Clip left
					(BattleStar->shoot[i].y0 < 0)  )
			{
				BattleStar->shoot[i].alive = 0;
        BattleStar->shoot[i].y0 = 0;
        BattleStar->shoot[i].x0 = 0;
			}
      //int pointInPolygon(SSD1306_pointTypeDef * poly, int polyCorners, double x, double y  ) {
      dfs += pointInPolygon( (MobileUnitTypeDef *)&Meteorite, Meteorite.shape.SSD1306_points_number,
      BattleStar->shoot[i].x0, BattleStar->shoot[i].y0 );
      // end of debug
		}
	}
}

void ShootMake(MobileUnitTypeDef * BattleStar )
{
	static int i;
	for( i = 0; i < SHOOTS; i++)
	{
		if (BattleStar->shoot[i].alive == 0)
		{
			BattleStar->shoot[i].alive = SHOOT_LIFE;
			BattleStar->shoot[i].angle = BattleStar->angle;
			BattleStar->shoot[i].x0 = BattleStar->posX;
			BattleStar->shoot[i].y0 = BattleStar->posY;
			return;
		}
	}
}

//  Globals which should be set before calling this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

int pointInPolygon(MobileUnitTypeDef * poly, int polyCorners, double x, double y  ) {
	int   nodes, nodeX[MAX_POLY_CORNERS], pixelX, pixelY;
  int   i, j = polyCorners - 1;
  int   oddNodes = 0;

  x = x - poly->posX;
  y = y - poly->posY;
  
  for ( i = 0; i < polyCorners; i++ ) 
  {
    if ((( poly->shape.SSD1306_points_pointer[i].y < y  && poly->shape.SSD1306_points_pointer[j].y >= y ) 
       || (poly->shape.SSD1306_points_pointer[j].y < y  && poly->shape.SSD1306_points_pointer[i].y >= y )) 
       && (poly->shape.SSD1306_points_pointer[i].x <= x || poly->shape.SSD1306_points_pointer[j].x <= x )) 
    {
      oddNodes ^= ( poly->shape.SSD1306_points_pointer[i].x + (y-poly->shape.SSD1306_points_pointer[i].y) 
      / (poly->shape.SSD1306_points_pointer[j].y-poly->shape.SSD1306_points_pointer[i].y) 
      * (poly->shape.SSD1306_points_pointer[j].x-poly->shape.SSD1306_points_pointer[i].x)<x); 
    }
    j=i; 
  }
  return oddNodes; 
}
//bool pointInPolygon() {
//  int   i, j=polyCorners-1 ;
//  bool  oddNodes=NO      ;
//  for (i=0; i<polyCorners; i++) {
//    if ((polyY[i]< y && polyY[j]>=y
//    ||   polyY[j]< y && polyY[i]>=y)
//    &&  (polyX[i]<=x || polyX[j]<=x)) {
//      oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
//    j=i; }
//  return oddNodes; }
  
  
  
  
/***************************************************************END OF FILE****/
