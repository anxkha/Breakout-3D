// ----------------------------------------------------------------------------
//  Filename: text.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

class CText
{
protected:
	ID3DXFont*			m_pFont;

public:
	CText();
	virtual ~CText();

	HRESULT	Init( IDirect3DDevice9* pDevice );
	HRESULT	Release();

	VOID	Print( int x, int y, DWORD color, string sText );
};
