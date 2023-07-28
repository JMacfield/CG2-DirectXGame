#pragma once

#include "DirectXCommon.h"

class ImGuiManager {
public:
	void Initialize(WinApp* winApp, DirectXCommon* directXCommon);
	
	void Finalize();
	
	void Begin();
	
	void End();
	
	void Draw();

private:
	DirectXCommon* directXCommon_;
};