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
	int64_t m_rtStart, m_rtStop;
	int64_t m_rtSegmentStart, m_rtSegmentStop;
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

	int64_t GetStart();
	int64_t GetStop();
	void SetStart(int64_t rtStart);
	void SetStop(int64_t rtStop);

	HRESULT GetDesc(SubPicDesc& spd);

	virtual HRESULT ClearDirtyRect(DWORD color) = 0;
	HRESULT GetDirtyRect(RECT* pDirtyRect);

	HRESULT GetMaxSize(SIZE* pMaxSize);
	HRESULT SetSize(SIZE size, RECT vidrect);

	virtual HRESULT Lock(SubPicDesc& spd) = 0;
	virtual HRESULT Unlock(RECT* pDirtyRect) = 0;

	virtual HRESULT AlphaBlt(RECT* pSrc, RECT* pDst, SubPicDesc* pTarget) = 0;

	HRESULT SetVirtualTextureSize (const SIZE pSize, const POINT pTopLeft);
	HRESULT GetSourceAndDest(SIZE* pSize, RECT* pRcSource, RECT* pRcDest);

	int64_t GetSegmentStart();
	int64_t GetSegmentStop();
	void SetSegmentStart(int64_t rtStart);
	void SetSegmentStop(int64_t rtStop);

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

	HRESULT SetCurSize(SIZE cursize);
	HRESULT SetCurVidRect(RECT curvidrect);
	HRESULT GetStatic(ISubPicImpl** ppSubPic);
	HRESULT AllocDynamic(std::shared_ptr<ISubPicImpl>& ppSubPic);
	HRESULT SetMaxTextureSize(SIZE MaxTextureSize) { return E_NOTIMPL; };
};

//
// ISubPicProvider
//

class ISubPicProviderImpl
{
protected:

public:
	ISubPicProviderImpl();
	virtual ~ISubPicProviderImpl();

	// ISubPicProvider

	HRESULT Lock();
	HRESULT Unlock();

	virtual POSITION GetStartPosition(int64_t rt) = 0;
	virtual POSITION GetNext(POSITION pos) = 0;

	virtual int64_t GetStart(POSITION pos) = 0;
	virtual int64_t GetStop(POSITION pos) = 0;

	virtual HRESULT Render(SubPicDesc& spd, int64_t rt, RECT& bbox) = 0;
	virtual HRESULT GetTextureSize (POSITION pos, SIZE& MaxTextureSize, SIZE& VirtualSize, POINT& VirtualTopLeft) { return E_NOTIMPL; };
};

//
// ISubPicQueue
//

class CSubPicQueueNoThread
{
	std::shared_ptr<ISubPicProviderImpl> m_pSubPicProvider;
	std::shared_ptr<ISubPicImpl> m_pSubPic;

protected:
	int64_t m_rtNow;
	int64_t m_rtNowLast;

	std::shared_ptr<ISubPicAllocatorImpl> m_pAllocator;

	HRESULT RenderTo(std::shared_ptr<ISubPicImpl>& pSubPic, int64_t rtStart, int64_t rtStop);

public:
	CSubPicQueueNoThread(std::shared_ptr<ISubPicAllocatorImpl>& pAllocator, HRESULT* phr);
	virtual ~CSubPicQueueNoThread();

	// ISubPicQueue

	HRESULT SetSubPicProvider(const std::shared_ptr<ISubPicProviderImpl>& pSubPicProvider);
	HRESULT GetSubPicProvider(std::shared_ptr<ISubPicProviderImpl>& pSubPicProvider);

	HRESULT SetTime(int64_t rtNow);

	HRESULT Invalidate(int64_t rtInvalidate = -1);
	bool LookupSubPic(int64_t rtNow, std::shared_ptr<ISubPicImpl> &pSubPic);
};

