/*
 *    wiic.h
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
 *	@brief API header file.
 *
 *	If this file is included from inside the wiiuse source
 *	and not from a third party program, then wiimote_internal.h
 *	is also included which extends this file.
 */
#ifndef WIIC_H_INCLUDED
#define WIIC_H_INCLUDED

#ifdef _WIN32
	/* windows */
	#include <windows.h>
#elif MACOSX
	/* mac */
	#include <CoreFoundation/CoreFoundation.h>
	#include <IOBluetooth/IOBluetoothUserLib.h>
#else
	/* nix */
	#include <bluetooth/bluetooth.h>
#endif

#ifdef WIIC_INTERNAL_H_INCLUDED
	#define WCONST
#else
	#define WCONST		const
#endif

/*
 *	Largest known payload is 21 bytes.
 *	Add 2 for the prefix and round up to a power of 2.
 */
#define MAX_PAYLOAD			32

/*
 *	This is left over from an old hack, but it may actually
 *	be a useful feature to keep so it wasn't removed.
 */
#ifdef WIN32
	#define WIIMOTE_DEFAULT_TIMEOUT		10
	#define WIIMOTE_EXP_TIMEOUT			10
#endif

/*****************************************
 *
 *	Include API specific stuff
 *
 *****************************************/

#ifdef _WIN32
	#define WIIUSE_EXPORT_DECL __declspec(dllexport)
	#define WIIUSE_IMPORT_DECL __declspec(dllimport)
#else
	#define WIIUSE_EXPORT_DECL
	#define WIIUSE_IMPORT_DECL
#endif

#ifdef WIIUSE_COMPILE_LIB
	#define WIIUSE_EXPORT WIIUSE_EXPORT_DECL
#else
	#define WIIUSE_EXPORT WIIUSE_IMPORT_DECL
#endif

#include "wiic_macros.h"
#include "wiic_structs.h"
#include "wiic_functions.h"


#endif /* WIIC_H_INCLUDED */

