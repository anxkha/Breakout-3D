// ----------------------------------------------------------------------------
//  Filename: game.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

class CGame
{
protected:
	IDirect3DDevice9*	m_pDevice;
	IDirect3DTexture9*	m_pBackground;
	IDirect3DTexture9*	m_pBoard;

	D3DLIGHT9	m_Light1;

	CGraphics*	m_pGraphics;
	CCamera*	m_pCamera;
	CInput*		m_pInput;
	CText*		m_pText;

	GameState	m_PreviousState;
	GameState	m_CurrentState;
	GameState	m_NextState;

	HWND		m_hWnd;
	HINSTANCE	m_hInstance;

	DWORD		m_dwWinWidth;
	DWORD		m_dwWinHeight;

	FLOAT		m_dwMouseX;
	FLOAT		m_dwMouseY;

	FLOAT		m_fOldTime;
	FLOAT		m_fNewTime;
	FLOAT		m_fDeltaTime;

	CObject*	m_pRedBrick;
	CObject*	m_pBlueBrick;
	CObject*	m_pGreenBrick;
	CObject*	m_pBall;
	CObject*	m_pPaddle;

	BOOL		m_bEscape;
	BOOL		m_bMouseL;
	BOOL		m_bMouseR;

protected:
	BOOL	m_bStartGameSelected;
	BOOL	m_bExitGameSelected;

protected:
	CHAR		m_tMap[105];

	DWORD		m_dwTotalBricks;

	D3DVECTOR	m_vPaddlePos;
	D3DVECTOR	m_vBallPos;
	D3DVECTOR	m_vBallVel;

	FLOAT		m_fBallRadius;

	DWORD		m_dwBallTimer;

	DWORD		m_dwScore;

	DWORD		m_dwOldFPS;
	DWORD		m_dwNewFPS;
	FLOAT		m_fSecondCount;

public:
	CGame();
	virtual ~CGame();

	LRESULT		WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	HRESULT		Init( HWND hWnd, HINSTANCE hInstance, DWORD nWidth, DWORD nHeight );
	void		Destroy();

	void		Run();
	HRESULT		Update( FLOAT fElapsedTime );
	HRESULT		Render();

	HRESULT		InitTitleScreen();
	GameState	UpdateTitleScreen( FLOAT fElapsedTime );
	HRESULT		RenderTitleScreen();

	HRESULT		InitGameScreen();
	GameState	UpdateGameScreen( FLOAT fElapsedTime );
	HRESULT		RenderGameScreen();

	VOID		CheckForCollisions( FLOAT fElapsedTime );

	VOID		RenderMouse();
	VOID		RenderBackground();
	VOID		RenderBoard();
	VOID		RenderScore();

	D3DVECTOR	VecNormalize( D3DVECTOR v );
	FLOAT		VecDot( D3DVECTOR v1, D3DVECTOR v2 );
};
