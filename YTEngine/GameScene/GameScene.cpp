#include "GameScene.h"

void GameScene::Initialize(YTEngine* engine, DirectXCommon* directXCommon) {
	engine_ = engine;
	directXCommon_ = directXCommon;

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,1.6f,0.0f},{0.0f,0.0f,0.0f} };
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	model_ = new Model();
	model_->Initialize(directXCommon_, engine_, "Resource", "plane.obj", 0, directionalLight_);

	uvResourceNumber_ = 0;
	engine_->LoadTexture("Resource/uvChecker.png",uvResourceNumber_);

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

}

void GameScene::Update() {
	transform_.rotate.y += 0.01f;
	directionalLight_.direction = Normalize(directionalLight_.direction);

	ImGui::Begin("Window");
	ImGui::DragFloat3("CameraTranslate", &cameraTransform_.translate.x, 0.01f);
	ImGui::DragFloat3("ModelMaterial", &modelMaterial_.x, 0.01f);
	ImGui::DragFloat3("ModelScale", &modelTransform_.scale.x, 0.01f);
	ImGui::DragFloat3("ModelRotate", &modelTransform_.rotate.x, 0.01f);
	ImGui::DragFloat3("ModelTranslate", &modelTransform_.translate.x, 0.01f);
	ImGui::End();
}

void GameScene::Draw3D() {	
	model_->Draw(modelMaterial_, modelTransform_, 0, cameraTransform_);
}

void GameScene::Draw2D() {
}

void GameScene::Finalize() {
	delete model_;
}