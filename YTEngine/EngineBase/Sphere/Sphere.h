#pragma once

#include "DirectXCommon.h"
#include "MatrixCalculate.h"
#include "YTEngine.h"

class Sphere {
public:
	void Initialize(DirectXCommon* directXCommon, YTEngine* engine);

	void Draw(const Vector4& material, const Matrix4x4& wvpData,uint32_t texIndex);

	void Finalize();

private:
	void CreateVertexData();
	void SetColor();
	void TransformMatrix();

private:
	YTEngine* engine_;
	DirectXCommon* directXCommon_;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource_;
	VertexData* vertexData_;
	
	ID3D12Resource* wvpResource_;
	Matrix4x4* wvpData_;
	ID3D12Resource* materialResource_;
	Vector4* materialData_;
	
	const float pi = 3.1415f;
	
	uint32_t kSubDivision;
	uint32_t vertexCount;
};