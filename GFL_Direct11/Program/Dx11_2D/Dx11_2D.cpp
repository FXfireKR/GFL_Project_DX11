// Dx11_2D.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Dx11_2D.h"
#include "MainGame.h"
#include <crtdbg.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE	g_hInst;											// 현재 인스턴스입니다.
HWND		g_hWnd;												// 윈도우 핸들
POINT		_ptMouse;

WCHAR		szTitle[MAX_LOADSTRING] = L"DIRECTX_2D API_v0601";		// 제목 표시줄 텍스트입니다.
MainGame*	pMainGame;
int KeyCode;
mutex			locker;

// Dx11 전역 변수:

IDXGIFactory* dxgiFactory;
IDXGIDevice* pDXGIDevice;
IDXGIAdapter *pAdapter;
ID2D1RenderTarget* d2Rtg;
ID2D1Factory* d2dFactory;
IDWriteFactory* dwFactory;
IDXGISurface* pBackBuffer;


IDXGISwapChain* SwapChain;			// 렌더 버퍼(모든 윈도우) 관리 변수 : 클리어, 비긴, 엔드, 프레젠트
ID3D11Device* Device;				// 하나의 장치(창)에 대한 버퍼, 텍스쳐 등 생성 관리를 위한 인터페이스 (CPU)
ID3D11DeviceContext* DeviceContext;	// 생성 된 리소스를 관리하기 위한 인터페이스 (GPU -> 텍스쳐 렌더링)
ID3D11RenderTargetView* RTV;		// 렌더타겟 지정 포인터

ID3D11VertexShader* VertexShader;	// 버텍스 셰이더
ID3D11PixelShader* PixelShader;		// 픽셀 셰이더
ID3D10Blob* VsBlob;					// 컴파일 된 버텍스 셰이더
ID3D10Blob* PsBlob;					// 컴파일 된 픽셀 셰이더


ID2D1Factory* factory;

D3DXVECTOR2	virtualPos;

// 컴파일(Compile) : 고급 언어(인간이 이해할 수 있는 언어 HLSL : High Level Shading Language) -> 기계어(컴퓨터 언어)

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
void                InitInstance(HINSTANCE, int);
void				InitDirectX(HINSTANCE);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 전역 문자열을 초기화합니다.
    MyRegisterClass(hInstance);
    // 응용 프로그램 초기화를 수행합니다:
	InitInstance(hInstance, nCmdShow);
	// DirectX 초기화
	InitDirectX(hInstance);

	pMainGame = new MainGame;
	ImGui::Create(g_hWnd, Device, DeviceContext);
	ImGui::StyleColorsDark();

    MSG msg;
	FPSTIMER->init();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			pMainGame->Update();
			pMainGame->Render();
		}
	}

	delete pMainGame;
	ImGui::Delete();

	FPSTIMER->release();
	FPSTIMER->delInstance();

	//SAFE_REL(Rendtrg);

	//_CrtDumpMemoryLeaks();

    return (int) msg.wParam;
}

/*
----------------------------------------------------------------------


		[	N O N E		M E M O R Y		L E A K		]


				>>>>	2 0 1 9 - 1 1 - 0 4

----------------------------------------------------------------------
*/


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;	//WNDCLASS : 윈도우의 정보를 저장하기 위한 구조체

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;									// 윈도우 스타일
    wcex.lpfnWndProc    = WndProc;													// 윈도우 프로시져
    wcex.cbClsExtra     = NULL;														// 클래스 여분 메모리
    wcex.cbWndExtra     = NULL;														// 윈도우 여분 메모리
    wcex.hInstance      = hInstance;												// 인스턴스
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX112D));			// 아이콘
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);							// 커서
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);									// 백그라운드
    wcex.lpszMenuName   = NULL;														// 메뉴 이름(NULL 메유 없앰)
    wcex.lpszClassName  = szTitle;													// 클래스 이름
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));		// 작은 아이콘

    return RegisterClassExW(&wcex);	// 윈도우 클래스 등록
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
void InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 화면 해상도 얻기
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);
	// 창 화면 중앙 배치 위치 계산
	int nWinPosX = nResolutionX / 2 - WINSIZEX / 2;
	int nWinPosY = nResolutionY / 2 - WINSIZEY / 2 - 40;

	HWND hWnd = CreateWindowW(
		szTitle,				// 윈도우 클래스 이름
		szTitle,				// 타이틀바에 띠울 이름
		WS_OVERLAPPEDWINDOW,	// 윈도우 스타일
		nWinPosX,				// 윈도우 화면 좌표 x
		nWinPosY,				// 윈도우 화면 좌표 y
		WINSIZEX,				// 윈도우 가로 사이즈
		WINSIZEY,				// 윈도우 세로 사이즈
		nullptr,				// 부모 윈도우
		nullptr,				// 메뉴 핸들
		hInstance,				// 인스턴스 지정
		nullptr);				// 자식 윈도우를 생성하면 지정해주고 그렇지 않으면 NULL

	assert(hWnd);

	g_hWnd = hWnd;				// 윈도우 핸들을 전역 변수에 저장

	// 윈도우 사이즈 조정 (타이틀바 및 메뉴 사이즈 실 사이즈에서 제외)
	RECT rt = { nWinPosX, nWinPosY, nWinPosX + WINSIZEX, nWinPosY + WINSIZEY };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	MoveWindow(g_hWnd, nWinPosX, nWinPosY, (rt.right - rt.left), (rt.bottom - rt.top), TRUE);

	ShowWindow(hWnd, nCmdShow);	// 화면에 창을 보여준다.
	UpdateWindow(hWnd);			// 창 업데이트
}

void InitDirectX(HINSTANCE hInstance)
{
	//Create g_pDevice and g_pDeviceContext, g_pSwapChain
	{
		HRESULT hr = S_OK;

		D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
			NULL, NULL, D3D11_SDK_VERSION, &Device, NULL, &DeviceContext);

		if (SUCCEEDED(hr))
			hr = Device->QueryInterface(&Device);
		
		if (SUCCEEDED(hr))
			hr = Device->QueryInterface(&pDXGIDevice);
		
		if (SUCCEEDED(hr))
			hr = pDXGIDevice->GetAdapter(&pAdapter);
		
		if (SUCCEEDED(hr))
			hr = pAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
		

		DXGI_SWAP_CHAIN_DESC swapDesc;
		::ZeroMemory(&swapDesc, sizeof(swapDesc));

		swapDesc.BufferDesc.Width = WINSIZEX;
		swapDesc.BufferDesc.Height = WINSIZEY;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = g_hWnd;
		swapDesc.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(Device, &swapDesc, &SwapChain);
		assert(SUCCEEDED(hr));
	}

	//Create BackBuffer
	{
		HRESULT hr;

		ID3D11Texture2D* BackBuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
		assert(SUCCEEDED(hr));

		// 백버퍼의 렌더타겟 뷰를 생성
		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV); // 뷰에서 엑세스 하는 리소스, 렌더 타겟 뷰의 정의, 렌더 타겟 뷰를 받아올 변수
		assert(SUCCEEDED(hr));
		BackBuffer->Release();

		// 렌더타겟 뷰를 Output-Merger의 렌더 타겟으로 설정
		DeviceContext->OMSetRenderTargets(1, &RTV, NULL); // 렌더타겟수, 렌더타겟 뷰의 배열, 렌더링 파이프 라인에 넘겨주는 깊이/스텐실 뷰의 포인터(깊이/스텐실 버퍼 설정 x)
	}

	//Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = WINSIZEX;
		viewport.Height = WINSIZEY;

		DeviceContext->RSSetViewports(1, &viewport);
	}

	// Disable DepthStencil
	{
		D3D11_DEPTH_STENCIL_DESC desc = { 0, };

		ID3D11DepthStencilState* depthStencilState;
		Device->CreateDepthStencilState(&desc, &depthStencilState);

		DeviceContext->OMSetDepthStencilState(depthStencilState, 0xFF);
		SAFE_REL(depthStencilState);
	}

	// Disable CullMode
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;

		ID3D11RasterizerState* rasterizerState;
		Device->CreateRasterizerState(&desc, &rasterizerState);

		DeviceContext->RSSetState(rasterizerState);
		SAFE_REL(rasterizerState);
	}
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc(hWnd, message, wParam, lParam))
		return true;

    switch (message)
    {
	case WM_KEYDOWN:	
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}

		KeyCode = wParam;
	
		break;

	case WM_MOUSEMOVE:
		_ptMouse.x = static_cast<float>(LOWORD(lParam));
		_ptMouse.y = static_cast<float>(HIWORD(lParam));
		//_ptMouse.y = WINSIZEY - _ptMouse.y;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}