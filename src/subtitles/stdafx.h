/*
 *	Copyright (C) 2003-2006 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define WINVER			0x0600

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components

// TODO: reference additional headers your program requires here

#include <cstdint>

#include <atlbase.h>
#include <atlconv.h>
#include <atlcoll.h>
#include <atlstr.h>
#include <atltypes.h>
#include <windowsx.h>

// typedef struct tagRECT
// {
//     int32_t    left;
//     int32_t    top;
//     int32_t    right;
//     int32_t    bottom;
// } RECT, *PRECT;

// typedef struct tagPOINT
// {
//     int32_t  x;
//     int32_t  y;
// } POINT, *PPOINT;

// typedef struct tagSIZE
// {
//     int32_t        cx;
//     int32_t        cy;
// } SIZE, *PSIZE, *LPSIZE;

// typedef int POSITION;
// typedef unsigned char BYTE;
