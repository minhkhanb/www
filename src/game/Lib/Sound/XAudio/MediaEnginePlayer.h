/* 
    Copyright (c) 2012 Microsoft Corporation.  All rights reserved.
    Use of this sample source code is subject to the terms of the Microsoft license 
    agreement under which you licensed this sample source code and is provided AS-IS.
    If you did not accept the terms of the license agreement, you are not authorized 
    to use this sample source code.  For the terms of the license, please see the 
    license agreement between you and Microsoft.
  
    To see all Code Samples for Windows Phone, visit http://go.microsoft.com/fwlink/?LinkID=219604 
  
*/
#pragma once

//#include <wrl.h>
#include <mfmediaengine.h>
#include <strsafe.h>
#include <mfapi.h>
#include <agile.h>

using namespace std;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;


// MediaEngineNotifyCallback - Defines the callback method to process media engine events.
struct MediaEngineNotifyCallback abstract
{
    virtual void OnMediaEngineEvent(DWORD meEvent) = 0;
};

class MediaEnginePlayer : public MediaEngineNotifyCallback
{
    ComPtr<IMFMediaEngine>           m_spMediaEngine;
    ComPtr<IMFMediaEngineEx>         m_spEngineEx;

    MFARGB                           m_bkgColor;
	int								m_loop;
	BSTR							m_bstrURL;
public:
    MediaEnginePlayer();
    ~MediaEnginePlayer();
	void ThrowIfFailed(HRESULT hr);
    // Media Info
    void GetNativeVideoSize(DWORD *cx, DWORD *cy);
    bool IsPlaying();

    // Initialize/Shutdown
    void Initialize(ComPtr<ID3D11Device> device, DXGI_FORMAT d3dFormat);
    void Shutdown();    

    // Media Engine related
    void OnMediaEngineEvent(DWORD meEvent);

    // Media Engine Actions
    void Play(int loop);//-1 loop, > 0 time, 
    void Pause();
	void Resume();
    void SetMuted(bool muted);

    // Media Source
    void SetSource(Platform::String^ sourceUri);
    void SetSource(const char* sourceUri);
    void SetBytestream(IRandomAccessStream^ streamHandle, Platform::String^ szURL);

    // Transfer Video Frame
    void TransferFrame(ComPtr<ID3D11Texture2D> texture, MFVideoNormalizedRect rect, RECT rcTarget);

private:
    bool m_isPlaying;
};


