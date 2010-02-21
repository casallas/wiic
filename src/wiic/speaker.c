/*
 *	speaker.c
 *
 *	Written By:
 *		Gabriele Randelli	
 *		Email: < randelli (--AT--) dis [--DOT--] uniroma1 [--DOT--] it >
 *
 *	Copyright 2010
 *
 *	This file is part of wiiC.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Handles the Wiimote speakers.
 */
#include "speaker.h"

void wiic_set_speaker(struct wiimote_t* wm, int status)
{
	if(!wm || !WIIMOTE_IS_CONNECTED(wm) || !WIIMOTE_IS_SET(wm, WIIMOTE_STATE_HANDSHAKE_COMPLETE)) {
		WIIUSE_ERROR("Tried to enable the speakers, but the Wiimote is not connected or the handshaking is still pending.");
		return;
	}

	if(status) {
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_SPEAKER);
		printf("credo che lo sto abilitando\n");	
	}
	else
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_SPEAKER);
	
	/* set the wiimote report type */
	wiiuse_set_report_type(wm);
	
	/* wait for the wiimote to catch up */
	#ifndef WIN32
		usleep(50000);
	#else
		Sleep(50);
	#endif

	WIIUSE_DEBUG("Enabled speakers for wiimote id %i.", wm->unid);
}


