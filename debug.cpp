// ----------------------------------------------------------------------------
//  Filename: debug.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// Global declarations.
ofstream g_DbgFile;




// -----------------------------------------------------------------------------
//  Name: DbgOpen
//
//  Desc: Open the specified debug file.
// -----------------------------------------------------------------------------
BOOL DbgOpen( char* sFileName )
{
	g_DbgFile.open( sFileName, ios::out );

	// TODO: Fix this so that it returns false if the file could not be opened.
	return TRUE;
}




// -----------------------------------------------------------------------------
//  Name: DbgClose
//
//  Desc: Close the debug file.
// -----------------------------------------------------------------------------
void DbgClose()
{
	if( g_DbgFile ) g_DbgFile.close();
}




// -----------------------------------------------------------------------------
//  Name: DbgPrint
//
//  Desc: Print a line to the debug file.
// -----------------------------------------------------------------------------
void DbgPrint( string sOutput )
{
	g_DbgFile << sOutput << endl;
}
