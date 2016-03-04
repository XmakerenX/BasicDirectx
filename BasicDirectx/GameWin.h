#ifndef  _GAMEWIN_H
#define  _GAMEWIN_H

#include <assert.h>
#include <windows.h>
#include <d3d11.h>

#include <string>
#include <vector>

#include "d3d.h"
#include "Timer.h"

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)																										\
	{																													\
		HRESULT hr = (x);																								\
		if (FAILED(hr))																									\
		{																												\
			LPWSTR output;																								\
																														\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,	\
				nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&output, 0, nullptr);					\
			MessageBox(nullptr, output, L"Error", MB_OK);																\
		}																												\
	}
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

struct DEVICETYPEINFO
{
	enum MODE{ WINDOWED, FULLSCREEN };

	D3D_DRIVER_TYPE deviceType;
	std::string deviceDescription;
	DXGI_FORMAT fomratType;

	//bool bHardwareAcceleration[2];
	//BOOL bDepthEnable[2];
	//std::vector<D3DFORMAT> validDepths[2];
	//std::vector<D3DMULTISAMPLE_TYPE> validMultiSampleTypes[2];
	//std::vector<DWORD> vpTypes;
};

struct DISPLAYMODE
{
	UINT Width;
	UINT Height;
	std::vector<DXGI_RATIONAL> RefreshRates;
	DXGI_FORMAT Format;

	DISPLAYMODE::DISPLAYMODE(DXGI_MODE_DESC& displayMode)
	{
		this->Format = displayMode.Format;
		this->Height = displayMode.Height;
		this->RefreshRates.push_back(displayMode.RefreshRate);
		this->Width = displayMode.Width;
	}
};

struct ADAPTERINFO
{
	UINT adapterNum;
	std::wstring adapterDescription;

	//std::vector<DEVICETYPEINFO> deviceTypes;
	std::vector<DISPLAYMODE> displayModes;

	void addDisplayMode(DXGI_MODE_DESC& displayMode)
	{
		for (UINT i = 0; i < displayModes.size(); i++)
		{
			if (displayMode.Width == displayModes[i].Width && displayMode.Height == displayModes[i].Height)
			{
				displayModes[i].RefreshRates.push_back(displayMode.RefreshRate);
				return;
			}
		}

		displayModes.push_back(DISPLAYMODE(displayMode));
	}
};

class CGameWin
{
public:
	CGameWin(HINSTANCE hInstance);
	virtual ~CGameWin();

	bool InitGame();
	bool CreateGameWindow();
	bool CreateDevice();
	void ResetDevice();

	int  BeginGame();
	bool ShutDown();

	void Release();

	void UpdateScene(float deltaTime);
	void DrawScene();

	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	HWND		m_hWnd;
	HINSTANCE	m_hGameInst;		

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pD3dDeviceContext;
	D3D_DRIVER_TYPE m_d3dDriverType;

	std::vector<ADAPTERINFO> m_adpatersInfo;

	IDXGISwapChain* m_SwapChain;

	ID3D11Texture2D*		m_depthStencilBuffer;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;

	D3D11_VIEWPORT m_screenViewport;

	UINT      m_4xMsaaQuality;
	bool	  m_Enable4xMsaa;

	int m_clientWitdh;
	int m_clientHeight;

	bool m_gameRunning;
	bool m_gamePaused;

	bool m_minimized;
	bool m_maximized;
	bool m_resizing;

	CTimer m_timer;
	
};

#endif  //_GAMEWIN_H