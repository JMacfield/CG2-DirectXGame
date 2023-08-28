#include "YTEngine.h"
#include "GameScene.h"
#include <dxgidebug.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	CoInitializeEx(0, COINIT_MULTITHREADED);

	WinApp* winApp = nullptr;
	YTEngine* engine = new YTEngine;
	
	engine->Initialize(winApp, 1280, 720);
	
	GameScene* gameScene = new GameScene();
	gameScene->Initialize(engine, engine->GetDirectXCommon());

	while (true) {
		if (winApp->ProcessMessage()) {
			break;
		}

		engine->BeginFrame();

		gameScene->Update();

		gameScene->Draw3D();
		gameScene->Draw2D();

		engine->Draw();

		engine->EndFrame();
	}

	gameScene->Finalize();
	engine->Finalize();

	delete gameScene;
	CoUninitialize();
	delete engine;

	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}

	return 0;
}