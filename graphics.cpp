// ----------------------------------------------------------------------------
//  Filename: graphics.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// ----------------------------------------------------------------------------
//  Name: CGraphics
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CGraphics::CGraphics()
{
	m_pD3D			= NULL;
	m_pD3DDevice	= NULL;
}




// ----------------------------------------------------------------------------
//  Name: ~CGraphics
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CGraphics::~CGraphics()
{
	Destroy();
}




// ----------------------------------------------------------------------------
//  Name: Init
//
//  Desc: Attempts to initializes the graphics subsystem.
// ----------------------------------------------------------------------------
HRESULT CGraphics::Init( HWND hWnd, DWORD nWidth, DWORD nHeight )
{
	HRESULT hr;

	// Create an IDirect3D9 object for later use.
	if( !m_pD3D )
	{
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if( !m_pD3D ) return E_FAIL;
	}

	// Set up the create parameters according to what we were told.
	ZeroMemory( &m_tD3DPP, sizeof(D3DPRESENT_PARAMETERS) );

	// Z-Buffer bit depth. Enable it.
	m_tD3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	m_tD3DPP.EnableAutoDepthStencil = TRUE;

	// Create a backbuffer. This will also define our full-screen resolution.
	m_tD3DPP.BackBufferCount = 1;
	m_tD3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_tD3DPP.BackBufferWidth = nWidth;
	m_tD3DPP.BackBufferHeight = nHeight;

	// Miscellaneous options.
	m_tD3DPP.hDeviceWindow = hWnd;
	m_tD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_tD3DPP.Windowed = FALSE;

	// Destroy previously created device (if any) to prepare for the new one.
	if( m_pD3DDevice )
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Attempt to create a device with the given parameters.
	// Hardware vertex processing does not work on every graphics card.
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_tD3DPP, &m_pD3DDevice );
	if( FAILED( hr ) )
	{
		DbgPrint( "Failed to create a Direct3D 9 device." );
		return hr;
	}

	// We succeeded. Set up a basic environment. Z-Buffer on, lighting off
	// for now, cull counter-clockwise facing primitives.
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );

	// Turn on alpha blending, this is just the basic parameters necessary
	// for transparency.
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Destroy
//
//  Desc: Frees resources used by the graphics subsystem.
// ----------------------------------------------------------------------------
void CGraphics::Destroy()
{
	// Release the devices.
	if( m_pD3DDevice ) m_pD3DDevice->Release();
	if( m_pD3D ) m_pD3D->Release();

	m_pD3DDevice = NULL;
	m_pD3D = NULL;
}




// ----------------------------------------------------------------------------
//  Name: GetDevice
//
//  Desc: Returns the Direct3D device object for use elsewhere.
// ----------------------------------------------------------------------------
IDirect3DDevice9* CGraphics::GetDevice()
{
	return m_pD3DDevice;
}
