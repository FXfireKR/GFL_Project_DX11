#pragma once
// Dx11 전역 변수


#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace D2D1;

extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

extern ID3D11VertexShader* VertexShader;
extern ID3D11PixelShader* PixelShader;
extern ID3D10Blob* VsBlob;
extern ID3D10Blob* PsBlob;

extern ID2D1RenderTarget* d2Rtg;
extern ID2D1Factory* d2dFactory;
extern IDWriteFactory* dwFactory;
extern IDXGISurface* pBackBuffer;

#include "Shader.h"
#include "DXRect.h"

struct tagVPMatrix
{
	D3DXMATRIX View;
	D3DXMATRIX Projection;
};