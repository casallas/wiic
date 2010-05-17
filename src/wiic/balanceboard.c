/*
 *	balanceboard.c
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
#include "balanceboard.h"

/**
 *	@brief Handle the handshake data from the balance board.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 *
 *	@return	Returns 1 if handshake was successful, 0 if not.
 */
int balance_board_handshake(struct wiimote_t* wm, struct balance_board_t* bb, byte* data, unsigned short len) 
{
	/* handshake done */
	wm->exp.type = EXP_BALANCE_BOARD;

	return 1;
}

/**
 *	@brief Handle balance board event.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 *	@param msg		The message specified in the event packet.
 */
void balance_board_event(struct balance_board_t* bb, byte* msg) 
{
	printf("balance board event\n");
}

/**
 *	@brief The balance board disconnected.
 *
 *	@param bb		A pointer to a balance_board_t structure.
 */
void balance_board_disconnected(struct balance_board_t* bb)
{
	memset(bb, 0, sizeof(struct balance_board_t));
}
