#pragma once

#include "DirectXCommon.h"

class ImGuiManager {
public:
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);
	void Finalize();
	void Begin();
	void End();
	void Draw();

private:
	DirectXCommon* dxCommon_;
};