#pragma once

#include "DirectXCommon.h"
#include "Vector4.h"
#include "YTEngine.h"
#include <string>

class Model {
public:
	void Initialize(DirectXCommon* directXCommon, YTEngine* engine, const std::string& directoryPath, const std::string& fileName, uint32_t index);

	void Draw(const Vector4& material, const Transform& transform, uint32_t texIndex, const Transform& cameraTransform, const DirectionalLight& light);

	void Finalize();

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	DirectXCommon* directXCommon_;
	YTEngine* engine_;

	ModelData modelData_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource;
	VertexData* vertexData_;

	ID3D12Resource* wvpResource_;
	TransformationMatrix* wvpData_;
	ID3D12Resource* materialResource_;

	Material* material_;

	DirectionalLight* directionalLight_;
	ID3D12Resource* directionalLightResource_;

private:
	void CreateVertexData();
	void SetColor();
	void TransformMatrix();
	void CreateDirectionalLight();
};