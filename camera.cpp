// ----------------------------------------------------------------------------
//  Filename: camera.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// -----------------------------------------------------------------------------
//  Name: Initialize
//
//  Desc: Initialize the camera.
// -----------------------------------------------------------------------------
HRESULT CCamera::Initialize()
{
	m_CP.x = m_CP.y = m_CP.z = 0.0f;
	m_CR.x = m_CR.y = m_CR.z = 0.0f;

	return RestoreDeviceObjects();
}




// -----------------------------------------------------------------------------
//  Name: RestoreDeviceObjects
//
//  Desc: A bit redundant, but this calculates the translation and rotation
//        matrices and prepares them for use.
// -----------------------------------------------------------------------------
HRESULT CCamera::RestoreDeviceObjects()
{
	D3DXMATRIX matRx, matRy, matRz;

	D3DXMatrixTranslation( &m_matTranslation, -m_CP.x, -m_CP.y, -m_CP.z );
	D3DXMatrixRotationX( &matRx, -m_CR.x );
	D3DXMatrixRotationY( &matRy, -m_CR.y );
	D3DXMatrixRotationZ( &matRz, -m_CR.z );

	m_matRotation = matRz;
	D3DXMatrixMultiply( &m_matRotation, &m_matRotation, &matRy );
	D3DXMatrixMultiply( &m_matRotation, &m_matRotation, &matRx );

	return D3D_OK;
}




// -----------------------------------------------------------------------------
//  Name: LookAt
//
//  Desc: Sets the position of the camera, a point for it to look at and
//        a vector specifying which direction is "up" all in one fell swoop.
// -----------------------------------------------------------------------------
D3DXMATRIX CCamera::LookAt( D3DXVECTOR3 vPos, D3DXVECTOR3 vAt, D3DXVECTOR3 vUp )
{
	D3DXMatrixLookAtLH( &m_matView, &vPos, &vAt, &vUp );

	return m_matView;
}




// -----------------------------------------------------------------------------
//  Name: GetViewMatrix
//
//  Desc: Returns a viewing matrix for the camera.
// -----------------------------------------------------------------------------
D3DXMATRIX CCamera::GetViewMatrix()
{
	D3DXMatrixMultiply( &m_matView, &m_matTranslation, &m_matRotation );

	return m_matView;
}




// -----------------------------------------------------------------------------
//  Name: GetProjectionMatrix
//
//  Desc: Returns a projection matrix. This is only generally needed once.
// -----------------------------------------------------------------------------
D3DXMATRIX CCamera::GetProjectionMatrix( float fScreenWidth, float fScreenHeight )
{
	D3DXMATRIX matProj;

	D3DXMatrixPerspectiveFovLH( &matProj, (D3DX_PI / 4), (fScreenWidth / fScreenHeight), 0.0001f, 100.0f );

	return matProj;
}




// -----------------------------------------------------------------------------
//  Name: Move
//
//  Desc: Moves the camera a certain amount in the direction it's currently
//        facing.
// -----------------------------------------------------------------------------
VOID CCamera::Move( FLOAT d )
{
	FLOAT nx, nz;

	nx = d * sinf( (m_CR.y) * (D3DX_PI / 180) );
	nz = d * cosf( (m_CR.y) * (D3DX_PI / 180) );

	PositionRel( nx, 0, nz );
}




// -----------------------------------------------------------------------------
//  Name: Strafe
//
//  Desc: Strafes the camera based on the direction it's currently facing.
// -----------------------------------------------------------------------------
VOID CCamera::Strafe( FLOAT d )
{
	FLOAT nx, nz;

	nx = d * sinf( (m_CR.y + 90) * (D3DX_PI / 180) );
	nz = d * cosf( (m_CR.y + 90) * (D3DX_PI / 180) );

	PositionRel( nx, 0, nz );
}




// -----------------------------------------------------------------------------
//  Name: Rotate
//
//  Desc: Sets the rotation of the camera.
// -----------------------------------------------------------------------------
VOID CCamera::Rotate( FLOAT x, FLOAT y, FLOAT z )
{
	D3DXMATRIX matRx, matRy, matRz;

	m_CR.x = x;
	m_CR.y = y;
	m_CR.z = z;

	// Converts degrees to radians.
	D3DXMatrixRotationX( &matRx, -m_CR.x * (D3DX_PI / 180) );
	D3DXMatrixRotationY( &matRy, -m_CR.y * (D3DX_PI / 180) );
	D3DXMatrixRotationZ( &matRz, -m_CR.z * (D3DX_PI / 180) );

	m_matRotation = matRz;
	D3DXMatrixMultiply( &m_matRotation, &m_matRotation, &matRy );
	D3DXMatrixMultiply( &m_matRotation, &m_matRotation, &matRx );
}




// -----------------------------------------------------------------------------
//  Name: Position
//
//  Desc: Sets the position of the camera.
// -----------------------------------------------------------------------------
VOID CCamera::Position( FLOAT x, FLOAT y, FLOAT z)
{
	m_CP.x = x;
	m_CP.y = y;
	m_CP.z = z;

	D3DXMatrixTranslation( &m_matTranslation, -m_CP.x, -m_CP.y, -m_CP.z );
}




// -----------------------------------------------------------------------------
//  Name: PositionRel
//
//  Desc: Sets the position of the camera relative to where it is now.
// -----------------------------------------------------------------------------
VOID CCamera::PositionRel( FLOAT x, FLOAT y, FLOAT z )
{
	m_CP.x += x;
	m_CP.y += y;
	m_CP.z += z;

	Position( m_CP.x, m_CP.y, m_CP.z );
}




// -----------------------------------------------------------------------------
//  Name: RotateRel
//
//  Desc: Rotates the camera relative to its current rotation.
// -----------------------------------------------------------------------------
VOID CCamera::RotateRel( FLOAT x, FLOAT y, FLOAT z )
{
	m_CR.x += x;
	m_CR.y += y;
	m_CR.z += z;

	Rotate( m_CR.x, m_CR.y, m_CR.z );
}
