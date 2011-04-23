// ----------------------------------------------------------------------------
//  Filename: main.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// Global declarations.
HWND		g_hWnd;
HINSTANCE	g_hInstance;

CGame*		g_pGame;




// ----------------------------------------------------------------------------
//  Name: MsgProc
//
//  Desc: Message handler function. Passes off to the game message handler.
// ----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_pGame )
	{
		return g_pGame->MsgProc( hWnd, msg, wParam, lParam );
	}
	else
	{
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
}




// ----------------------------------------------------------------------------
//  Name: WinMain
//
//  Desc: Application entry. Handles window creation and general
//        initialization.
// ----------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowParam )
{
	WNDCLASSEX wcex;

	DbgOpen( "debug.txt" );

	if( hPrevInstance )
	{
		DbgPrint( "It is not recommended to run more than one instance of this game at a time. Do this at your own risk." );
		MessageBox( NULL, "It is not recommended to run more than one instance of this game at a time. Do this at your own risk.", GAME_TITLE, MB_OK );
	}

	g_hInstance = hInstance;

	// Create the window class.
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbClsExtra = 0L;
	wcex.cbWndExtra = 0L;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = NULL;
	wcex.hCursor = NULL;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hInstance = g_hInstance;
	wcex.lpfnWndProc = MsgProc;
	wcex.lpszClassName = GAME_TITLE;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_CLASSDC;

	// Register the window class.
	RegisterClassEx( &wcex );

	// Create the window. This should NOT fail...
	g_hWnd = CreateWindowEx( NULL, GAME_TITLE, GAME_TITLE, (WS_POPUP | WS_VISIBLE), 0, 0, 800, 600, GetDesktopWindow(), NULL, g_hInstance, NULL );
	if( !g_hWnd )
	{
		DbgPrint( "Failed to create the game window. Something is likely wrong with your windows installation or you need to reboot." );
		MessageBox( NULL, "Failed to create the game window. Something is likely wrong with your windows installation or you need to reboot.", GAME_TITLE, MB_OK );
		return -1;
	}

	// Create a new game instance.
	g_pGame = new CGame();

	// Attempt to initialize the game.
	if( FAILED( g_pGame->Init( g_hWnd, g_hInstance, 1920, 1080 ) ) )
	{
		delete g_pGame;

		DbgPrint( "One or more parts of the game could not be initialized. Please check your installation and try again." );
		MessageBox( NULL, "One or more parts of the game could not be initialized. Please check your installation and try again.", GAME_TITLE, MB_OK );

		DbgClose();

		return -2;
	}

	// Start the actual game.
	g_pGame->Run();

	// We are all done.
	delete g_pGame;

	DbgClose();

	return 0L;
}




// ----------------------------------------------------------------------------
//  Name: TrueRandNumb
//
//  Desc: Statistically, the C++ standard library rand() function does not
//        return a truly random number. This function uses a different
//        procedure to generate a truly random number (or as close as it can
//        possibly get).
// ----------------------------------------------------------------------------
DWORD TrueRandNum( int nLowBound, int nHighBound )
{
		MMTIME now;
		int i;
		DWORD seed = 0;
		PUCHAR p;

		// Get the system time.
		timeGetSystemTime( &now, sizeof(MMTIME) );

		p = (PUCHAR)&now.u.ms;

		// Create a hash of the system time for the random seed.
		for( i = 0; i < sizeof(now.u.ms); i++ ) seed = seed * (UCHAR_MAX + 2U) + p[i];

		// Seed the random number generator.
		srand( seed );

		// This fixes the lower/upper bound issue with rand().
		return nLowBound + (rand() * (1.0 / (RAND_MAX + 1.0))) * nHighBound;
}
