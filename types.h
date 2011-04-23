// ----------------------------------------------------------------------------
//  Filename: types.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

#include <d3dx9.h>

#define D3DFVF_VERTEX		D3DFVF_XYZ | D3DFVF_DIFFUSE
#define D3DFVF_LVERTEX		D3DFVF_XYZ | D3DFVF_NORMAL
#define D3DFVF_TVERTEX		D3DFVF_XYZ | D3DFVF_TEX1
#define D3DFVF_TLVERTEX		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
#define D3DFVF_TVERTEX2D	D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE
#define D3DFVF_VERTEX2D		D3DFVF_XYZRHW | D3DFVF_DIFFUSE

// A textured, 2D vertex. RHW should always be 1.0f. z should be something between 0.0f and 1.0f.
struct TVERTEX2D
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT tu, tv;
};

// Textured vertex with a light normal component. Materials required for color.
struct TLVERTEX
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT tu, tv;
};

// Plain and simple, colored, 2D vertex.
struct VERTEX2D
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

enum GameState
{
	InitScreen = 1,
	TitleScreen,
	GameScreen,
	ExitingScreen
};
