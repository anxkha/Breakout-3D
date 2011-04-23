// ----------------------------------------------------------------------------
//  Filename: graphics.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

class CGraphics
{
protected:
	IDirect3D9*			m_pD3D;
	IDirect3DDevice9*	m_pD3DDevice;

	D3DPRESENT_PARAMETERS	m_tD3DPP;

public:
	CGraphics();
	virtual ~CGraphics();

	HRESULT	Init( HWND hWnd, DWORD nWidth, DWORD nHeight );
	void	Destroy();

	IDirect3DDevice9*	GetDevice();
};
