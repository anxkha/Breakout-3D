// ----------------------------------------------------------------------------
//  Filename: input.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

#include <dinput.h>

#define INPUT_CREATE_KEYBOARD	0x01
#define INPUT_CREATE_MOUSE		0x02

class CInput
{
protected:
	IDirectInput8* m_pDI;

	IDirectInputDevice8* m_pDIKDev;
	IDirectInputDevice8* m_pDIMDev;

	BOOL m_bKeyboardInitialized;
	BOOL m_bMouseInitialized;

public:
	CInput();
	virtual ~CInput();

	HRESULT	Init( HINSTANCE hInst, HWND hWnd, DWORD dwCreateFlags );
	HRESULT	Restore();
	HRESULT	InvalidateDeviceObjects();
	void	Release();

	VOID GetMouse( BOOL *l, BOOL *r, FLOAT *x, FLOAT *y );
	BOOL KeyDown( UCHAR key );
};
