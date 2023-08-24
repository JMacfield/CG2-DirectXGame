#pragma once

#include "DirectXCommon.h"
#include "MatrixCalculate.h"

class YTEngine;

class Triangle
{
public:
	void Initialize(DirectXCommon* directXCommon, YTEngine* engine);
	
	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Transform& transform, const Transform& cameraTransform, const DirectionalLight& light);
	
	void Finalize();

private:
	YTEngine* engine_;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	
	DirectXCommon* directXCommon_;

	VertexData* vertexData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

private:
	void SettingVertex();
	void SetColor();
	void TransformMatrix();
	void CreateDirectionalLight();
};