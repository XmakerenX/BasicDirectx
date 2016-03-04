#include "GameWin.h"

//-----------------------------------------------------------------------------
// Name : CGameWin (constructor)
//-----------------------------------------------------------------------------
CGameWin::CGameWin(HINSTANCE hInstance)
: m_hWnd(nullptr),
  m_hGameInst(hInstance),
  m_pD3DDevice(nullptr),
  m_pD3dDeviceContext(nullptr),
  m_d3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
  m_SwapChain(nullptr),
  m_depthStencilBuffer(nullptr),
  m_renderTargetView(nullptr),
  m_depthStencilView(nullptr),
  m_4xMsaaQuality(0),
  m_Enable4xMsaa(false),
  m_clientWitdh(800),
  m_clientHeight(600),
  m_gameRunning(true)
{
	ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
}

//-----------------------------------------------------------------------------
// Name : CGameWin (destructor)
//-----------------------------------------------------------------------------
CGameWin::~CGameWin()
{
	Release();
}

//-----------------------------------------------------------------------------
// Name : InitGame ()
//-----------------------------------------------------------------------------
bool CGameWin::InitGame()
{
	if (!CreateGameWindow())
		return false;

	if (!CreateDevice())
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateGameWindow ()
//-----------------------------------------------------------------------------
bool CGameWin::CreateGameWindow()
{
	// The Window Struct
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hGameInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Directx11Demo";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT rc = { 0, 0, m_clientWitdh, m_clientHeight };
	::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	int windowWidth = rc.right - rc.left;
	int windowhHight = rc.bottom - rc.top;

	m_hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE, L"Directx11Demo", L"Directx11Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowhHight, NULL, NULL, m_hGameInst, (void*)this);

	if (!m_hWnd)
	{
		::MessageBox(0, L"Failed To Create Window.", 0, 0);
		return false;
	}

	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd);

	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDevice ()
// TODO : add documentation I can't understand shit....
//-----------------------------------------------------------------------------
bool CGameWin::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevel; 
	DXGI_FORMAT formatType = DXGI_FORMAT_R8G8B8A8_UNORM;

	IDXGIFactory * pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

	IDXGIAdapter* pAdapter;
	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		ADAPTERINFO curAdapterInfo;
		curAdapterInfo.adapterNum = i;

		IDXGIOutput* pOutput = nullptr;
		HR(pAdapter->EnumOutputs(0, &pOutput));

		DXGI_OUTPUT_DESC outputDesc;
		pOutput->GetDesc(&outputDesc);
		curAdapterInfo.adapterDescription = outputDesc.DeviceName;

		UINT nModes;
		DXGI_MODE_DESC* displayModes = nullptr;

		pOutput->GetDisplayModeList(formatType, 0, &nModes, nullptr);

		displayModes = new DXGI_MODE_DESC[nModes];

		pOutput->GetDisplayModeList(formatType, 0, &nModes, displayModes);

		for (UINT j = 0; j < nModes; ++j)
		{
			curAdapterInfo.addDisplayMode(displayModes[j]);
			
		}

		m_adpatersInfo.push_back(curAdapterInfo);
	}

	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		m_d3dDriverType,
		0,                 // no software device
		0,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_pD3DDevice,
		&featureLevel,
		&m_pD3dDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice() Failed.", 0, 0);
		return false; 
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	//TODO: if there is no nead for 4xmsaa don't make program on fail on it 
	HR(m_pD3DDevice->CheckMultisampleQualityLevels(formatType, 4, &m_4xMsaaQuality));
	assert(m_4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_clientWitdh;
	sd.BufferDesc.Height = m_clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = formatType;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice* dxgiDevice = 0;
	HR(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetAdapter(&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_SwapChain));

	if (dxgiDevice)
	{
		dxgiDevice->Release();
		dxgiDevice = nullptr;
	}

	if (dxgiAdapter)
	{
		dxgiAdapter->Release();
		dxgiAdapter = nullptr;
	}

	if (dxgiFactory)
	{
		dxgiFactory->Release();
		dxgiFactory = nullptr;
	}

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.
	ResetDevice();

	return true;
}

//-----------------------------------------------------------------------------
// Name : ResetDevice ()
//-----------------------------------------------------------------------------
void CGameWin::ResetDevice()
{
	assert(m_pD3dDeviceContext);
	assert(m_pD3DDevice);
	assert(m_SwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	// Resize the swap chain and recreate the render target view.
	HR(m_SwapChain->ResizeBuffers(1, m_clientWitdh, m_clientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* backBuffer;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_pD3DDevice->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView));

	if (backBuffer)
	{
		backBuffer->Release();
		backBuffer = nullptr;
	}

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_clientWitdh;
	depthStencilDesc.Height = m_clientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView));

	// Bind the render target view and depth/stencil view to the pipeline.
	m_pD3dDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Set the viewport transform.
	m_screenViewport.TopLeftX = 0;
	m_screenViewport.TopLeftY = 0;
	m_screenViewport.Width = static_cast<float>(m_clientWitdh);
	m_screenViewport.Height = static_cast<float>(m_clientHeight);
	m_screenViewport.MinDepth = 0.0f;
	m_screenViewport.MaxDepth = 1.0f;

	m_pD3dDeviceContext->RSSetViewports(1, &m_screenViewport);
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
//-----------------------------------------------------------------------------
int CGameWin::BeginGame()
{
	MSG		msg = { 0 };

	m_timer.reset();

	// Start main loop
	while (m_gameRunning)
	{
		// Did we receive a message, or are we idling ?
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_gameRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Advance Game Frame.
		m_timer.frameAdvanced();
		DrawScene();
		//FrameAdvance(m_timer->getTimeElapsed());
		// End If messages waiting
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
//-----------------------------------------------------------------------------
bool CGameWin::ShutDown()
{
	Release();

	// Destroy the render window
	if (m_hWnd) DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	// Shutdown Success
	return true;
}

//-----------------------------------------------------------------------------
// Name : Release ()
//-----------------------------------------------------------------------------
void CGameWin::Release()
{
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain = false;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_pD3dDeviceContext)
	{
		m_pD3dDeviceContext->ClearState();
		m_pD3dDeviceContext->Release();
		m_pD3dDeviceContext = nullptr;
	}

	// Destroy Direct3D Objects
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;

	}

}

//-----------------------------------------------------------------------------
// Name : UpdateScene ()
//-----------------------------------------------------------------------------
void CGameWin::UpdateScene(float deltaTime)
{

}

//-----------------------------------------------------------------------------
// Name : DrawScene ()
//-----------------------------------------------------------------------------
void CGameWin::DrawScene()
{
	assert(m_pD3dDeviceContext);
	assert(m_SwapChain); 

	m_pD3dDeviceContext->ClearRenderTargetView(m_renderTargetView, reinterpret_cast<const float*>(&Colors::Green));
	m_pD3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	HR(m_SwapChain->Present(0, 0));
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc 
// Desc : forward the message to the proper instance of the class 
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameWin::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if (Message == WM_CREATE) SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameWin *Destination = (CGameWin*)GetWindowLong(hWnd, GWL_USERDATA);

	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->MsgProc(hWnd, Message, wParam, lParam);

	// No destination found, defer to system...
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name : MsgProc
//-----------------------------------------------------------------------------
LRESULT CGameWin::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_gamePaused = true;
			m_timer.stop();
		}
		else
		{
			m_gamePaused = false;
			m_timer.start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_clientWitdh = LOWORD(lParam);
		m_clientHeight = HIWORD(lParam);
		if (m_pD3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_gamePaused = true;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_gamePaused = false;
				m_minimized = false;
				m_maximized = true;
				ResetDevice();
				//OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_minimized)
				{
					m_gamePaused = false;
					m_minimized = false;
					ResetDevice();
					//OnResize();
				}

				// Restoring from maximized state?
				else if (m_maximized)
				{
					m_gamePaused = false;
					m_maximized = false;
					ResetDevice();
					//OnResize();
				}
				else if (m_resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					ResetDevice();
					//OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_gamePaused = true;
		m_resizing = true;
		m_timer.stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_gamePaused = false;
		m_resizing = false;
		m_timer.start();
		ResetDevice();
		//OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		//OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}