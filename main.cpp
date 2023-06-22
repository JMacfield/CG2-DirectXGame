#include "YTEngine.h"
#include "Triangle.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* win_ = nullptr;
	YTEngine* Engine = new YTEngine;
	
	Engine->Initialize(win_, 1280, 720);

	Engine->variableInitialize();

	while (true) {
		if (win_->ProcessMessage()) {
			break;
		}

		Engine->BeginFrame();
		Engine->Update();

		Engine->Draw();

		Engine->EndFrame();
	}

	Engine->Finalize();

	return 0;
}