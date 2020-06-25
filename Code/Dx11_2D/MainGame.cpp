#include "stdafx.h"
#include "MainGame.h"

MainGame::MainGame()
	: m_pDirLightBuffer(NULL)
{
	//int nBreak_1 = _CrtSetBreakAlloc(714028);

	srand((UINT)time(NULL));
	whlCount = 0;
	
	worldColor = D3DXCOLOR(1, 1, 1, 1);
	CreateConstantBuffer(&m_pDirLightBuffer, sizeof(D3DXCOLOR), worldColor);

	settingMaxtrix();
	settingBlendOp();
	settingDepth();

#ifdef IMGUIAPI_DEBUG
	ImGui::Create(g_hWnd, Device, DeviceContext);
	ImGui::StyleColorsDark();
#endif

	//	pre create instance singleton class
	D2D, SOUND, SCENE, CAMERA, KEYMANAGER, IMAGEMAP, LOAD, DWRITE, BULLET, EFFECT, BDATA, EQUIP, MAP;

	//	Prepare the background images
	IMAGEMAP->InsertImageFile("bkGuard", "bkGuard.png");
	IMAGEMAP->InsertImageFile("LoadLogo", "LoadLogo.png");
	IMAGEMAP->InsertImageFile("Logo", "Logo.png");
}

MainGame::~MainGame()
{

}

void MainGame::Init()
{
	SCENE->createScene("LOGO", new LogoScene);
	SCENE->createScene("BATTLE", new BattleScene);
	SCENE->createScene("LOAD", new LoadScene);
	SCENE->createScene("LOBBY", new LobbyScene);
	SCENE->createScene("CHAPTER", new ChapterScene);
	SCENE->createScene("STORY", new StoryScene);
	SCENE->createScene("TUTORIAL", new TutorialScene);
	SCENE->createScene("EQUIP", new EquipScene);
	SCENE->createScene("SQUAD", new SquadEdditScene);
	SCENE->createScene("WORLD", new worldMapScene);
	SCENE->createScene("UNLOAD", new UnLoadScene);


	SCENE->createScene("TEST", new TestScene);

	SCENE->changeScene("LOGO", true);
	//SCENE->initScene("LOGO");
}

void MainGame::Update()
{
	CAMERA->update();
		SCENE->update_curScene();
	CAMERA->bufferUpdate();

	if (m_pDirLightBuffer)
	{
	#ifdef IMGUIAPI_DEBUG
		ImGui::DragFloat("worldColor_R", &worldColor.r, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_G", &worldColor.g, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_B", &worldColor.b, 0.01F, 0.0F, 1.0F);
		ImGui::DragFloat("worldColor_A", &worldColor.a, 0.01F, 0.0F, 1.0F);
	#endif
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
				DRAW->render("bkGuard", VEC2(WINSIZEX, WINSIZEY), VEC2(WINSIZEX *0.5f, WINSIZEY*0.5f));
				SCENE->render_curScene();
			}
			DeviceContext->OMSetBlendState(NormalBlendState, NULL, 0xFF);

		}

#ifdef IMGUIAPI_DEBUG
	ImGui::Render();
#endif

	SwapChain->Present(0, 0);
}

void MainGame::Release()
{
	SAFE_RELEASE(DepthEnable.depthStencilState);
	SAFE_RELEASE(DepthAble.depthStencilState);

	SCENE->releaseInstance();

	IMAGEMAP->releaseInstance();

	SOUND->releaseInstance();

	DRAW->releaseInstance();

	CAMERA->releaseInstance();

	KEYMANAGER->releaseInstance();

	D2D->releaseInstance();

	LOAD->releaseInstance();

	DWRITE->releaseInstance();

	SHADER->releaseInstance();

	BULLET->releaseInstance();

	EFFECT->releaseInstance();

	EQUIP->releaseInstance();

	MAP->releaseInstance();


#ifdef IMGUIAPI_DEBUG
	ImGui::Delete();
#endif
}

void MainGame::settingDepth()
{
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

}

void MainGame::settingBlendOp()
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

void MainGame::settingMaxtrix()
{
	//// Projection
	//D3DXMatrixPerspectiveFovLH(&vpMatrix->Projection, RAD(60), WINSIZEX / (float)WINSIZEY, 1000, 10000);
	//D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);
}