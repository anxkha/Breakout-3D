// ----------------------------------------------------------------------------
//  Filename: object.cpp
//  Author: Lucas Suggs
//
//  Copyright (c) 2009, Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include "main.h"




// ----------------------------------------------------------------------------
//  Name: CObject
//
//  Desc: Constructor
// ----------------------------------------------------------------------------
CObject::CObject()
{
	m_pMesh					= NULL;
	m_pMaterials			= NULL;
	m_pTextures				= NULL;
	m_bVisible				= FALSE;
	m_nNumberOfMaterials	= 0;

	m_vPosition				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vRotation				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}




// ----------------------------------------------------------------------------
//  Name: ~CObject
//
//  Desc: Destructor
// ----------------------------------------------------------------------------
CObject::~CObject()
{
	Release();
}



// ----------------------------------------------------------------------------
//  Name: LoadX
//
//  Desc: Loads an object from an x file.
// ----------------------------------------------------------------------------
HRESULT CObject::LoadX( IDirect3DDevice9* pDevice, string sPath, string sFileName )
{
	string			sMsg;
	ID3DXBuffer*	pMtrlBuffer;
	D3DXMATERIAL*	pMaterials;
	string			sTextureName;
	HRESULT			hr;

	sTextureName = sPath + sFileName;

	// Attempt to load the mesh from a file.
	hr = D3DXLoadMeshFromX( sTextureName.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &pMtrlBuffer, NULL, &m_nNumberOfMaterials, &m_pMesh );
	if( FAILED( hr ) )
	{
		switch( hr )
		{
		case D3DERR_INVALIDCALL:
			DbgPrint( "Invalid call to D3DXLoadMeshFromX in file object.cpp" );
			break;

		case E_OUTOFMEMORY:
			DbgPrint( "Out of memory error in file object.cpp" );
			break;

		default:
			DbgPrint( "Undetermined error loading .x file." );
		}
		
		DbgPrint( "Unable to load: " + sTextureName );
		return hr;
	}

	// Load the materials and textures.
	pMaterials = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

	m_pMaterials = new D3DMATERIAL9[m_nNumberOfMaterials];
	m_pTextures = new IDirect3DTexture9*[m_nNumberOfMaterials];

	for( DWORD i = 0; i < m_nNumberOfMaterials; i++ )
	{
		// Copy each material and set the ambient reflectivity.
		// TODO: Fix the meshes so that the alpha channel is copied
		// properly, having to hardcode it is not good practice.
		m_pMaterials[i] = pMaterials->MatD3D;
		m_pMaterials[i].Diffuse.a = 0.75;
		m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;

		if( pMaterials->pTextureFilename )
		{
			// If a texture file name was specified for this material,
			// load it.
			sTextureName = sPath + pMaterials->pTextureFilename;

			if( FAILED( D3DXCreateTextureFromFile( pDevice, sTextureName.c_str(), &m_pTextures[i] ) ) )
			{
				DbgPrint( "A texture could not be loaded." );
				m_pTextures[i] = NULL;
			}
		}
		else
		{
			m_pTextures[i] = NULL;
		}
	}

	pMtrlBuffer->Release();

	m_bVisible = TRUE;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: LoadX
//
//  Desc: Loads an object from an x file.
// ----------------------------------------------------------------------------
HRESULT CObject::CreateBox( FLOAT fWidth, FLOAT fHeight, FLOAT fDepth )
{
	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Release
//
//  Desc: Releases all the memory used by the object.
// ----------------------------------------------------------------------------
HRESULT CObject::Release()
{
	if( m_pMesh )
	{
		m_pMesh->Release();

		for( ULONG i = 0; i < m_nNumberOfMaterials; i++ )
		{
			if( m_pTextures[i] ) m_pTextures[i]->Release();
		}

		delete[] m_pMaterials;
		delete[] m_pTextures;
	}

	m_pMesh					= NULL;
	m_pMaterials			= NULL;
	m_pTextures				= NULL;
	m_bVisible				= FALSE;
	m_nNumberOfMaterials	= 0;

	m_vPosition				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vRotation				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: Render
//
//  Desc: Renders the object to the screen.
// ----------------------------------------------------------------------------
HRESULT CObject::Render( IDirect3DDevice9* pDevice )
{
	D3DXMATRIX matRx, matRy, matRz;
	D3DXMATRIX matTranslation, matRotation;
	D3DXMATRIX matWorld;

	if( !m_bVisible ) return D3D_OK;

	// Calculate the translation matrix.
	D3DXMatrixTranslation( &matTranslation, m_vPosition.x, m_vPosition.y, m_vPosition.z );

	// Calculate the rotation matrices.
	D3DXMatrixRotationX( &matRx, m_vRotation.x * (D3DX_PI / 180) );
	D3DXMatrixRotationY( &matRy, m_vRotation.y * (D3DX_PI / 180) );
	D3DXMatrixRotationZ( &matRz, m_vRotation.z * (D3DX_PI / 180) );

	// Perform rotations first, then translation. Otherwise, everything will be
	// rotated/translated wrong.
	matRotation = matRx;
	D3DXMatrixMultiply( &matRotation, &matRotation, &matRy );
	D3DXMatrixMultiply( &matRotation, &matRotation, &matRz );
	D3DXMatrixMultiply( &matWorld, &matRotation, &matTranslation );

	pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Turn on lighting temporarily.
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	for( DWORD i = 0; i < m_nNumberOfMaterials; i++ )
	{
		// Each mesh is divided up into subsets. Each subset is determined by each
		// material. If there is only one material, there is only one subset.
		pDevice->SetMaterial( &m_pMaterials[i] );
		pDevice->SetTexture( 0, m_pTextures[i] );

		m_pMesh->DrawSubset( i );
	}

	// Turn off lighting.
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: SetPosition
//
//  Desc: Sets the position of the object.
// ----------------------------------------------------------------------------
HRESULT CObject::SetPosition( FLOAT x, FLOAT y, FLOAT z )
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: SetRotation
//
//  Desc: Sets the rotation of the object.
// ----------------------------------------------------------------------------
HRESULT CObject::SetRotation( FLOAT x, FLOAT y, FLOAT z )
{
	m_vRotation.x = x;
	m_vRotation.y = y;
	m_vRotation.z = z;

	return D3D_OK;
}




// ----------------------------------------------------------------------------
//  Name: GetPosition
//
//  Desc: Returns the position of the object.
// ----------------------------------------------------------------------------
VOID CObject::GetPosition( D3DXVECTOR3* p )
{
	*p = m_vPosition;
}




// ----------------------------------------------------------------------------
//  Name: GetRotation
//
//  Desc: Returns the rotation of the object.
// ----------------------------------------------------------------------------
VOID CObject::GetRotation( D3DXVECTOR3* p )
{
	*p = m_vRotation;
}
