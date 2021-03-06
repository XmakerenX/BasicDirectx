#ifndef __d3dH__
#define __d3dH__

#include <d3d11.h>
#include <DirectXMath.h>

namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

// namespace d3d
// {
// 	template<class T> void Release(T t)
// 	{
// 		if( t )
// 		{
// 			t->Release();
// 			t = 0;
// 		}
// 	}
// 
// 	template<class T> void Delete(T t)
// 	{
// 		if( t )
// 		{
// 			delete t;
// 			t = 0;
// 		}
// 	}
// 
// 	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
// 	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
// 	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
// 	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
// 	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
// 	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
// 	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
// 	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );
// 	const D3DXCOLOR	      GRAY( D3DCOLOR_XRGB(127, 127, 127) );
// 
// 
// 	//
// 	// Lights
// 	//
// 
// 	LIGHT_PREFS InitDirectionalLight(D3DXVECTOR4& direction, D3DXCOLOR color);
// 	LIGHT_PREFS InitPointLight(D3DXVECTOR3& position, D3DXCOLOR color);
// 	LIGHT_PREFS InitSpotLight(D3DXVECTOR3& position, D3DXVECTOR4& direction, D3DXCOLOR color);
// 
// 	//
// 	// Materials
// 	//
// 
// 	// Materials for objects (easy to send to shader) 
// 	OBJMATERIAL InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
// 	//D3DXVECTOR4 copyColorToVector(D3DXCOLOR color);
// 
// 	const OBJMATERIAL WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
// 	const OBJMATERIAL RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
// 	const OBJMATERIAL GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
// 	const OBJMATERIAL BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
// 	const OBJMATERIAL YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
// 
// }

#endif // __d3dH__