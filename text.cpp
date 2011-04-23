// ----------------------------------------------------------------------------
//  Filename: text.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// ----------------------------------------------------------------------------
//  Name: CText
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CText::CText()
{
	m_pFont	= NULL;
}




// ----------------------------------------------------------------------------
//  Name: ~CText
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CText::~CText()
{
	Release();
}




// ----------------------------------------------------------------------------
//  Name: Init
//
//  Desc: Initialize the text drawing engine.
// ----------------------------------------------------------------------------
HRESULT CText::Init( IDirect3DDevice9* pDevice )
{
	HRESULT	hr;
	HDC		hDC = GetDC( NULL );
    int		nLogPixelsY = GetDeviceCaps( hDC, LOGPIXELSY );
	int		nHeight = -12 * nLogPixelsY / 72;

	// Attempt to create the font for drawing text.
    hr = D3DXCreateFont( pDevice,             // D3D device
                         nHeight,               // Height
                         0,                     // Width
                         FW_BOLD,               // Weight
                         1,                     // MipLevels, 0 = autogen mipmaps
                         FALSE,                 // Italic
                         DEFAULT_CHARSET,       // CharSet
                         OUT_DEFAULT_PRECIS,    // OutputPrecision
                         DEFAULT_QUALITY,       // Quality
                         DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
                         "Verdana",              // pFaceName
                         &m_pFont );              // ppFont
    if( FAILED( hr ) ) return hr;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Release
//
//  Desc: Releases all memory used.
// ----------------------------------------------------------------------------
HRESULT CText::Release()
{
	if( m_pFont ) m_pFont->Release();

	m_pFont	= NULL;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Print
//
//  Desc: Draw text to the device buffer.
// ----------------------------------------------------------------------------
VOID CText::Print( int x, int y, DWORD color, string sText )
{
	RECT rc;

	SetRect( &rc, x, y, 0, 0 );

	if( !m_pFont->DrawText( NULL, sText.c_str(), -1, &rc, DT_NOCLIP, D3DXCOLOR(color) ) )
	{
		DbgPrint( "Failed to draw text: " + sText );
	}
}
