#include "stdafx.h"
#include "MainGame.h"

MainGame::MainGame()
	: m_pDirLightBuffer(NULL)
{
	srand((UINT)GetTickCount64());
	
	whlCount = 0;
	worldColor = D3DXCOLOR(1, 1, 1, 1);

	D3DXCOLOR color;
	CreateConstantBuffer(&m_pDirLightBuffer, sizeof(D3DXCOLOR), color);

	//	// Projection
	//	//D3DXMatrixPerspectiveFovLH(&vpMatrix->Projection, RAD(60), WINSIZEX / (float)WINSIZEY, 1000, 10000);
	//	//D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);


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
	SHADER->getInstance();
	DRAW->init();
	EQUIP;

	ThreadPool* beforeGame = new ThreadPool;
	beforeGame->SetThread(8);
	IMAGEMAP->InsertImageBinary(beforeGame, "LoadBK", "../../_Assets/Texture2D/LoadImg_First.ab");
	//IMAGEMAP->InsertImageBinary(beforeGame, "Logo", "../../_Assets/Texture2D/Logo.ab");
	//IMAGEMAP->InsertImageBinary(beforeGame, "LogoFont", "../../_Assets/Texture2D/Logo_font.ab");
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
	SCENE->Create_Scene("LOGO", new MainLoadScene);

	SCENE->Create_Scene("BATTLE", new BattleScene);
	SCENE->Create_Scene("LOAD", new LoadScene);
	SCENE->Create_Scene("LOBBY", new LobbyScene);
	SCENE->Create_Scene("CHAPTER", new ChapterScene);
	SCENE->Create_Scene("STORY", new StoryScene);
	SCENE->Create_Scene("TUTORIAL", new TutorialScene);
	SCENE->Create_Scene("EQUIP", new EquipScene);
	SCENE->Create_Scene("SQUAD", new SquadEdditScene);
	SCENE->Create_Scene("WORLD", new worldmapScene);
	//

	//SCENE->Change_Scene("BATTLE");
	SCENE->Change_Scene("LOGO");
	SCENE->Init_Scene();
}

void MainGame::Update()
{
	CAMERA->update();
	SCENE->Update_Scene();
	CAMERA->bufferUpdate();

	if (m_pDirLightBuffer)
	{
		ImGui::DragFloat("worldColor_R", &worldColor.r, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_G", &worldColor.g, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_B", &worldColor.b, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_A", &worldColor.a, 0.01F, 0.0F, 1.0F);

		DeviceContext->UpdateSubresource(m_pDirLightBuffer, 0, 0, &worldColor, 0, 0);
	}

}

void MainGame::Render()
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr = SwapChain->GetDesc(&swapDesc);

	if (SUCCEEDED(hr))
		if (d2Rtg)
		{
			D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);

			DeviceContext->ClearRenderTargetView(RTV, (float *)background);
			DeviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
			DeviceContext->OMSetDepthStencilState(DepthEnable.depthStencilState, 1);
		
			// Set VPMatrix Buffer
			CAMERA->setCameraBuffer();
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
	SAFE_RELEASE(DepthEnable.depthStencilState);
	SAFE_RELEASE(DepthAble.depthStencilState);

	ImGui::Delete(); // ImGui 해제

	//SAFE_DEL(vpMatrix);

	//	Release Singleton Memory
	D2DX->delInstance();
	TEXT->release();
	TEXT->delInstance();
	SHADER->delInstance();	
	SCENE->delInstance();
	IMAGEMAP->delInstance();
	SOUNDMANAGER->delInstance();
	MAP->delInstance();
	LOADMANAGER->delInstance();
	GAMETIME->delInstance();
	DRAW->delInstance();
	BULLET->delInstance();
	EFFECT->delInstance();
	CAMERA->delInstance();
	
	EQUIP->delInstance();

	DAMAGE->release();
	DAMAGE->delInstance();

	KEYMANAGER->delInstance();
}

void MainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}