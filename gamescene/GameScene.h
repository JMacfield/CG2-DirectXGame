#pragma once

#include "Vector4.h"
#include "Triangle.h"
#include "YTEngine.h"

class GameScene {
public:
	void Initialize(YTEngine* engine, DirectXCommon* direct);
	void Update();
	void Draw2D();
	void Draw3D();
	void Finalize();

private:
	YTEngine* engine_;
	DirectXCommon* directX_;
	
};