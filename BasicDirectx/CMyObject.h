#pragma once

#include <windows.h>
#include <DirectXMath.h>
//#include "main.h"
//#include "CMyMesh.h"

struct OBJECT_PREFS //groups together all the preferences of the object
{
	DirectX::XMFLOAT3 rotAngels;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 scale;
};

//class CMyMesh; 

//-----------------------------------------------------------------------------
// Name : CObject (Class)
// Desc : Mesh container class used to store instances of meshes.
//-----------------------------------------------------------------------------
class CMyObject
{
public:
	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	CMyObject(void);
	//CMyObject( CMyMesh * pMesh ,OBJECT_PREFS* objectPref );
	virtual ~CMyObject(void);

	DirectX::XMMATRIX 	getWorldMatrix(DirectX::XMMATRIX& worldMatrix);
	//void				    attachMesh			(CMyMesh * pMesh);
	//virtual void			drawSubset			(IDirect3DDevice9* pd3dDevice, ULONG AttributeID, ID3DXEffect * effect, UINT numPass, D3DXMATRIX ViewProj);
	//bool					customiseAtrributes (ULONG* newAttribMap,ULONG atrributeCount);
	//CMyMesh*				getMesh				( ) const;

	bool					isObjectHidden		();
	void					setObjectHidden     (bool newStatus);
	void					setPos				(DirectX::XMFLOAT3 newPos);
	void					setRotAngels		(DirectX::XMFLOAT3 rotAngels);
	void					Rotate				(float x, float y, float z);
	void					setRotaionMatrix	(DirectX::XMFLOAT4X4 rotMatrix);

	//static void				setShadersHandles   (D3DXHANDLE matWorldViewProjH, D3DXHANDLE matWorldH, D3DXHANDLE matWorldTH);

	//-------------------------------------------------------------------------
	// Public Variables for This Class
	//-------------------------------------------------------------------------
	DirectX::XMFLOAT4X4   m_mtxWorld;             // Objects world matrix
	DirectX::XMFLOAT4X4   m_mtxRot;
	DirectX::XMFLOAT4X4   m_mtxScale;
	DirectX::XMFLOAT3   m_rotAngles;

private:
	bool		m_useCustomAttribMap;
	bool        m_hideObject;
	ULONG	  * m_pCustomAtribbMap;
	ULONG       m_customAtrribCount;

protected:
	//CMyMesh   * m_pMesh;                // Mesh we are instancing

	//static D3DXHANDLE 		m_matWorldViewProjH;
	//static D3DXHANDLE 		m_matWorldH;
	//static D3DXHANDLE		m_matWorldTH;
};
