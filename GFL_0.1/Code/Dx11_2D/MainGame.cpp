#include "stdafx.h"
#include "MainGame.h"

MainGame::MainGame()
	: m_pDirLightBuffer(NULL)
{
	srand((UINT)GetTickCount64());

	whlCount = 0;

	// Create VPBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ViewProjMatrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &vpBuffer);
		assert(SUCCEEDED(hr));
	}

	D3DXCOLOR color;
	CreateConstantBuffer(&m_pDirLightBuffer, sizeof(D3DXCOLOR), color);

	// Matrix Setting
	{
		vpMatrix = new ViewProjMatrix;

		// View
		D3DXVECTOR3 eye(0, 0, -5);
		D3DXVECTOR3 lookAt(0, 0, 0);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMatrixLookAtLH(&vpMatrix->View, &eye, &lookAt, &up);
		D3DXMatrixTranspose(&vpMatrix->View, &vpMatrix->View);

		// Projection
		//D3DXMatrixPerspectiveFovLH(&vpMatrix->Projection, RAD(60), WINSIZEX / (float)WINSIZEY, 1000, 10000);
		//D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);

		// Projection
		D3DXMatrixOrthoOffCenterLH(&vpMatrix->Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
		D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);
	}

	// 블렌드 옵션 생성
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

	//	Setting Depth Able
	{
		ZeroMemory(&DepthAble.depthStencilDesc, sizeof(DepthAble.depthStencilDesc));

		DepthAble.depthStencilDesc.DepthEnable = true;
		DepthAble.depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthAble.depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		DepthAble.depthStencilDesc.StencilEnable = true;
		DepthAble.depthStencilDesc.StencilReadMask = 0xFF;
		DepthAble.depthStencilDesc.StencilWriteMask = 0xFF;

		DepthAble.depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthAble.depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		DepthAble.depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthAble.depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DepthAble.depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthAble.depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		DepthAble.depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthAble.depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		Device->CreateDepthStencilState(&DepthAble.depthStencilDesc, 
			&DepthAble.depthStencilState);
	}

	//	Setting Depth Enable
	{
		ZeroMemory(&DepthEnable.depthStencilDesc, sizeof(DepthEnable.depthStencilDesc));

		DepthEnable.depthStencilDesc.DepthEnable = false;
		DepthEnable.depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthEnable.depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		DepthEnable.depthStencilDesc.StencilEnable = true;
		DepthEnable.depthStencilDesc.StencilReadMask = 0xFF;
		DepthEnable.depthStencilDesc.StencilWriteMask = 0xFF;

		DepthEnable.depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthEnable.depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		DepthEnable.depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthEnable.depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DepthEnable.depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthEnable.depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		DepthEnable.depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthEnable.depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		Device->CreateDepthStencilState(&DepthEnable.depthStencilDesc,
			&DepthEnable.depthStencilState);
	}

	ImGui::Create(g_hWnd, Device, DeviceContext);	// ImGui 생성
	ImGui::StyleColorsDark();						// 스타일 선택

	// 매니져 초기화
	KEYMANAGER->Init();
	D2DX->init();
	TEXT->DWInit();
	SOUNDMANAGER->init();

	ThreadPool* beforeGame = new ThreadPool;
	beforeGame->SetThread(8);
	IMAGEMAP->InsertImageBinary(beforeGame, "LoadBK", "../../_Assets/Texture2D/LoadImg_First.ab");
	IMAGEMAP->InsertImageBinary(beforeGame, "Logo", "../../_Assets/Texture2D/Logo.ab");
	IMAGEMAP->InsertImageBinary(beforeGame, "LogoFont", "../../_Assets/Texture2D/Logo_font.ab");
	IMAGEMAP->InsertImageBinary(beforeGame, "AR_BLT", "../../_Assets/Texture2D/arbullet.ab");
	IMAGEMAP->InsertImageBinary(beforeGame, "MG_BLT", "../../_Assets/Texture2D/mgBullet.ab");
	//IMAGEMAP->InsertImageBinary(beforeGame, "LogoFont", "../../_Assets/Texture2D/Logo_font.ab");

	beforeGame->release();
	SAFE_DELETE(beforeGame);
}

MainGame::~MainGame()
{

}

void MainGame::Init()
{

	SCENE->Create_Scene("BATTLE", new BattleScene);
	SCENE->Create_Scene("FirstLoad", new MainLoadScene);
	SCENE->Create_Scene("LOAD", new LoadScene);
	SCENE->Create_Scene("LOBBY", new LobbyScene);
	//SCENE->Create_Scene("STORY", new StoryScene);
	//SCENE->Create_Scene("WORLD", new WorldScene);

	SCENE->Change_Scene("LOBBY");
	SCENE->Init_Scene();
}

void MainGame::Update()
{
	const float Speed = 450.0f;

	{
		if (KEYMANAGER->isKeyStayDown(VK_LEFT))
			CameraPositionX -= GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_RIGHT))
			CameraPositionX += GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_DOWN))
			CameraPositionY -= GAMETIME->DeltaTime() * Speed;

		else if (KEYMANAGER->isKeyStayDown(VK_UP))
			CameraPositionY += GAMETIME->DeltaTime() * Speed;
	}

	if (CameraPositionX < 0)
		CameraPositionX = 0;

	SCENE->Update_Scene();

	D3DXVECTOR3 eye(CameraPositionX, CameraPositionY, -5);
	D3DXVECTOR3 lookAt(CameraPositionX, CameraPositionY, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&vpMatrix->View, &eye, &lookAt, &up);
	D3DXMatrixTranspose(&vpMatrix->View, &vpMatrix->View);


	if (m_pDirLightBuffer)
	{
		static D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1);

		ImGui::DragFloat("worldColor_R", &color.r, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_G", &color.g, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_B", &color.b, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_A", &color.a, 0.01F, 0.0F, 1.0F);

		DeviceContext->UpdateSubresource(m_pDirLightBuffer, 0, 0, &color, 0, 0);
	}

}

void MainGame::Render()
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr = SwapChain->GetDesc(&swapDesc);

	//g_pTimeManager->Render();

	if (SUCCEEDED(hr))
		if (d2Rtg)
		{
			D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);

			DeviceContext->ClearRenderTargetView(RTV, (float *)background);
			DeviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
			DeviceContext->OMSetDepthStencilState(DepthEnable.depthStencilState, 1);
		
			// Set VPMatrix Buffer
			DeviceContext->UpdateSubresource(vpBuffer, 0, NULL, vpMatrix, 0, 0);
			DeviceContext->VSSetConstantBuffers(0, 1, &vpBuffer);

			DeviceContext->PSSetConstantBuffers(1, 1, &m_pDirLightBuffer);

			DeviceContext->OMSetBlendState(AlphaBlendState, NULL, 0xFF);			
			{
				SCENE->Render_Scene();
			}
			DeviceContext->OMSetBlendState(NormalBlendState, NULL, 0xFF);

		}

	ImGui::Render();
	SwapChain->Present(0, 0);
}

void MainGame::Release()
{
	ImGui::Delete(); // ImGui 해제

	SAFE_DEL(vpMatrix);

	//	Release Singleton Memory
	D2DX->delInstance();
	TEXT->release();
	TEXT->delInstance();
	SHADER->delInstance();	

	KEYMANAGER->delInstance();
}

void MainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}