/*
 *    dynamics.c
 *
 *	  This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *    Copyright 2010
 *		
 *	  This file is based on Wiiuse, written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	  Copyright 2006-2007
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	  $Header$
 */

/**
 *	@file
 *	@brief Handles the dynamics of the wiimote.
 *
 *	The file includes functions that handle the dynamics
 *	of the wiimote.  Such dynamics include orientation and
 *	motion sensing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "definitions.h"
#include "wiic_internal.h"
#include "ir.h"
#include "dynamics.h"

/**
 *	@brief Calculate the roll, pitch, yaw.
 *
 *	@param ac			An accelerometer (accel_t) structure.
 *	@param accel		[in] Pointer to a vec3b_t structure that holds the raw acceleration data.
 *	@param orient		[out] Pointer to a orient_t structure that will hold the orientation data.
 *	@param rorient		[out] Pointer to a orient_t structure that will hold the non-smoothed orientation data.
 *	@param smooth		If smoothing should be performed on the angles calculated. 1 to enable, 0 to disable.
 *
 *	Given the raw acceleration data from the accelerometer struct, calculate
 *	the orientation of the device and set it in the \a orient parameter.
 */
void calculate_orientation(struct gforce_t* gforce, struct orient_t* orient, int smooth) {
	float x, y, z;

	/*
	 *	roll	- use atan(z / x)		[ ranges from -180 to 180 ]
	 *	pitch	- use atan(z / y)		[ ranges from -180 to 180 ]
	 *	yaw		- impossible to tell without IR
	 */

	/* yaw - set to 0, IR will take care of it if it's enabled */
	orient->yaw = 0.0f;

	/* find out how much it actually moved and normalize to +/- 1g */
	x = gforce->x;
	y = gforce->y;
	z = gforce->z;

	/* make sure x,y,z are between -1 and 1 for the tan functions */
	if (x < -1.0f)			x = -1.0f;
	else if (x > 1.0f)		x = 1.0f;
	if (y < -1.0f)			y = -1.0f;
	else if (y > 1.0f)		y = 1.0f;
	if (z < -1.0f)			z = -1.0f;
	else if (z > 1.0f)		z = 1.0f;

	/* if it is over 1g then it is probably accelerating and the gravity vector cannot be identified */
	if (abs(gforce->x) <= 1.0) {
		/* roll */
		x = -RAD_TO_DEGREE(atan2f(x, z));

		orient->roll = x;
	}

	if (abs(gforce->y) <= 1.0) {
		/* pitch */
		y = RAD_TO_DEGREE(atan2f(y, z));

		orient->pitch = y;
	}

	if(smooth) {	// Same stuff, but this time using unsmoothed gforce
		x = gforce->ax;
		y = gforce->ay;
		z = gforce->az;

		if (x < -1.0f)			x = -1.0f;
		else if (x > 1.0f)		x = 1.0f;
		if (y < -1.0f)			y = -1.0f;
		else if (y > 1.0f)		y = 1.0f;
		if (z < -1.0f)			z = -1.0f;
		else if (z > 1.0f)		z = 1.0f;

		if (abs(gforce->ax) <= 1.0) {
			/* roll */
			x = -RAD_TO_DEGREE(atan2f(x, z));

			orient->a_roll = x;
		}

		if (abs(gforce->ay) <= 1.0) {
			/* pitch */
			y = RAD_TO_DEGREE(atan2f(y, z));

			orient->a_pitch = y;
		}		
	}
	else {	// Same values
		orient->a_roll = x;
		orient->a_pitch = y;		
	}
}


/**
 *	@brief Calculate the gravity forces on each axis.
 *
 *	@param ac			An accelerometer (accel_t) structure.
 *	@param accel		[in] Pointer to a vec3b_t structure that holds the raw acceleration data.
 *	@param gforce		[out] Pointer to a gforce_t structure that will hold the gravity force data.
 */
void calculate_gforce(struct accel_t* ac, struct vec3b_t* accel, struct gforce_t* gforce, int smooth) {
	float xg, yg, zg;

	/* find out how much it has to move to be 1g */
	xg = (int)ac->cal_g.x;
	yg = (int)ac->cal_g.y;
	zg = (int)ac->cal_g.z;

	/* find out how much it actually moved and normalize to +/- 1g */
	gforce->ax = ((int)accel->x - (int)ac->cal_zero.x) / xg;
	gforce->ay = ((int)accel->y - (int)ac->cal_zero.y) / yg;
	gforce->az = ((int)accel->z - (int)ac->cal_zero.z) / zg;
	
	if(smooth) {
		apply_smoothing(gforce, ac->st_alpha);
	}
	else {
		gforce->x = gforce->ax;
		gforce->y = gforce->ay;
		gforce->z = gforce->az;		
	}
}


/**
 *	@brief Calculate the angle and magnitude of a joystick.
 *
 *	@param js	[out] Pointer to a joystick_t structure.
 *	@param x	The raw x-axis value.
 *	@param y	The raw y-axis value.
 */
void calc_joystick_state(struct joystick_t* js, float x, float y) {
	float rx, ry, ang;

	/*
	 *	Since the joystick center may not be exactly:
	 *		(min + max) / 2
	 *	Then the range from the min to the center and the center to the max
	 *	may be different.
	 *	Because of this, depending on if the current x or y value is greater
	 *	or less than the assoicated axis center value, it needs to be interpolated
	 *	between the center and the minimum or maxmimum rather than between
	 *	the minimum and maximum.
	 *
	 *	So we have something like this:
	 *		(x min) [-1] ---------*------ [0] (x center) [0] -------- [1] (x max)
	 *	Where the * is the current x value.
	 *	The range is therefore -1 to 1, 0 being the exact center rather than
	 *	the middle of min and max.
	 */
	if (x == js->center.x)
		rx = 0;
	else if (x >= js->center.x)
		rx = ((float)(x - js->center.x) / (float)(js->max.x - js->center.x));
	else
		rx = ((float)(x - js->min.x) / (float)(js->center.x - js->min.x)) - 1.0f;

	if (y == js->center.y)
		ry = 0;
	else if (y >= js->center.y)
		ry = ((float)(y - js->center.y) / (float)(js->max.y - js->center.y));
	else
		ry = ((float)(y - js->min.y) / (float)(js->center.y - js->min.y)) - 1.0f;

	/* calculate the joystick angle and magnitude */
	ang = RAD_TO_DEGREE(atanf(ry / rx));
	ang -= 90.0f;
	if (rx < 0.0f)
		ang -= 180.0f;
	js->ang = absf(ang);
	js->mag = (float) sqrt((rx * rx) + (ry * ry));
}

/**
 *
 * @brief Apply a smooth factor to accelerometer angles.
 *
 * @param accel		Last acceleration measured and normalized to +/-g.
 * @param gforce	[out] gravity vector after smoothing
 */
void apply_smoothing(struct gforce_t* gforce, float alpha) {
	
	gforce->x = alpha*gforce->x + (1.0-alpha)*gforce->ax;
	gforce->y = alpha*gforce->y + (1.0-alpha)*gforce->ay;
	gforce->z = alpha*gforce->z + (1.0-alpha)*gforce->az;
}
