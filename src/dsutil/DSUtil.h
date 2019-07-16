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

#pragma once
/*
#ifdef UNICODE
#ifdef DEBUG
#pragma comment(lib, "dsutilDU")
#else
#pragma comment(lib, "dsutilRU")
#endif
#else
#ifdef DEBUG
#pragma comment(lib, "dsutilD")
#else
#pragma comment(lib, "dsutilR")
#endif
#endif */

//#include "MediaTypes.h"
#include "text.h"

#define LCID_NOSUBTITLES			-1

extern LCID    ISO6391ToLcid(LPCSTR code);
extern LCID    ISO6392ToLcid(LPCSTR code);
extern CString ISO6391To6392(LPCSTR code);
extern CString ISO6392To6391(LPCSTR code);
extern BOOL CFileGetStatus(LPCTSTR lpszFileName, CFileStatus& status);

#define QI(i) (riid == __uuidof(i)) ? GetInterface((i*)this, ppv) :
#define QI2(i) (riid == IID_##i) ? GetInterface((i*)this, ppv) :

#define countof(array) (sizeof(array)/sizeof(array[0]))

template <class T>
static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT* phr)
{
    *phr = S_OK;
    CUnknown* punk = DNew T(lpunk, phr);
    if(punk == NULL) *phr = E_OUTOFMEMORY;
    return punk;
}

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
