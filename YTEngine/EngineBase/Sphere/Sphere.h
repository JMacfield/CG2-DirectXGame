#pragma once

#include "DirectXCommon.h"
#include "MatrixCalculate.h"
#include "YTEngine.h"

class Sphere {
public:
	void Initialize(DirectXCommon* directXCommon, YTEngine* engine);

	void Draw(const Vector4& material, const Transform& wvpdata, uint32_t texIndex, const Transform& cameratransform, const DirectionalLight& light);

	void Finalize();

private:
	YTEngine* engine_;
	DirectXCommon* directXCommon_;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource_;
	VertexData* vertexData_;
	
	ID3D12Resource* wvpResource_;
	TransformationMatrix* wvpData_;
	ID3D12Resource* materialResource_;
	Material* materialData_;
	
	const float pi = 3.1415f;
	
	uint32_t kSubDivision;
	uint32_t vertexCount;

	DirectionalLight* directionalLight_;
	ID3D12Resource* directionalLightResource_;

private:
	void CreateVertexData();
	void SetColor();
	void TransformMatrix();
	void CreateDirectionalLight();
};