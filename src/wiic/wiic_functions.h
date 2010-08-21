/*
 *    wiic_functions.h
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
 *
 *	@brief WiiC public functions.
 *
 *	Contains all the public functions, available for third
 *	party applications. This header is included in wiic.h.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* wiiuse.c */
WIIUSE_EXPORT extern const char* wiiuse_version();

WIIUSE_EXPORT extern struct wiimote_t** wiiuse_init(int wiimotes);
WIIUSE_EXPORT extern void wiiuse_disconnected(struct wiimote_t* wm);
WIIUSE_EXPORT extern void wiiuse_cleanup(struct wiimote_t** wm, int wiimotes);
WIIUSE_EXPORT extern void wiiuse_rumble(struct wiimote_t* wm, int status);
WIIUSE_EXPORT extern void wiiuse_toggle_rumble(struct wiimote_t* wm);
WIIUSE_EXPORT extern void wiiuse_set_leds(struct wiimote_t* wm, int leds);
WIIUSE_EXPORT extern void wiiuse_motion_sensing(struct wiimote_t* wm, int status);
WIIUSE_EXPORT extern int wiiuse_read_data(struct wiimote_t* wm, byte* buffer, unsigned int offset, unsigned short len);
WIIUSE_EXPORT extern int wiiuse_write_data(struct wiimote_t* wm, unsigned int addr, byte* data, byte len);
WIIUSE_EXPORT extern void wiiuse_status(struct wiimote_t* wm);
WIIUSE_EXPORT extern struct wiimote_t* wiiuse_get_by_id(struct wiimote_t** wm, int wiimotes, int unid);
WIIUSE_EXPORT extern int wiiuse_set_flags(struct wiimote_t* wm, int enable, int disable);
WIIUSE_EXPORT extern float wiiuse_set_smooth_alpha(struct wiimote_t* wm, float alpha);
WIIUSE_EXPORT extern void wiiuse_set_orient_threshold(struct wiimote_t* wm, float threshold);
WIIUSE_EXPORT extern void wiiuse_resync(struct wiimote_t* wm);
WIIUSE_EXPORT extern void wiiuse_set_accel_threshold(struct wiimote_t* wm, int threshold);

/* connect.c */
WIIUSE_EXPORT extern int wiiuse_find(struct wiimote_t** wm, int max_wiimotes, int timeout);
WIIUSE_EXPORT extern int wiiuse_connect(struct wiimote_t** wm, int wiimotes);
WIIUSE_EXPORT extern void wiiuse_disconnect(struct wiimote_t* wm);

/* events.c */
WIIUSE_EXPORT extern int wiic_poll(struct wiimote_t** wm, int wiimotes);

/* ir.c */
WIIUSE_EXPORT extern void wiiuse_set_ir(struct wiimote_t* wm, int status);
WIIUSE_EXPORT extern void wiiuse_set_ir_vres(struct wiimote_t* wm, unsigned int x, unsigned int y);
WIIUSE_EXPORT extern void wiiuse_set_ir_position(struct wiimote_t* wm, enum ir_position_t pos);
WIIUSE_EXPORT extern void wiiuse_set_aspect_ratio(struct wiimote_t* wm, enum aspect_t aspect);
WIIUSE_EXPORT extern void wiiuse_set_ir_sensitivity(struct wiimote_t* wm, int level);

/* nunchuk.c */
WIIUSE_EXPORT extern void wiiuse_set_nunchuk_orient_threshold(struct wiimote_t* wm, float threshold);
WIIUSE_EXPORT extern void wiiuse_set_nunchuk_accel_threshold(struct wiimote_t* wm, int threshold);

/* speaker.c */
WIIUSE_EXPORT extern void wiic_set_speaker(struct wiimote_t* wm, int status);

/* motionplus.c */
WIIUSE_EXPORT extern void wiic_set_motion_plus(struct wiimote_t* wm, int status);
WIIUSE_EXPORT extern void wiic_calibrate_motion_plus(struct motion_plus_t* mp);
WIIUSE_EXPORT extern void wiiuse_set_mp_threshold(struct wiimote_t* wm, int threshold);

#ifdef __cplusplus
}
#endif
