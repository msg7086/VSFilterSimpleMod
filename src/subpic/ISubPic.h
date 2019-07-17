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

#include <memory>
#include <atlbase.h>
#include <atlcoll.h>
#include "CoordGeom.h"

#pragma pack(push, 1)
struct SubPicDesc
{
	int type;
	int w, h, bpp, pitch, pitchUV;
	void* bits;
	BYTE* bitsU;
	BYTE* bitsV;
	RECT vidrect; // video rectangle

	struct SubPicDesc() {type = 0; w = h = bpp = pitch = pitchUV = 0; bits = NULL; bitsU = bitsV = NULL;}
};
#pragma pack(pop)

//
// ISubPic
//

class ISubPicImpl
{
protected:
	REFERENCE_TIME m_rtStart, m_rtStop;
	REFERENCE_TIME m_rtSegmentStart, m_rtSegmentStop;
	CRect	m_rcDirty;
	CSize	m_maxsize;
	CSize	m_size;
	CRect	m_vidrect;
	CSize	m_VirtualTextureSize;
	CPoint	m_VirtualTextureTopLeft;

/*

                          Texture
		+-------+---------------------------------+ 
		|       .                                 |   . 
		|       .             m_maxsize           |       .
 TextureTopLeft .<=============================== |======>    .           Video
		| . . . +-------------------------------- | -----+       +-----------------------------------+
		|       |                         .       |      |       |  m_vidrect                        |
		|       |                         .       |      |       |                                   |
		|       |                         .       |      |       |                                   |
		|       |        +-----------+    .       |      |       |                                   |
		|       |        | m_rcDirty |    .       |      |       |                                   |
		|       |        |           |    .       |      |       |                                   |
		|       |        +-----------+    .       |      |       |                                   |
		|       +-------------------------------- | -----+       |                                   |
		|                    m_size               |              |                                   |
		|       <=========================>       |              |                                   |
		|                                         |              |                                   |
		|                                         |              +-----------------------------------+
		|                                         |          . 
		|                                         |      .     
		|                                         |   .   
		+-----------------------------------------+  
                   m_VirtualTextureSize 
        <=========================================>

*/


public:
	ISubPicImpl();

	// ISubPic

	STDMETHODIMP_(REFERENCE_TIME) GetStart();
	STDMETHODIMP_(REFERENCE_TIME) GetStop();
	STDMETHODIMP_(void) SetStart(REFERENCE_TIME rtStart);
	STDMETHODIMP_(void) SetStop(REFERENCE_TIME rtStop);

	STDMETHODIMP GetDesc(SubPicDesc& spd);

	virtual STDMETHODIMP ClearDirtyRect(DWORD color) = 0;
	STDMETHODIMP GetDirtyRect(RECT* pDirtyRect);

	STDMETHODIMP GetMaxSize(SIZE* pMaxSize);
	STDMETHODIMP SetSize(SIZE size, RECT vidrect);

	virtual STDMETHODIMP Lock(SubPicDesc& spd) = 0;
	virtual STDMETHODIMP Unlock(RECT* pDirtyRect) = 0;

	virtual STDMETHODIMP AlphaBlt(RECT* pSrc, RECT* pDst, SubPicDesc* pTarget) = 0;

	STDMETHODIMP SetVirtualTextureSize (const SIZE pSize, const POINT pTopLeft);
	STDMETHODIMP GetSourceAndDest(SIZE* pSize, RECT* pRcSource, RECT* pRcDest);

	STDMETHODIMP_(REFERENCE_TIME) GetSegmentStart();
	STDMETHODIMP_(REFERENCE_TIME) GetSegmentStop();
	STDMETHODIMP_(void) SetSegmentStart(REFERENCE_TIME rtStart);
	STDMETHODIMP_(void) SetSegmentStop(REFERENCE_TIME rtStop);

};

//
// ISubPicAllocator
//

class ISubPicAllocatorImpl
{
	std::shared_ptr<ISubPicImpl> m_pStatic;

private:
	CSize m_cursize;
	CRect m_curvidrect;

	virtual bool Alloc(bool fStatic, std::shared_ptr<ISubPicImpl>& ppSubPic) = 0;

public:
	ISubPicAllocatorImpl(SIZE cursize);

	// ISubPicAllocator

	STDMETHODIMP SetCurSize(SIZE cursize);
	STDMETHODIMP SetCurVidRect(RECT curvidrect);
	STDMETHODIMP GetStatic(ISubPicImpl** ppSubPic);
	STDMETHODIMP AllocDynamic(std::shared_ptr<ISubPicImpl>& ppSubPic);
	STDMETHODIMP SetMaxTextureSize(SIZE MaxTextureSize) { return E_NOTIMPL; };
};

//
// ISubPicProvider
//

[uuid("D62B9A1A-879A-42db-AB04-88AA8F243CFD")]
interface ISubPicProvider
{
	STDMETHOD (Lock) () PURE;
	STDMETHOD (Unlock) () PURE;

	STDMETHOD_(POSITION, GetStartPosition) (REFERENCE_TIME rt) PURE;
	STDMETHOD_(POSITION, GetNext) (POSITION pos) PURE;

	STDMETHOD_(REFERENCE_TIME, GetStart) (POSITION pos) PURE;
	STDMETHOD_(REFERENCE_TIME, GetStop) (POSITION pos) PURE;

	STDMETHOD (Render) (SubPicDesc& spd, REFERENCE_TIME rt, RECT& bbox) PURE;
	STDMETHOD (GetTextureSize) (POSITION pos, SIZE& MaxTextureSize, SIZE& VirtualSize, POINT& VirtualTopLeft) PURE;
};

class ISubPicProviderImpl : public ISubPicProvider
{
protected:
	CCritSec* m_pLock;

public:
	ISubPicProviderImpl(CCritSec* pLock);
	virtual ~ISubPicProviderImpl();

	// ISubPicProvider

	STDMETHODIMP Lock();
	STDMETHODIMP Unlock();

	STDMETHODIMP_(POSITION) GetStartPosition(REFERENCE_TIME rt) = 0;
	STDMETHODIMP_(POSITION) GetNext(POSITION pos) = 0;

	STDMETHODIMP_(REFERENCE_TIME) GetStart(POSITION pos) = 0;
	STDMETHODIMP_(REFERENCE_TIME) GetStop(POSITION pos) = 0;

	STDMETHODIMP Render(SubPicDesc& spd, REFERENCE_TIME rt, RECT& bbox) = 0;
	STDMETHODIMP GetTextureSize (POSITION pos, SIZE& MaxTextureSize, SIZE& VirtualSize, POINT& VirtualTopLeft) { return E_NOTIMPL; };
};

//
// ISubPicQueue
//

class CSubPicQueueNoThread
{
	CCritSec m_csSubPicProvider;
	std::shared_ptr<ISubPicProvider> m_pSubPicProvider;
	CCritSec m_csLock;
	std::shared_ptr<ISubPicImpl> m_pSubPic;

protected:
	REFERENCE_TIME m_rtNow;
	REFERENCE_TIME m_rtNowLast;

	std::shared_ptr<ISubPicAllocatorImpl> m_pAllocator;

	HRESULT RenderTo(std::shared_ptr<ISubPicImpl>& pSubPic, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop);

public:
	CSubPicQueueNoThread(std::shared_ptr<ISubPicAllocatorImpl>& pAllocator, HRESULT* phr);
	virtual ~CSubPicQueueNoThread();

	// ISubPicQueue

	STDMETHODIMP SetSubPicProvider(const std::shared_ptr<ISubPicProvider>& pSubPicProvider);
	STDMETHODIMP GetSubPicProvider(std::shared_ptr<ISubPicProvider>& pSubPicProvider);

	STDMETHODIMP SetTime(REFERENCE_TIME rtNow);

	STDMETHODIMP Invalidate(REFERENCE_TIME rtInvalidate = -1);
	STDMETHODIMP_(bool) LookupSubPic(REFERENCE_TIME rtNow, std::shared_ptr<ISubPicImpl> &pSubPic);
};

