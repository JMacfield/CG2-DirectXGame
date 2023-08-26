#include "YTEngine.h"
#include "GameScene.h"

#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	CoInitializeEx(0, COINIT_MULTITHREADED);

	WinApp* winApp_ = nullptr;
	YTEngine* engine = new YTEngine;
	
	engine->Initialize(winApp_, 1280, 720);
	
	GameScene* gameScene = new GameScene();
	gameScene->Initialize(engine, engine->GetDirectXCommon());

	while (true) {
		if (winApp_->ProcessMessage()) {
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

	CoUninitialize();

	return 0;
}