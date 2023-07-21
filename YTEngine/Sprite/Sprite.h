#pragma once

#include "DirectXCommon.h"
#include "Vector4.h"
#include "MatrixCalculate.h"

class Sprite {
public:
	void Initialize(DirectXCommon* directXCommon);
	void Draw(Vector4 a, Vector4 b, Transform transform);
	void Finalize();

private:
	DirectXCommon* directXCommon_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource;
	VertexData* vertexData_;
	ID3D12Resource* transformationMatrixResource;
	Matrix4x4 transformationMatrixData;

private:
	void CreateVertexData();
	void CreateTransform();
};