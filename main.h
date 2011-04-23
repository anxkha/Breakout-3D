// ----------------------------------------------------------------------------
//  Filename: main.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

#include <d3dx9.h>
#include <tchar.h>
#include <math.h>
#include <string>
#include <fstream>
using namespace std;

#include "debug.h"
#include "types.h"
#include "graphics.h"
#include "object.h"
#include "camera.h"
#include "input.h"
#include "text.h"
#include "game.h"

#define GAME_TITLE	"Breakout 3D"

DWORD	TrueRandNum( int nLowBound, int nHighBound );
