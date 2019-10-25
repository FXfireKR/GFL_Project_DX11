#include "stdafx.h"
#include "MainGame.h"


MainGame::MainGame()
{
	srand((UINT)GetTickCount64());

	FramePerSecond = 90.0f;

	// Create VPBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(tagVPMatrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &vpBuffer);
		assert(SUCCEEDED(hr));
	}

	// Matrix Setting
	{
		vpMatrix = new tagVPMatrix;

		// View
		D3DXVECTOR3 eye(0, 0, -1);
		D3DXVECTOR3 lookAt(0, 0, 0);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMatrixLookAtLH(&vpMatrix->View, &eye, &lookAt, &up);
		D3DXMatrixTranspose(&vpMatrix->View, &vpMatrix->View);

		// Projection
		D3DXMatrixOrthoOffCenterLH(&vpMatrix->Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
		D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);
	}

	// 喉坊靛 可记 积己
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Device->CreateBlendState(&desc, &NormalBlendState);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Device->CreateBlendState(&desc, &AlphaBlendState);
	}

	KEYMANAGER->init();
	SOUNDMANAGER->init();

	Init();
	ShaderInsert();
}

MainGame::~MainGame()
{
	SAFE_REL(vpBuffer);
	SAFE_DEL(vpMatrix);

	SOUNDMANAGER->release();
	SOUNDMANAGER->delInstance();

	SHADER->release();
	SHADER->delInstance();

	KEYMANAGER->delInstance();

	SCENE->release();
	SCENE->delInstance();
}

void MainGame::ShaderInsert()
{
	// Shader :: Sprite_alpha.hlsl
	D3D11_INPUT_ELEMENT_DESC PTLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	static UINT PTElementCount = ARRAYSIZE(PTLayoutDesc);

	SHADER->AddShaderFile(PTLayoutDesc, PTElementCount, "SPRITE2", L"../../_Shader/Sprite2_alpha.hlsl");

	D3D11_INPUT_ELEMENT_DESC PTLayoutDesc2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	PTElementCount = ARRAYSIZE(PTLayoutDesc2);

	SHADER->AddShaderFile(PTLayoutDesc2, PTElementCount, "RECT", L"../../_Shader/Rect.hlsl");
}

void MainGame::Init()
{
	SCENE->Create_Scene("TEST", new TestScene);
	SCENE->Init_Scene();

	SCENE->Change_Scene("TEST");

	SOUNDMANAGER->InsertSoundFile("test", "../../_SoundSource/Solom.mp3");
}

void MainGame::Update()
{
	ImGui::Update();
	FPSTIMER->update(60.0f);
	SCENE->Update_Scene();
	SOUNDMANAGER->setVolum();

	//ImGui::DragFloat("FPS", &FramePerSecond, 0.5f, 15.0f, 200.0f);
}

void MainGame::Render()
{
	D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float *)background);

	// Set VPMatrix Buffer
	DeviceContext->UpdateSubresource(vpBuffer, 0, NULL, vpMatrix, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &vpBuffer);

	DeviceContext->OMSetBlendState(AlphaBlendState, NULL, 0xFF);
	{	
		SCENE->Render_Scene();
	}
	DeviceContext->OMSetBlendState(NormalBlendState, NULL, 0xFF);

	ImGui::Render();
	SwapChain->Present(0, 0);
}