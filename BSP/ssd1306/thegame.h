#ifndef __THEGAME_H__
#define __THEGAME_H__

#include "ssd1306/oled_ssd1306.h"


#define			SHOOTS  				4
#define 		SHOOT_SPEED			5
#define 		SHOOT_LIFE			50

/*
 *  Shoot structure
 */
typedef struct {
	double 	x0, y0, speed, angle;
	int 		alive, lenth;
}	ShootTypeDef;

/*
 *  Battle star structure
 */
typedef struct {
		SSD1306_polyTypeDef shape;
		int  posX, posY;
		double	angle;
		double  speed;
		ShootTypeDef shoot[SHOOTS];
} MobileUnitTypeDef ;


void DrawObject(MobileUnitTypeDef * Object, int fill);
void RotateObject( MobileUnitTypeDef * Object, double angle );
int pointInPolygon(MobileUnitTypeDef * poly, int polyCorners, double x, double y  );

#endif
