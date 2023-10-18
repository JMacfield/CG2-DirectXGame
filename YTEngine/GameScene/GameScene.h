#pragma once

#include "Vector4.h"
#include "Triangle.h"
#include "YTEngine.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"

struct TriangleVertex {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
};

class GameScene {
public:
	void Initialize(YTEngine* engine, DirectXCommon* directXCommon);
	void Update();
	void Draw2D();
	void Draw3D();
	void Finalize();

private:
	YTEngine* engine_;
	DirectXCommon* directXCommon_;

	Transform transform_;

	Transform modelTransform_;
	Vector4 modelMaterial_;

	Matrix4x4 worldMatrix_;
	Transform cameraTransform_;

	DirectionalLight directionalLight_;

	uint32_t uvResourceNumber_;
	int monsterBallResourceNumber_;

	Model* model_;
};