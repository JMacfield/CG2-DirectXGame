#include "Sphere.h"
#include <assert.h>
#include "YTEngine.h"

void Sphere::Initialize(DirectXCommon* directXCommon, YTEngine* engine) {
	directXCommon_ = directXCommon;
	engine_ = engine;

	SettingVertex();
	SetColor();
	TransformMatrix();
}

void Sphere::SetColor() {
	materialResource_ = DirectXCommon::CreateBufferResource(
		directXCommon_->GetDevice(), sizeof(VertexData));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Sphere::TransformMatrix() {
	wvpResource_ = DirectXCommon::CreateBufferResource(directXCommon_->GetDevice(), sizeof(Matrix4x4));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();
}

void Sphere::Draw() {

}