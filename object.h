// ----------------------------------------------------------------------------
//  Filename: object.h
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------
#pragma once

class CObject
{
protected:
	ID3DXMesh*			m_pMesh;

	D3DMATERIAL9*		m_pMaterials;
	IDirect3DTexture9**	m_pTextures;

	TLVERTEX*			m_pVertices;
	DWORD*				m_pIndices;

	D3DXVECTOR3			m_vPosition;
	D3DXVECTOR3			m_vRotation;

	DWORD				m_nNumberOfMaterials;

	BOOL				m_bVisible;

public:
	CObject();
	virtual ~CObject();

	VOID	GetPosition( D3DXVECTOR3* p );
	VOID	GetRotation( D3DXVECTOR3* p );

	HRESULT	LoadX( IDirect3DDevice9* pDevice, string sPath, string FileName );
	HRESULT CreateBox( FLOAT fWidth, FLOAT fLength, FLOAT fDepth );
	HRESULT CreateSphere( FLOAT fRadius, DWORD nLong, DWORD nLat );

	HRESULT	Release();
	HRESULT	Render( IDirect3DDevice9* pDevice );

	HRESULT	SetPosition( FLOAT x, FLOAT y, FLOAT z );
	HRESULT SetRotation( FLOAT x, FLOAT y, FLOAT z );
};
