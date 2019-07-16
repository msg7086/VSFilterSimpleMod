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
#include "ISubPic.h"
#include "..\DSUtil\DSUtil.h"

//
// ISubPicImpl
//

ISubPicImpl::ISubPicImpl()
    : CUnknown(NAME("ISubPicImpl"), NULL)
    , m_rtStart(0), m_rtStop(0)
    , m_rtSegmentStart(0), m_rtSegmentStop(0)
    , m_rcDirty(0, 0, 0, 0), m_maxsize(0, 0), m_size(0, 0), m_vidrect(0, 0, 0, 0)
    , m_VirtualTextureSize(0, 0), m_VirtualTextureTopLeft(0, 0)
{
}

STDMETHODIMP ISubPicImpl::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    return
        QI(ISubPic)
        __super::NonDelegatingQueryInterface(riid, ppv);
}

// ISubPic

STDMETHODIMP_(REFERENCE_TIME) ISubPicImpl::GetStart()
{
    return(m_rtStart);
}

STDMETHODIMP_(REFERENCE_TIME) ISubPicImpl::GetStop()
{
    return(m_rtStop);
}

STDMETHODIMP_(REFERENCE_TIME) ISubPicImpl::GetSegmentStart()
{
    if(m_rtSegmentStart)
        return(m_rtSegmentStart);
    return(m_rtStart);
}

STDMETHODIMP_(REFERENCE_TIME) ISubPicImpl::GetSegmentStop()
{
    if(m_rtSegmentStop)
        return(m_rtSegmentStop);
    return(m_rtStop);
}

STDMETHODIMP_(void) ISubPicImpl::SetSegmentStart(REFERENCE_TIME rtStart)
{
    m_rtSegmentStart = rtStart;
}

STDMETHODIMP_(void) ISubPicImpl::SetSegmentStop(REFERENCE_TIME rtStop)
{
    m_rtSegmentStop = rtStop;
}



STDMETHODIMP_(void) ISubPicImpl::SetStart(REFERENCE_TIME rtStart)
{
    m_rtStart = rtStart;
}

STDMETHODIMP_(void) ISubPicImpl::SetStop(REFERENCE_TIME rtStop)
{
    m_rtStop = rtStop;
}

STDMETHODIMP ISubPicImpl::CopyTo(ISubPic* pSubPic)
{
    if(!pSubPic)
        return E_POINTER;

    pSubPic->SetStart(m_rtStart);
    pSubPic->SetStop(m_rtStop);
    pSubPic->SetSegmentStart(m_rtSegmentStart);
    pSubPic->SetSegmentStop(m_rtSegmentStop);
    pSubPic->SetDirtyRect(m_rcDirty);
    pSubPic->SetSize(m_size, m_vidrect);
    pSubPic->SetVirtualTextureSize(m_VirtualTextureSize, m_VirtualTextureTopLeft);

    return S_OK;
}

STDMETHODIMP ISubPicImpl::GetDirtyRect(RECT* pDirtyRect)
{
    return pDirtyRect ? *pDirtyRect = m_rcDirty, S_OK : E_POINTER;
}

STDMETHODIMP ISubPicImpl::GetSourceAndDest(SIZE* pSize, RECT* pRcSource, RECT* pRcDest)
{
    CheckPointer(pRcSource, E_POINTER);
    CheckPointer(pRcDest,	 E_POINTER);

    if(m_size.cx > 0 && m_size.cy > 0)
    {
        CRect		rcTemp = m_rcDirty;

        // FIXME
        rcTemp.DeflateRect(1, 1);

        *pRcSource = rcTemp;

        rcTemp.OffsetRect(m_VirtualTextureTopLeft);
        *pRcDest = CRect(rcTemp.left   * pSize->cx / m_VirtualTextureSize.cx,
                         rcTemp.top    * pSize->cy / m_VirtualTextureSize.cy,
                         rcTemp.right  * pSize->cx / m_VirtualTextureSize.cx,
                         rcTemp.bottom * pSize->cy / m_VirtualTextureSize.cy);

        return S_OK;
    }
    else
        return E_INVALIDARG;
}

STDMETHODIMP ISubPicImpl::SetDirtyRect(RECT* pDirtyRect)
{
    return pDirtyRect ? m_rcDirty = *pDirtyRect, S_OK : E_POINTER;
}

STDMETHODIMP ISubPicImpl::GetMaxSize(SIZE* pMaxSize)
{
    return pMaxSize ? *pMaxSize = m_maxsize, S_OK : E_POINTER;
}

STDMETHODIMP ISubPicImpl::SetSize(SIZE size, RECT vidrect)
{
    m_size = size;
    m_vidrect = vidrect;

    if(m_size.cx > m_maxsize.cx)
    {
        m_size.cy = MulDiv(m_size.cy, m_maxsize.cx, m_size.cx);
        m_size.cx = m_maxsize.cx;
    }

    if(m_size.cy > m_maxsize.cy)
    {
        m_size.cx = MulDiv(m_size.cx, m_maxsize.cy, m_size.cy);
        m_size.cy = m_maxsize.cy;
    }

    if(m_size.cx != size.cx || m_size.cy != size.cy)
    {
        m_vidrect.top = MulDiv(m_vidrect.top, m_size.cx, size.cx);
        m_vidrect.bottom = MulDiv(m_vidrect.bottom, m_size.cx, size.cx);
        m_vidrect.left = MulDiv(m_vidrect.left, m_size.cy, size.cy);
        m_vidrect.right = MulDiv(m_vidrect.right, m_size.cy, size.cy);
    }
    m_VirtualTextureSize = m_size;

    return S_OK;
}

STDMETHODIMP ISubPicImpl::SetVirtualTextureSize(const SIZE pSize, const POINT pTopLeft)
{
    m_VirtualTextureSize.SetSize(pSize.cx, pSize.cy);
    m_VirtualTextureTopLeft.SetPoint(pTopLeft.x, pTopLeft.y);

    return S_OK;
}

//
// ISubPicAllocatorImpl
//

ISubPicAllocatorImpl::ISubPicAllocatorImpl(SIZE cursize)
    : CUnknown(NAME("ISubPicAllocatorImpl"), NULL)
    , m_cursize(cursize)
{
    m_curvidrect = CRect(CPoint(0, 0), m_cursize);
}

STDMETHODIMP ISubPicAllocatorImpl::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    return
        QI(ISubPicAllocator)
        __super::NonDelegatingQueryInterface(riid, ppv);
}

// ISubPicAllocator

STDMETHODIMP ISubPicAllocatorImpl::SetCurSize(SIZE cursize)
{
    m_cursize = cursize;
    return S_OK;
}

STDMETHODIMP ISubPicAllocatorImpl::SetCurVidRect(RECT curvidrect)
{
    m_curvidrect = curvidrect;
    return S_OK;
}

STDMETHODIMP ISubPicAllocatorImpl::GetStatic(ISubPic** ppSubPic)
{
    if(!ppSubPic)
        return E_POINTER;

    if(!m_pStatic)
    {
        if(!Alloc(true, &m_pStatic) || !m_pStatic)
            return E_OUTOFMEMORY;
    }

    m_pStatic->SetSize(m_cursize, m_curvidrect);

    (*ppSubPic = m_pStatic)->AddRef();

    return S_OK;
}

STDMETHODIMP ISubPicAllocatorImpl::AllocDynamic(ISubPic** ppSubPic)
{
    if(!ppSubPic)
        return E_POINTER;

    if(!Alloc(false, ppSubPic) || !*ppSubPic)
        return E_OUTOFMEMORY;

    (*ppSubPic)->SetSize(m_cursize, m_curvidrect);

    return S_OK;
}

STDMETHODIMP ISubPicAllocatorImpl::ChangeDevice(IUnknown* pDev)
{
    m_pStatic = NULL;
    return S_OK;
}


//
// ISubPicProviderImpl
//

ISubPicProviderImpl::ISubPicProviderImpl(CCritSec* pLock)
    : CUnknown(NAME("ISubPicProviderImpl"), NULL)
    , m_pLock(pLock)
{
}

ISubPicProviderImpl::~ISubPicProviderImpl()
{
}

STDMETHODIMP ISubPicProviderImpl::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    return
        QI(ISubPicProvider)
        __super::NonDelegatingQueryInterface(riid, ppv);
}

// ISubPicProvider

STDMETHODIMP ISubPicProviderImpl::Lock()
{
    return m_pLock ? m_pLock->Lock(), S_OK : E_FAIL;
}

STDMETHODIMP ISubPicProviderImpl::Unlock()
{
    return m_pLock ? m_pLock->Unlock(), S_OK : E_FAIL;
}

//
// ISubPicQueueImpl
//

ISubPicQueueImpl::ISubPicQueueImpl(ISubPicAllocator* pAllocator, HRESULT* phr)
    : CUnknown(NAME("ISubPicQueueImpl"), NULL)
    , m_pAllocator(pAllocator)
    , m_rtNow(0)
    , m_rtNowLast(0)
    , m_fps(25.0)
{
    if(phr) *phr = S_OK;

    if(!m_pAllocator)
    {
        if(phr) *phr = E_FAIL;
        return;
    }
}

ISubPicQueueImpl::~ISubPicQueueImpl()
{
}

STDMETHODIMP ISubPicQueueImpl::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    return
        QI(ISubPicQueue)
        __super::NonDelegatingQueryInterface(riid, ppv);
}

// ISubPicQueue

STDMETHODIMP ISubPicQueueImpl::SetSubPicProvider(ISubPicProvider* pSubPicProvider)
{
    CAutoLock cAutoLock(&m_csSubPicProvider);

//	if(m_pSubPicProvider != pSubPicProvider)
    {
        m_pSubPicProvider = pSubPicProvider;

        Invalidate();
    }

    return S_OK;
}

STDMETHODIMP ISubPicQueueImpl::GetSubPicProvider(ISubPicProvider** pSubPicProvider)
{
    if(!pSubPicProvider)
        return E_POINTER;

    CAutoLock cAutoLock(&m_csSubPicProvider);

    if(m_pSubPicProvider)
    { 
 	   *pSubPicProvider = m_pSubPicProvider; 
 	   (*pSubPicProvider)->AddRef(); 
 	} 

    return !!*pSubPicProvider ? S_OK : E_FAIL;
}

STDMETHODIMP ISubPicQueueImpl::SetFPS(double fps)
{
    m_fps = fps;

    return S_OK;
}

STDMETHODIMP ISubPicQueueImpl::SetTime(REFERENCE_TIME rtNow)
{
    m_rtNow = rtNow;

    return S_OK;
}

// private

HRESULT ISubPicQueueImpl::RenderTo(ISubPic* pSubPic, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop, double fps)
{
    HRESULT hr = E_FAIL;

    if(!pSubPic)
        return hr;

    CComPtr<ISubPicProvider> pSubPicProvider;
    if(FAILED(GetSubPicProvider(&pSubPicProvider)) || !pSubPicProvider)
        return hr;

    if(FAILED(pSubPicProvider->Lock()))
        return hr;

    SubPicDesc spd;
    if(SUCCEEDED(pSubPic->ClearDirtyRect(0xFF000000))
       && SUCCEEDED(pSubPic->Lock(spd)))
    {
        CRect r(0, 0, 0, 0);
        hr = pSubPicProvider->Render(spd, ((rtStart + rtStop) / 2), fps, r);

        pSubPic->SetStart(rtStart);
        pSubPic->SetStop(rtStop);

        pSubPic->Unlock(r);
    }

    pSubPicProvider->Unlock();

    return hr;
}

//
// CSubPicQueueNoThread
//

CSubPicQueueNoThread::CSubPicQueueNoThread(ISubPicAllocator* pAllocator, HRESULT* phr)
    : ISubPicQueueImpl(pAllocator, phr)
{
}

CSubPicQueueNoThread::~CSubPicQueueNoThread()
{
}

// ISubPicQueue

STDMETHODIMP CSubPicQueueNoThread::Invalidate(REFERENCE_TIME rtInvalidate)
{
    CAutoLock cQueueLock(&m_csLock);

    m_pSubPic = NULL;

    return S_OK;
}

STDMETHODIMP_(bool) CSubPicQueueNoThread::LookupSubPic(REFERENCE_TIME rtNow, CComPtr<ISubPic> &ppSubPic)
{

    CComPtr<ISubPic> pSubPic;

    {
        CAutoLock cAutoLock(&m_csLock);

        if(!m_pSubPic)
        {
            if(FAILED(m_pAllocator->AllocDynamic(&m_pSubPic)))
                return(false);
        }

        pSubPic = m_pSubPic;
    }

    if(pSubPic->GetStart() <= rtNow && rtNow < pSubPic->GetStop())
    {
        ppSubPic = pSubPic;
    }
    else
    {
        CComPtr<ISubPicProvider> pSubPicProvider;
        GetSubPicProvider(&pSubPicProvider); 
 	    if (pSubPicProvider && SUCCEEDED(pSubPicProvider->Lock())) 
        {
            double fps = m_fps;

            POSITION pos = pSubPicProvider->GetStartPosition(rtNow, fps); 
 	        if(pos != 0) 
            {
                REFERENCE_TIME rtStart = pSubPicProvider->GetStart(pos, fps);
                REFERENCE_TIME rtStop = pSubPicProvider->GetStop(pos, fps);

                rtStart = rtNow;
                rtStop = rtNow + 1;

                if(rtStart <= rtNow && rtNow < rtStop)
                {
                    SIZE	MaxTextureSize, VirtualSize;
                    POINT	VirtualTopLeft;
                    HRESULT	hr2;
                    if(SUCCEEDED(hr2 = pSubPicProvider->GetTextureSize(pos, MaxTextureSize, VirtualSize, VirtualTopLeft)))
                        m_pAllocator->SetMaxTextureSize(MaxTextureSize);

                    if(SUCCEEDED(RenderTo(m_pSubPic, rtStart, rtStop, fps)))
                        ppSubPic = pSubPic;
                    if(SUCCEEDED(hr2))
                        pSubPic->SetVirtualTextureSize(VirtualSize, VirtualTopLeft);
                }
            }

            pSubPicProvider->Unlock();

            if(ppSubPic)
            {
                CAutoLock cAutoLock(&m_csLock);

                m_pSubPic = ppSubPic;
            }
        }
    }

    return(!!ppSubPic);
}

STDMETHODIMP CSubPicQueueNoThread::GetStats(int& nSubPics, REFERENCE_TIME& rtNow, REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop)
{
    CAutoLock cAutoLock(&m_csLock);

    nSubPics = 0;
    rtNow = m_rtNow;
    rtStart = rtStop = 0;

    if(m_pSubPic)
    {
        nSubPics = 1;
        rtStart = m_pSubPic->GetStart();
        rtStop = m_pSubPic->GetStop();
    }

    return S_OK;
}

STDMETHODIMP CSubPicQueueNoThread::GetStats(int nSubPic, REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop)
{
    CAutoLock cAutoLock(&m_csLock);

    if(!m_pSubPic || nSubPic != 0)
        return E_INVALIDARG;

    rtStart = m_pSubPic->GetStart();
    rtStop = m_pSubPic->GetStop();

    return S_OK;
}

//
// ISubPicAllocatorPresenterImpl
//

ISubPicAllocatorPresenterImpl::ISubPicAllocatorPresenterImpl(HWND hWnd, HRESULT& hr, CString *_pError)
    : CUnknown(NAME("ISubPicAllocatorPresenterImpl"), NULL)
    , m_hWnd(hWnd)
    , m_NativeVideoSize(0, 0), m_AspectRatio(0, 0)
    , m_VideoRect(0, 0, 0, 0), m_WindowRect(0, 0, 0, 0)
    , m_fps(25.0)
    , m_rtSubtitleDelay(0)
    , m_bDeviceResetRequested(false) 
    , m_bPendingResetDevice(false)
{
    if(!IsWindow(m_hWnd))
    {
        hr = E_INVALIDARG;
        if(_pError)
            *_pError += "Invalid window handle in ISubPicAllocatorPresenterImpl\n";
        return;
    }
    GetWindowRect(m_hWnd, &m_WindowRect);
    SetVideoAngle(Vector(), false);
    hr = S_OK;
}

ISubPicAllocatorPresenterImpl::~ISubPicAllocatorPresenterImpl()
{
}

STDMETHODIMP ISubPicAllocatorPresenterImpl::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{

    return
        QI(ISubPicAllocatorPresenter)
        QI(ISubPicAllocatorPresenter2)
        __super::NonDelegatingQueryInterface(riid, ppv);
}

void ISubPicAllocatorPresenterImpl::AlphaBltSubPic(CSize size, SubPicDesc* pTarget)
{
    CComPtr<ISubPic> pSubPic;
    if(m_pSubPicQueue->LookupSubPic(m_rtNow, pSubPic))
    {
        CRect rcSource, rcDest;
        if(SUCCEEDED(pSubPic->GetSourceAndDest(&size, rcSource, rcDest)))
            pSubPic->AlphaBlt(rcSource, rcDest, pTarget);
        /*		SubPicDesc spd;
        		pSubPic->GetDesc(spd);

        		if(spd.w > 0 && spd.h > 0)
        		{
        			CRect r;
        			pSubPic->GetDirtyRect(r);

        			// FIXME
        			r.DeflateRect(1, 1);

        			CRect rDstText(
        				r.left * size.cx / spd.w,
        				r.top * size.cy / spd.h,
        				r.right * size.cx / spd.w,
        				r.bottom * size.cy / spd.h);

        			pSubPic->AlphaBlt(r, rDstText, pTarget);
        		}*/
    }
}

// ISubPicAllocatorPresenter

STDMETHODIMP_(SIZE) ISubPicAllocatorPresenterImpl::GetVideoSize(bool fCorrectAR)
{
    CSize VideoSize(m_NativeVideoSize);

    if(fCorrectAR && m_AspectRatio.cx > 0 && m_AspectRatio.cy > 0)
        VideoSize.cx = (LONGLONG(VideoSize.cy) * LONGLONG(m_AspectRatio.cx)) / LONGLONG(m_AspectRatio.cy);

    return(VideoSize);
}

STDMETHODIMP_(void) ISubPicAllocatorPresenterImpl::SetPosition(RECT w, RECT v)
{
    bool fWindowPosChanged = !!(m_WindowRect != w);
    bool fWindowSizeChanged = !!(m_WindowRect.Size() != CRect(w).Size());

    m_WindowRect = w;

    bool fVideoRectChanged = !!(m_VideoRect != v);

    m_VideoRect = v;

    if(fWindowSizeChanged || fVideoRectChanged)
    {
        if(m_pAllocator)
        {
            m_pAllocator->SetCurSize(m_WindowRect.Size());
            m_pAllocator->SetCurVidRect(m_VideoRect);
        }

        if(m_pSubPicQueue)
        {
            m_pSubPicQueue->Invalidate();
        }
    }

    if(fWindowPosChanged || fVideoRectChanged)
        Paint(fWindowSizeChanged || fVideoRectChanged);
}

STDMETHODIMP_(void) ISubPicAllocatorPresenterImpl::SetTime(REFERENCE_TIME rtNow)
{
    /*
    	if(m_rtNow <= rtNow && rtNow <= m_rtNow + 1000000)
    		return;
    */
    m_rtNow = rtNow - m_rtSubtitleDelay;

    if(m_pSubPicQueue)
    {
        m_pSubPicQueue->SetTime(m_rtNow);
    }
}

STDMETHODIMP_(void) ISubPicAllocatorPresenterImpl::SetSubtitleDelay(int delay_ms)
{
    m_rtSubtitleDelay = delay_ms * 10000i64;
}

STDMETHODIMP_(int) ISubPicAllocatorPresenterImpl::GetSubtitleDelay()
{
    return (m_rtSubtitleDelay / 10000);
}

STDMETHODIMP_(double) ISubPicAllocatorPresenterImpl::GetFPS()
{
    return(m_fps);
}

STDMETHODIMP_(void) ISubPicAllocatorPresenterImpl::SetSubPicProvider(ISubPicProvider* pSubPicProvider)
{
    m_SubPicProvider = pSubPicProvider;

    if(m_pSubPicQueue)
        m_pSubPicQueue->SetSubPicProvider(pSubPicProvider);
}

STDMETHODIMP_(void) ISubPicAllocatorPresenterImpl::Invalidate(REFERENCE_TIME rtInvalidate)
{
    if(m_pSubPicQueue)
        m_pSubPicQueue->Invalidate(rtInvalidate);
}

#include <math.h>

void ISubPicAllocatorPresenterImpl::Transform(CRect r, Vector v[4])
{
    v[0] = Vector(r.left, r.top, 0);
    v[1] = Vector(r.right, r.top, 0);
    v[2] = Vector(r.left, r.bottom, 0);
    v[3] = Vector(r.right, r.bottom, 0);

    Vector center(r.CenterPoint().x, r.CenterPoint().y, 0);
    int l = (int)(Vector(r.Size().cx, r.Size().cy, 0).Length() * 1.5f) + 1;

    for(ptrdiff_t i = 0; i < 4; i++)
    {
        v[i] = m_xform << (v[i] - center);
        v[i].z = v[i].z / l + 0.5f;
        v[i].x /= v[i].z * 2;
        v[i].y /= v[i].z * 2;
        v[i] += center;
    }
}

STDMETHODIMP ISubPicAllocatorPresenterImpl::SetVideoAngle(Vector v, bool fRepaint)
{
    m_xform = XForm(Ray(Vector(0, 0, 0), v), Vector(1, 1, 1), false);
    if(fRepaint) Paint(true);
    return S_OK;
}
