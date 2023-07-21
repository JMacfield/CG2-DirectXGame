#pragma once

#include "DirectXCommon.h"
#include "MatrixCalculate.h"

class YTEngine;

class Triangle
{
public:
	void Initialize(DirectXCommon* direct, YTEngine* engine);
	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& wvpData);
	void Finalize();

private:
	void SettingVertex();
	void SetColor();
	void TransformMatrix();

private:
	ID3D12Resource* materialResource_;

	YTEngine* engine_;
	DirectXCommon* direct_;

	VertexData* vertexData_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Vector4* materialData_;
	ID3D12Resource* wvpResource_;
	Matrix4x4* wvpData_;
};