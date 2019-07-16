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

#include "stdafx.h"
#include <Vfw.h>
#include <winddk/devioctl.h>
#include <winddk/ntddcdrm.h>
#include "DSUtil.h"
#include <moreuuids.h>

#include <initguid.h>
#include <d3dx9.h>
#include <dxva.h>
#include <dxva2api.h>

#include <initguid.h>

DEFINE_GUID(CLSID_ReClock,
            0x9dc15360, 0x914c, 0x46b8, 0xb9, 0xdf, 0xbf, 0xe6, 0x7f, 0xd3, 0x6c, 0x6a);

void memsetd(void* dst, unsigned int c, int nbytes)
{
    for(int i = 0; i < nbytes / sizeof(DWORD); i++)
        ((DWORD*)dst)[i] = c;
}

BOOL CFileGetStatus(LPCTSTR lpszFileName, CFileStatus& status)
{
    try
    {
        return CFile::GetStatus(lpszFileName, status);
    }
    catch(CException* e)
    {
        // MFCBUG: E_INVALIDARG / "Parameter is incorrect" is thrown for certain cds (vs2003)
        // http://groups.google.co.uk/groups?hl=en&lr=&ie=UTF-8&threadm=OZuXYRzWDHA.536%40TK2MSFTNGP10.phx.gbl&rnum=1&prev=/groups%3Fhl%3Den%26lr%3D%26ie%3DISO-8859-1
        TRACE(_T("CFile::GetStatus has thrown an exception\n"));
        e->Delete();
        return false;
    }
}
