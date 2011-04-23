// ----------------------------------------------------------------------------
//  Filename: game.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// ----------------------------------------------------------------------------
//  Name: CGame
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CGame::CGame()
{
	m_pGraphics		= NULL;
	m_pCamera		= NULL;
	m_pBackground	= NULL;
	m_pBoard		= NULL;
	m_pText			= NULL;
	m_pRedBrick		= NULL;
	m_pBlueBrick	= NULL;
	m_pGreenBrick	= NULL;
	m_pBall			= NULL;
	m_pPaddle		= NULL;
	m_hWnd			= NULL;
	m_hInstance		= NULL;
}




// ----------------------------------------------------------------------------
//  Name: ~CGame
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CGame::~CGame()
{
	Destroy();
}




// ----------------------------------------------------------------------------
//  Name: Init
//
//  Desc: Initializes the game.
// ----------------------------------------------------------------------------
HRESULT CGame::Init( HWND hWnd, HINSTANCE hInstance, DWORD nWidth, DWORD nHeight )
{
	HRESULT hr;
	int i;
	char	sBackground[255];

	m_hWnd = hWnd;
	m_hInstance = hInstance;

	m_dwWinWidth = nWidth;
	m_dwWinHeight = nHeight;


	// Create a new graphics object.
	m_pGraphics = new CGraphics();
	if( !m_pGraphics ) return E_OUTOFMEMORY;

	// Create a new camera object.
	m_pCamera = new CCamera();
	if( !m_pCamera ) return E_OUTOFMEMORY;

	// Create a new input object.
	m_pInput = new CInput();
	if( !m_pInput ) return E_OUTOFMEMORY;

	// Create a new text object.
	m_pText = new CText();
	if( !m_pText ) return E_OUTOFMEMORY;

	// Create the game objects.
	m_pRedBrick = new CObject();
	if( !m_pRedBrick ) return E_OUTOFMEMORY;

	m_pBlueBrick = new CObject();
	if( !m_pBlueBrick ) return E_OUTOFMEMORY;

	m_pGreenBrick = new CObject();
	if( !m_pGreenBrick ) return E_OUTOFMEMORY;

	m_pBall = new CObject();
	if( !m_pBall ) return E_OUTOFMEMORY;

	m_pPaddle = new CObject();
	if( !m_pPaddle ) return E_OUTOFMEMORY;

	// Init the graphics system.
	hr = m_pGraphics->Init( hWnd, nWidth, nHeight );
	if( FAILED( hr ) ) return hr;

	ShowCursor( FALSE );

	m_pDevice = m_pGraphics->GetDevice();

	// Init the camera.
	m_pCamera->Initialize();

	m_pDevice->SetTransform( D3DTS_PROJECTION, &m_pCamera->GetProjectionMatrix( (FLOAT)nWidth, (FLOAT)nHeight ) );

	// Init the input system.
	hr = m_pInput->Init( m_hInstance, m_hWnd, INPUT_CREATE_KEYBOARD | INPUT_CREATE_MOUSE );
	if( FAILED( hr ) ) return hr;

	m_pInput->Restore();

	// Init the text system.
	hr = m_pText->Init( m_pDevice );
	if( FAILED( hr ) ) return hr;

	// Load the object models.
	hr = m_pRedBrick->LoadX( m_pDevice, "Data\\Models\\RedBrick\\", "redbrick.x" );
	if( FAILED( hr ) ) return hr;

	hr = m_pBlueBrick->LoadX( m_pDevice, "Data\\Models\\BlueBrick\\", "bluebrick.x" );
	if( FAILED( hr ) ) return hr;

	hr = m_pGreenBrick->LoadX( m_pDevice, "Data\\Models\\GreenBrick\\", "greenbrick.x" );
	if( FAILED( hr ) ) return hr;

	hr = m_pBall->LoadX( m_pDevice, "Data\\Models\\Ball\\", "ball.x" );
	if( FAILED( hr ) ) return hr;

	hr = m_pPaddle->LoadX( m_pDevice, "Data\\Models\\Paddle\\", "paddle.x" );
	if( FAILED( hr ) ) return hr;

	// Randomly pick a backdrop to use.
	i = TrueRandNum( 1, 10 );

	sprintf( sBackground, "Data\\Images\\%d.jpg", i );

	DbgPrint( sBackground );

	// Load the background texture.
	hr = D3DXCreateTextureFromFile( m_pDevice, sBackground, &m_pBackground );
	if( FAILED( hr ) ) return hr;

	// Load the board texture.
	hr = D3DXCreateTextureFromFile( m_pDevice, "Data\\Images\\bg2.png", &m_pBoard );
	if( FAILED( hr ) ) return hr;

	// Set up the scene lighting.
	ZeroMemory( &m_Light1, sizeof(D3DLIGHT9) );

	m_Light1.Type = D3DLIGHT_DIRECTIONAL;
	m_Light1.Diffuse.r = 1.0f;
	m_Light1.Diffuse.g = 1.0f;
	m_Light1.Diffuse.b = 1.0f;
	m_Light1.Diffuse.a = 1.0f;
	m_Light1.Direction.x = -1.0f;
	m_Light1.Direction.y = -1.0f;
	m_Light1.Direction.z = 1.0f;

	m_pDevice->SetLight( 0, &m_Light1 );
	m_pDevice->LightEnable( 0, TRUE );

	// Set up the game timing.
	timeBeginPeriod( 1 );

	m_fOldTime = 0.0f;
	m_fNewTime = 0.0f;
	m_fDeltaTime = 0.0f;

	// Set up the various screens. Title, game, etc.
	m_PreviousState = InitScreen;
	m_CurrentState = InitScreen;
	m_NextState = TitleScreen;

	// Set up the mouse position.
	m_dwMouseX = (m_dwWinWidth / 2);
	m_dwMouseY = (m_dwWinHeight / 2);

	m_bEscape = FALSE;
	m_bMouseL = FALSE;
	m_bMouseR = FALSE;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Destroy
//
//  Desc: Frees all used resources and shuts down the game.
// ----------------------------------------------------------------------------
void CGame::Destroy()
{
	timeEndPeriod( 1 );

	m_pBackground->Release();
	m_pBoard->Release();

	delete m_pPaddle;
	delete m_pBall;
	delete m_pGreenBrick;
	delete m_pBlueBrick;
	delete m_pRedBrick;
	delete m_pText;
	delete m_pInput;
	delete m_pCamera;
	delete m_pGraphics;

	ShowCursor( TRUE );

	m_pGraphics		= NULL;
	m_pCamera		= NULL;
	m_pBackground	= NULL;
	m_pBoard		= NULL;
	m_pText			= NULL;
	m_pRedBrick		= NULL;
	m_pBlueBrick	= NULL;
	m_pGreenBrick	= NULL;
	m_pBall			= NULL;
	m_pPaddle		= NULL;
	m_hWnd			= NULL;
	m_hInstance		= NULL;
}




// ----------------------------------------------------------------------------
//  Name: Run
//
//  Desc: The main loop of the game.
// ----------------------------------------------------------------------------
void CGame::Run()
{
	MSG msg;

	ZeroMemory( &msg, sizeof(MSG) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if( m_CurrentState == ExitingScreen )
			{
				PostQuitMessage( 0 );
			}
			else
			{
				m_fOldTime = (FLOAT)timeGetTime() / 1000;

				Update( m_fDeltaTime );
				Render();

				// Calculate how much time has passed. This helps with regulating
				// the speed of the game, otherwise things would move to fast.
				m_fNewTime = (FLOAT)timeGetTime() / 1000;
				m_fDeltaTime = m_fNewTime - m_fOldTime;
			}
		}
	}
}




// ----------------------------------------------------------------------------
//  Name: MsgProc
//
//  Desc: Handles system messages.
// ----------------------------------------------------------------------------
LRESULT WINAPI CGame::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
}



// ----------------------------------------------------------------------------
//  Name: Update
//
//  Desc: Updates the scene.
// ----------------------------------------------------------------------------
HRESULT CGame::Update( FLOAT fElapsedTime )
{
	GameState NextState;

	// The way this works... Each state has an initialize function, an update
	// function and a render function. The update function can return a new
	// state to switch to, or return the current state to not switch states.
	switch( m_CurrentState )
	{
	case InitScreen:
		NextState = TitleScreen;
		break;

	case TitleScreen:
		// We are in the title screen, update it.
		NextState = UpdateTitleScreen( fElapsedTime );
		break;

	case GameScreen:
		// We are in the game screen, update it.
		NextState = UpdateGameScreen( fElapsedTime );
		break;

	default:
		return D3D_OK;
	}

	if( NextState != m_CurrentState )
	{
		// We are switching states.
		DbgPrint( "Switching states." );

		m_PreviousState = m_CurrentState;
		m_CurrentState = NextState;

		switch( NextState )
		{
		case ExitingScreen:
			// Causes the program to shut down.
			PostMessage( NULL, WM_CLOSE, 0U, 0U );
			return D3D_OK;

		case GameScreen:
			// If we switched to the GameScreen state, initialize the game.
			InitGameScreen();
			break;

		case TitleScreen:
			// If we switched to the TitleScreen state, initialize the title
			// screen.
			InitTitleScreen();
			break;
		}
	}

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Render
//
//  Desc: Renders the scene.
// ----------------------------------------------------------------------------
HRESULT CGame::Render()
{
	// Clear the backbuffer.
	m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0, 1.0f, 0 );

	// Attempt to begin the scene.
	if( SUCCEEDED( m_pDevice->BeginScene() ) )
	{
		// See RenderBackground function below.
		RenderBackground();

		// What state are we in? Render appropriately.
		switch( m_CurrentState )
		{
		case TitleScreen:
			RenderTitleScreen();
			break;

		case GameScreen:
			RenderGameScreen();
			break;

		default:
			break;
		}

		// We are done rendering, end the scene.
		m_pDevice->EndScene();
	}
	else
	{
		DbgPrint( "For some reason we didn't succeed in beginning the scene..." );
	}

	// Present the backbuffer to the display.
	m_pDevice->Present( NULL, NULL, NULL, NULL );

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: InitTitleScreen
//
//  Desc: Initializes the title screen.
// ----------------------------------------------------------------------------
HRESULT CGame::InitTitleScreen()
{
	m_bStartGameSelected = FALSE;
	m_bExitGameSelected = FALSE;

	m_bMouseL = FALSE;
	m_bMouseR = FALSE;
	m_bEscape = FALSE;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: UpdateTitleScreen
//
//  Desc: Processes mouse and clicking events.
// ----------------------------------------------------------------------------
GameState CGame::UpdateTitleScreen( FLOAT fElapsedTime )
{
	FLOAT		x, y;
	BOOL		l, r;
	GameState	NextState = TitleScreen;

	// Exit the game if the Escape key is pressed. A simple toggle variable
	// is used so that key events are not processed too repeatedly.
	if( m_bEscape && !m_pInput->KeyDown( DIK_ESCAPE ) )
	{
		NextState = ExitingScreen;

		m_bEscape = FALSE;
	}
	else if( !m_bEscape && m_pInput->KeyDown( DIK_ESCAPE ) )
	{
		m_bEscape = TRUE;
	}
	
	// Update the mouse cursor position.
	m_pInput->GetMouse( &l, &r, &x, &y );

	m_dwMouseX += x;
	m_dwMouseY += y;

	// Make sure mouse doesn't go off the screen.
	if( m_dwMouseX < 0.0f ) m_dwMouseX = 0.0f;
	if( m_dwMouseX >= (FLOAT)m_dwWinWidth ) m_dwMouseX = (FLOAT)(m_dwWinWidth - 1);

	if( m_dwMouseY < 0.0f ) m_dwMouseY = 0.0f;
	if( m_dwMouseY >= (FLOAT)m_dwWinHeight ) m_dwMouseY = (FLOAT)(m_dwWinHeight - 1);

	// Make sure the camera is oriented just right.
	m_pCamera->Position( 0.0f, 0.0f, -1.0f );

	m_pDevice->SetTransform( D3DTS_VIEW, &m_pCamera->GetViewMatrix() );

	// Check if either of the menu options are selected.
	if( (m_dwMouseX >= ((m_dwWinWidth / 2) - 55)) && (m_dwMouseX <= ((m_dwWinWidth / 2) + 55)) && (m_dwMouseY >= ((m_dwWinHeight / 2) - 20)) && (m_dwMouseY <= ((m_dwWinHeight / 2) - 5)) )
	{
		// Did we click on the Start Game option?
		if( m_bMouseL && !l )
		{
			NextState = GameScreen;
			m_bMouseL = FALSE;
		}
		else if( !m_bMouseL && l )
		{
			m_bMouseL = TRUE;
		}

		m_bStartGameSelected = TRUE;
	}
	else
	{
		m_bStartGameSelected = FALSE;
	}

	if( (m_dwMouseX >= ((m_dwWinWidth / 2) - 55)) && (m_dwMouseX <= ((m_dwWinWidth / 2) + 55)) && (m_dwMouseY >= ((m_dwWinHeight / 2) + 10)) && (m_dwMouseY <= ((m_dwWinHeight / 2) + 25)) )
	{
		// Did we click on the Exit Game option?
		if( m_bMouseL && !l )
		{
			NextState = ExitingScreen;
			m_bMouseL = FALSE;
		}
		else if( !m_bMouseL && l )
		{
			m_bMouseL = TRUE;
		}

		m_bExitGameSelected = TRUE;
	}
	else
	{
		m_bExitGameSelected = FALSE;
	}

	return NextState;
}




// ----------------------------------------------------------------------------
//  Name: RenderTitleScreen
//
//  Desc: Renders the title screen.
// ----------------------------------------------------------------------------
HRESULT CGame::RenderTitleScreen()
{
	// Render the Start Game menu option. Change the color if the mouse is
	// hovering over it.
	if( !m_bStartGameSelected )
	{
		m_pText->Print( (m_dwWinWidth / 2) - 55, (m_dwWinHeight / 2) - 20, 0xFF0000FF, "Start Game" );
	}
	else
	{
		m_pText->Print( (m_dwWinWidth / 2) - 55, (m_dwWinHeight / 2) - 20, 0xFF00FF00, "Start Game" );
	}

	// Same as above.
	if( !m_bExitGameSelected )
	{
		m_pText->Print( (m_dwWinWidth / 2) - 50, (m_dwWinHeight / 2) + 10, 0xFF0000FF, "Exit Game" );
	}
	else
	{
		m_pText->Print( (m_dwWinWidth / 2) - 50, (m_dwWinHeight / 2) + 10, 0xFF00FF00, "Exit Game" );
	}

	// See the RenderMouse function below.
	RenderMouse();

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: InitGameScreen
//
//  Desc: Initializes the actual game portion and loads a level.
// ----------------------------------------------------------------------------
HRESULT CGame::InitGameScreen()
{
	ifstream	level;
	CHAR		brick;
	char		t[255];
	int			i;

	// Load the level.
	m_dwTotalBricks = 0;
	i = 0;

	// Open a level file.
	// Level files are a simple 10x10 grid of numbers 0-3.
	level.open( "Data\\Levels\\level1.lvl", ios::in );

	do
	{
		level >> brick;

		m_tMap[i] = brick;

		if( (brick > '0') && (brick < '4') )
		{
			// If a 1 2 or 3 is speficied, it's a brick.
			sprintf( t, "%c", brick );

			m_dwTotalBricks++;
		}

		i++;
	} while( !level.eof() && (i < 100) );

	level.close();

	// Set up the ball and paddle.
	m_vPaddlePos.x = 0.0f;
	m_vPaddlePos.y = -0.75f;
	m_vPaddlePos.z = 0.0f;

	m_vBallPos.x = 0.0f;
	m_vBallPos.y = m_vPaddlePos.y + 0.051f;
	m_vBallPos.z = 0.0f;

	m_vBallVel.x = m_vBallVel.y = m_vBallVel.z = 0.0f;

	m_fBallRadius = 0.03f;

	// FPS Counter. For fun.
	m_dwOldFPS = 0;
	m_dwNewFPS = 0;
	m_dwBallTimer = 0;
	m_dwScore = 0;
	m_fSecondCount = 0.f;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: UpdateGameScreen
//
//  Desc: Updates the game. Processes ball and paddle movement as well as
//        collisions and key presses.
// ----------------------------------------------------------------------------
GameState CGame::UpdateGameScreen( FLOAT fElapsedTime )
{
	FLOAT x, y;
	BOOL l, r;
	D3DVECTOR d;
	GameState NextState = GameScreen;

	// Go back to the title screen if escape is pressed.
	if( m_bEscape && !m_pInput->KeyDown( DIK_ESCAPE ) )
	{
		NextState = TitleScreen;

		m_bEscape = FALSE;
	}
	else if( !m_bEscape && m_pInput->KeyDown( DIK_ESCAPE ) )
	{
		m_bEscape = TRUE;
	}

	// Get the deltas for how much the mouse moved and what buttons were
	// pressed.
	m_pInput->GetMouse( &l, &r, &x, &y );

	// Position the paddle.
	m_vPaddlePos.x += (x * 0.1f * fElapsedTime);

	// Make sure the paddle cannot be moved outside of the game boundaries.
	// TODO: Refactor this so that magic numbers are NOT used.
	if( (m_vPaddlePos.x - 0.15f) <= (-0.9f) ) m_vPaddlePos.x = -0.75f;
	if( (m_vPaddlePos.x + 0.15f) >= (0.9f) ) m_vPaddlePos.x = 0.75f;

	m_pPaddle->SetPosition( m_vPaddlePos.x, m_vPaddlePos.y, m_vPaddlePos.z );

	// Position the ball.
	CheckForCollisions( fElapsedTime );

	// Bounce the ball off the sides of the walls.
	// TODO: Refactor this so that magic numbers are NOT used.
	if( (m_vBallPos.x + 0.03f) >= (0.99f) )
	{
		m_vBallVel.x = -m_vBallVel.x;
		m_vBallPos.x = 0.95f;
	}
	if( (m_vBallPos.x - 0.03f) <= (-1.0f) )
	{
		m_vBallVel.x = -m_vBallVel.x;
		m_vBallPos.x = -0.96f;
	}
	
	if( (m_vBallPos.y + 0.03f) >= (1.0f) )
	{
		m_vBallVel.y = -m_vBallVel.y;
		m_vBallPos.y = 0.96f;
	}
	if( (m_vBallPos.y - 0.03f) <= (-1.0f) )
	{
		m_vBallVel.y = -m_vBallVel.y;
		m_vBallPos.y = -0.96f;
	}

	// Find out how the ball bounces off the paddle.
	d.x = m_vPaddlePos.x - m_vBallPos.x;
	d.y = m_vPaddlePos.y - m_vBallPos.y;
	d.z = m_vPaddlePos.z - m_vBallPos.z;

	d = VecNormalize( d );

	d.x = m_vBallPos.x + (d.x * 0.03);
	d.y = m_vBallPos.y + (d.y * 0.03);
	d.z = m_vBallPos.z + (d.z * 0.03);

	// TODO: Refactor so that magic numbers are NOT used.
	if( (d.y <= (m_vPaddlePos.y + 0.02f)) && (d.x >= (m_vPaddlePos.x - 0.25f)) && (d.x <= (m_vPaddlePos.x + 0.25f)) )
	{
		m_vBallVel.y = -m_vBallVel.y;
		m_vBallPos.y = m_vPaddlePos.y + 0.051f;
	}
	
	if( (m_vBallPos.y < (m_vPaddlePos.y - 0.2f)) )
	{
		NextState = TitleScreen;
	}

	m_pBall->SetPosition( m_vBallPos.x, m_vBallPos.y, m_vBallPos.z );

	// Make sure the camera is oriented just right.
	m_pCamera->Position( 0.0f, 0.0f, -2.5f );

	m_pDevice->SetTransform( D3DTS_VIEW, &m_pCamera->GetViewMatrix() );

	// End the game if all the bricks have been destroyed.
	if( !m_dwTotalBricks ) NextState = TitleScreen;

	m_fSecondCount += fElapsedTime;
	m_dwNewFPS++;

	if( m_fSecondCount >= 1.0f )
	{
		// Calculate an FPS. Deprecated.
		m_dwOldFPS = m_dwNewFPS;
		m_dwNewFPS = 0;

		// Increase the ball start timer.
		m_dwBallTimer++;

		m_fSecondCount = 0.0f;
	}

	if( m_dwBallTimer == 2 )
	{
		// If 3 seconds have passed, then start the ball moving.
		m_vBallVel.x = 0.7f;
		m_vBallVel.y = 0.6f;
	}

	return NextState;
}




// ----------------------------------------------------------------------------
//  Name: RenderGameScreen
//
//  Desc: 
// ----------------------------------------------------------------------------
HRESULT CGame::RenderGameScreen()
{
	FLOAT x, y;

	x = 0;
	y = 0;

	// See RenderBoard function below.
	RenderBoard();

	// Print the little help message at the bottom.
	// X, Y, color, text.
	m_pText->Print( 200, (m_dwWinHeight) - 50, 0xFF0000FF, "Press Esc to quit and go back to the main menu." );

	// Render the paddle and the ball.
	m_pPaddle->Render( m_pDevice );
	m_pBall->Render( m_pDevice );

	// Render the remaining bricks in the map.
	for( int i = 0; i < 100; i++ )
	{
		switch( m_tMap[i] )
		{
		case '0':
			// Do nothing. Brick got destroyed or wasn't there in the first place.
			break;

		case '1':
			// Red brick.
			// Just calculate the position of the brick, then render it.
			// TODO: refactor this so the calculation is only done once in one place.
			m_pRedBrick->SetPosition( (-0.9f + (0.19f * x)), (0.4 - (0.08 * y)) + 0.5f, 0.0f );
			m_pRedBrick->Render( m_pDevice );

			break;

		case '2':
			// Green brick.
			// TODO: refactor this so the calculation is only done once in one place.
			m_pGreenBrick->SetPosition( (-0.9f + (0.19f * x)), (0.4 - (0.08 * y)) + 0.5f, 0.0f );
			m_pGreenBrick->Render( m_pDevice );
			break;

		case '3':
			// Blue brick;
			// TODO: refactor this so the calculation is only done once in one place.
			m_pBlueBrick->SetPosition( (-0.9f + (0.19f * x)), (0.4 - (0.08 * y)) + 0.5f, 0.0f );
			m_pBlueBrick->Render( m_pDevice );

			break;
		}

		x++;

		if( x > 9 )
		{
			x = 0;
			y++;
		}
	}

	RenderScore();

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: CheckForCollisions
//
//  Desc: Checks for a collision between the ball and each brick still in
//        existance.
// ----------------------------------------------------------------------------
VOID CGame::CheckForCollisions( FLOAT fElapsedTime )
{
	D3DVECTOR d;
	FLOAT x, y, bx, by;
	FLOAT newx, newy;
	FLOAT bx1, by1;
	FLOAT tx, ty;
	FLOAT d1, d2;
	BOOL cx, cy;
	BOOL hit = FALSE;
	char t[255];

	x = 0.0f;
	y = 0.0f;

	// For each brick slot in existance...
	for( int i = 0; i < 100; i++ )
	{
		switch( m_tMap[i] )
		{
		// This is going to be ball color independent.
		case '1':
		case '2':
		case '3':
			// Calculate the center of the current brick.
			bx = (-0.9f + (0.19f * x));
			by = (0.4f - (0.08f * y)) + 0.5f;

			// Calculate where the ball *will* be if it moves.
			newx = m_vBallPos.x + (m_vBallVel.x * fElapsedTime);
			newy = m_vBallPos.y + (m_vBallVel.y * fElapsedTime);

			// I am only going to type this once... for each side of the ball
			// figure out if the new position will take that side inside the
			// brick boundaries.
			d.x = newx;
			d.y = newy + m_fBallRadius;

			if( (d.y <= (by + 0.02f)) && (d.y >= (by - 0.02f)) && (d.x >= (bx - 0.075f)) && (d.x <= (bx + 0.075f)) && !hit )
			{
				// Ok, it went inside, we know it hit.
				hit = TRUE;

				// Reverse the direction of the ball based on which side of
				// the brick the ball hit.
				m_vBallVel.y = -m_vBallVel.y;

				// Use the triangle ration math equation to determine how much
				// the ball will move in the x direction (the y is pretty self-
				// explanatory).
				ty = by - 0.02f - d.y;
				tx = (ty * newx) / newy;

				// Move the ball, and we are done.
				m_vBallPos.x += tx * fElapsedTime;
				m_vBallPos.y += ty * fElapsedTime;
			}
			
			d.x = newx;
			d.y = newy - m_fBallRadius;

			if( (d.y <= (by + 0.02f)) && (d.y >= (by - 0.02f)) && (d.x >= (bx - 0.075f)) && (d.x <= (bx + 0.075f)) && !hit )
			{
				hit = TRUE;

				m_vBallVel.y = -m_vBallVel.y;

				ty = d.y - by + 0.02f;
				tx = (ty * newx) / newy;

				m_vBallPos.x += tx * fElapsedTime;
				m_vBallPos.y += ty * fElapsedTime;
			}

			d.x = newx - m_fBallRadius;
			d.y = newy;

			if( (d.y <= (by + 0.02f)) && (d.y >= (by - 0.02f)) && (d.x >= (bx - 0.075f)) && (d.x <= (bx + 0.075f)) && !hit )
			{
				hit = TRUE;

				m_vBallVel.x = -m_vBallVel.x;

				tx = d.x - (bx + 0.02f);
				ty = tx * (newy / newx);

				m_vBallPos.x += tx * fElapsedTime;
				m_vBallPos.y += ty * fElapsedTime;
			}

			d.x = newx + m_fBallRadius;
			d.y = newy;

			if( (d.y <= (by + 0.02f)) && (d.y >= (by - 0.02f)) && (d.x >= (bx - 0.075f)) && (d.x <= (bx + 0.075f)) && !hit )
			{
				hit = TRUE;

				m_vBallVel.x = -m_vBallVel.x;

				tx = (bx - 0.02f) - d.x;
				ty = tx * (newy / newx);

				m_vBallPos.x += tx * fElapsedTime;
				m_vBallPos.y += ty * fElapsedTime;
			}

			// This section is a bit different and more complex. As before I
			// will only go over this once to save space. For each corner of
			// the brick, calculate whether or not the corner is inside the
			// square boundaries of the ball.
			bx1 = bx - 0.075f;
			by1 = by + 0.02f;

			if( (bx1 >= (newx - m_fBallRadius)) && (bx1 <= (newx + m_fBallRadius)) && (by1 >= (newy - m_fBallRadius)) && (by1 <= (newy + m_fBallRadius)) && !hit )
			{
				// It's inside the boundaries. Determine if the corner is
				// actually inside the circle defined by the ball. This is
				// simply done by taking the distance between the corner and
				// the center of the ball, and if it's equal or less than the
				// radius of the ball, then it has intersected.
				d1 = sqrt( pow( (bx1 - newx), 2.0f ) + pow( (by1 - newy), 2.0f ) );

				if( d1 <= m_fBallRadius )
				{
					// Ok, we hit.
					hit = TRUE;

					d.x = m_vBallVel.x;
					d.y = m_vBallVel.y;
					d.z = m_vBallVel.z;

					// Figure out the magnitude of how much the ball would have
					// moved in the direction specified by the velocity vector.
					d2 = sqrt( pow( (d.x * fElapsedTime), 2.0f ) + pow( (d.y * fElapsedTime), 2.0f ) + pow( (d.z * fElapsedTime), 2.0f ) );

					// Now, normalize the velocity vector.
					d = VecNormalize( d );

					// Find out how much the ball actually needs to move so that
					// the edge of the ball is only just touching the corner of
					// the brick.
					d2 -= (m_fBallRadius - d1);

					// Move the ball.
					m_vBallPos.x += d.x * d2;
					m_vBallPos.y += d.y * d2;
					m_vBallPos.z += d.z * d2;

					// Now the fun part. Find out what the vector is between the
					// corner of the brick and the center of the ball (the
					// normal vector for this calculation).
					d.x = (bx1 - m_vBallPos.x);
					d.y = (by1 - m_vBallPos.y);
					d.z = 0.0f;

					// Normalize the vector.
					d = VecNormalize( d );

					// Now, to calculate the angle that the ball bounces when it
					// hits the brick, the formula is as follows:
					//
					// v2 = -(2 * (n . v1) * n - v1)
					//
					// Where . is the dot product of two vectors, n is the normal
					// vector at the colision point, v1 is the initial velocity
					// vector and v2 is the new velocity vector.
					d2 = 2 * VecDot( d, m_vBallVel );
					d.x *= d2;
					d.y *= d2;
					d.z *= d2;

					// Now we have a new direction and velocity vector.
					m_vBallVel.x = -(d.x - m_vBallVel.x);
					m_vBallVel.y = -(d.y - m_vBallVel.y);
					m_vBallVel.z = -(d.z - m_vBallVel.z);
				}
			}

			bx1 = bx + 0.075f;
			by1 = by + 0.02f;

			if( (bx1 >= (newx - m_fBallRadius)) && (bx1 <= (newx + m_fBallRadius)) && (by1 >= (newy - m_fBallRadius)) && (by1 <= (newy + m_fBallRadius)) && !hit )
			{
				d1 = sqrt( pow( (bx1 - newx), 2.0f ) + pow( (by1 - newy), 2.0f ) );

				if( d1 <= m_fBallRadius )
				{
					hit = TRUE;

					d.x = m_vBallVel.x;
					d.y = m_vBallVel.y;
					d.z = m_vBallVel.z;

					d2 = sqrt( pow( (d.x * fElapsedTime), 2.0f ) + pow( (d.y * fElapsedTime), 2.0f ) + pow( (d.z * fElapsedTime), 2.0f ) );

					d = VecNormalize( d );

					d2 -= (m_fBallRadius - d1);

					m_vBallPos.x += d.x * d2;
					m_vBallPos.y += d.y * d2;
					m_vBallPos.z += d.z * d2;

					d.x = (bx1 - m_vBallPos.x);
					d.y = (by1 - m_vBallPos.y);
					d.z = 0.0f;

					d = VecNormalize( d );

					d2 = 2 * VecDot( d, m_vBallVel );
					d.x *= d2;
					d.y *= d2;
					d.z *= d2;

					m_vBallVel.x = -(d.x - m_vBallVel.x);
					m_vBallVel.y = -(d.y - m_vBallVel.y);
					m_vBallVel.z = -(d.z - m_vBallVel.z);
				}
			}

			bx1 = bx - 0.075f;
			by1 = by - 0.02f;

			if( (bx1 >= (newx - m_fBallRadius)) && (bx1 <= (newx + m_fBallRadius)) && (by1 >= (newy - m_fBallRadius)) && (by1 <= (newy + m_fBallRadius)) && !hit )
			{
				d1 = sqrt( pow( (bx1 - newx), 2.0f ) + pow( (by1 - newy), 2.0f ) );

				if( d1 <= m_fBallRadius )
				{
					hit = TRUE;

					d.x = m_vBallVel.x;
					d.y = m_vBallVel.y;
					d.z = m_vBallVel.z;

					d2 = sqrt( pow( (d.x * fElapsedTime), 2.0f ) + pow( (d.y * fElapsedTime), 2.0f ) + pow( (d.z * fElapsedTime), 2.0f ) );

					d = VecNormalize( d );

					d2 -= (m_fBallRadius - d1);

					m_vBallPos.x += d.x * d2;
					m_vBallPos.y += d.y * d2;
					m_vBallPos.z += d.z * d2;

					d.x = (bx1 - m_vBallPos.x);
					d.y = (by1 - m_vBallPos.y);
					d.z = 0.0f;

					d = VecNormalize( d );

					d2 = 2 * VecDot( d, m_vBallVel );
					d.x *= d2;
					d.y *= d2;
					d.z *= d2;

					m_vBallVel.x = -(d.x - m_vBallVel.x);
					m_vBallVel.y = -(d.y - m_vBallVel.y);
					m_vBallVel.z = -(d.z - m_vBallVel.z);
				}
			}

			bx1 = bx + 0.075f;
			by1 = by - 0.02f;

			if( (bx1 >= (newx - m_fBallRadius)) && (bx1 <= (newx + m_fBallRadius)) && (by1 >= (newy - m_fBallRadius)) && (by1 <= (newy + m_fBallRadius)) && !hit )
			{
				d1 = sqrt( pow( (bx1 - newx), 2.0f ) + pow( (by1 - newy), 2.0f ) );

				if( d1 <= m_fBallRadius )
				{
					hit = TRUE;

					d.x = m_vBallVel.x * fElapsedTime;
					d.y = m_vBallVel.y * fElapsedTime;
					d.z = m_vBallVel.z * fElapsedTime;

					d2 = sqrt( pow( d.x, 2.0f ) + pow( d.y, 2.0f ) + pow( d.z, 2.0f ) );

					d = VecNormalize( d );

					d2 -= (m_fBallRadius - d1);

					m_vBallPos.x += d.x * d2;
					m_vBallPos.y += d.y * d2;
					m_vBallPos.z += d.z * d2;

					d.x = (bx1 - m_vBallPos.x);
					d.y = (by1 - m_vBallPos.y);
					d.z = 0.0f;

					d = VecNormalize( d );

					d2 = 2 * VecDot( d, m_vBallVel );

					d.x *= d2;
					d.y *= d2;
					d.z *= d2;

					m_vBallVel.x = -(d.x - m_vBallVel.x);
					m_vBallVel.y = -(d.y - m_vBallVel.y);
					m_vBallVel.z = -(d.z - m_vBallVel.z);
				}
			}

			// Now, if we hit, we will increment our score based on the color
			// of the brick, and we will "destroy" the brick, making it
			// disappear, as well as decrement the brick count so we know when
			// we end the game.
			if( hit )
			{
				switch( m_tMap[i] )
				{
				case '1':
					m_dwScore += 100;
					break;

				case '2':
					m_dwScore += 200;
					break;

				case '3':
					m_dwScore += 300;
					break;
				}

				m_tMap[i] = '0';
				m_dwTotalBricks--;

				return;
			}

			break;

		default:
			break;
		}

		x++;

		if( x > 9 )
		{
			x = 0;
			y++;
		}
	}

	// If we didn't hit any bricks, we'll just keep moving the same way we
	// were going to do otherwise.
	m_vBallPos.x += m_vBallVel.x * fElapsedTime;
	m_vBallPos.y += m_vBallVel.y * fElapsedTime;
	m_vBallPos.z += m_vBallVel.z * fElapsedTime;
}




// ----------------------------------------------------------------------------
//  Name: RenderMouse
//
//  Desc: Renders a basic mouse cursor on the screen.
// ----------------------------------------------------------------------------
VOID CGame::RenderMouse()
{
	// Create a line list to draw a simple mouse cursor. This is defined in
	// types.h
	VERTEX2D v[5] =
	{
		{ (FLOAT)m_dwMouseX, (FLOAT)m_dwMouseY, 0.5f, 1.0f, 0xFFFF0000 },
		{ (FLOAT)m_dwMouseX, (FLOAT)(m_dwMouseY + 15), 0.5f, 1.0f, 0xFFFF0000 },
		{ (FLOAT)(m_dwMouseX + 5), (FLOAT)(m_dwMouseY + 10), 0.5f, 1.0f, 0xFFFF0000 },
		{ (FLOAT)(m_dwMouseX + 10), (FLOAT)(m_dwMouseY + 10), 0.5f, 1.0f, 0xFFFF0000 },
		{ (FLOAT)m_dwMouseX, (FLOAT)m_dwMouseY, 0.5f, 1.0f, 0xFFFF0000 }
	};

	// Set the primitive format.
	m_pDevice->SetFVF( D3DFVF_VERTEX2D );

	// Render the mouse cursor.
	if( FAILED( m_pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, (void*)&v, sizeof(VERTEX2D) ) ) )
	{
		DbgPrint( "Failed to draw the mouse." );
	}
}




// ----------------------------------------------------------------------------
//  Name: RenderBoard
//
//  Desc: Renders the backdrop behind the actual game, to make it easier to 
//        see against the background.
// ----------------------------------------------------------------------------
VOID CGame::RenderBoard()
{
	D3DXMATRIX matWorld;
	D3DMATERIAL9 mat;

	// Create a square mesh for the background behind the actual game. This is
	// to help make the actual game easier to see.
	TLVERTEX v[4] =
	{
		// This structure is defined in types.h
		{ -1.3f, 1.3f, 0.6f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
		{ 1.3f, 1.3f, 0.6f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ -1.3f, -1.3f, 0.6f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
		{ 1.3f, -1.3f, 0.6f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f }
	};

	// Create a material for this. We just want it to receive full light, but
	// be 75% opaque.
	ZeroMemory( &mat, sizeof(D3DMATERIAL9) );
	mat.Diffuse.a = 0.75f;
	mat.Diffuse.r = mat.Diffuse.g = mat.Diffuse.b = 1.0f;
	mat.Ambient = mat.Diffuse;
	
	// We don't need to position/rotate this any.
	D3DXMatrixIdentity( &matWorld );

	// Set the transformation matrix.
	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set the format of the primitives to draw, set the board texture and
	// set the material we created above.
	m_pDevice->SetFVF( D3DFVF_TLVERTEX );
	m_pDevice->SetTexture( 0, m_pBoard );
	m_pDevice->SetMaterial( &mat );

	// Turn on lighting briefly.
	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// Render the board.
	if( FAILED( m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (void*)&v, sizeof(TLVERTEX) ) ) )
	{
		DbgPrint( "Failed to draw the board." );
	}

	// Lighting doesn't need to be on unless absolutely necessary.
	m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Clean up the texture, don't want this messing anything else up.
	m_pDevice->SetTexture( 0, NULL );
}




// ----------------------------------------------------------------------------
//  Name: RenderBackground
//
//  Desc: Renders the global backdrop image.
// ----------------------------------------------------------------------------
VOID CGame::RenderBackground()
{
	// Define the backdrop mesh. Type is defined in types.h
	TVERTEX2D v[4] =
	{
		{ 0.0f, 0.0f, 0.6f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
		{ (FLOAT)m_dwWinWidth, 0.0f, 0.6f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
		{ 0.0f, (FLOAT)m_dwWinHeight, 0.6f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
		{ (FLOAT)m_dwWinWidth, (FLOAT)m_dwWinHeight, 0.6f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f }
	};

	// Set the primitive format.
	m_pDevice->SetFVF( D3DFVF_TVERTEX2D );

	// Set the texture to use.
	m_pDevice->SetTexture( 0, m_pBackground );

	// If Z-Buffering is on, the mouse will get overwritten, as well as the
	// 3D models. This is because the Z index of the mesh is almost as close to
	// 0 as it's possible to get, so almost nothing can get drawn in front, it's
	// just simpler to turn it off.
	m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

	// Render the backdrop.
	if( FAILED( m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (void*)&v, sizeof(TVERTEX2D) ) ) )
	{
		DbgPrint( "Failed to draw the background." );
	}

	// All right, Z-buffering can be turned back on now.
	m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	// Clean up the texture.
	m_pDevice->SetTexture( 0, NULL );
}




// ----------------------------------------------------------------------------
//  Name: RenderScore
//
//  Desc: Render the game score.
// ----------------------------------------------------------------------------
VOID CGame::RenderScore()
{
	char sFPS[255];

	sprintf( sFPS, "Score: %d", m_dwScore );

	// X, Y, color (black at 100% opacity), text.
	m_pText->Print( 700, (m_dwWinHeight) - 50, 0xFF000000, sFPS );
}




// ----------------------------------------------------------------------------
//  Name: VecNormalize
//
//  Desc: Normalizes a vector.
// ----------------------------------------------------------------------------
D3DVECTOR CGame::VecNormalize( D3DVECTOR v )
{
	FLOAT		fMag;
	D3DVECTOR	r;

	// The simple distance formula.
	fMag = sqrt( pow( v.x, 2.0f ) + pow( v.y, 2.0f ) + pow( v.z, 2.0f ) );

	// Normal of a vector is the components divided by the magnitude.
	if( fMag )
	{
		r.x = v.x / fMag;
		r.y = v.y / fMag;
		r.z = v.z / fMag;
	}

	return r;
}




// ----------------------------------------------------------------------------
//  Name: VecDot
//
//  Desc: Computes the dot product of two vectors.
// ----------------------------------------------------------------------------
FLOAT CGame::VecDot( D3DVECTOR v1, D3DVECTOR v2 )
{
	// Can't any simpler than this.
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
