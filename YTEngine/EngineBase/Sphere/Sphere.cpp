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

void Sphere::Draw(const Vector4& material, const Matrix4x4& wvpData, uint32_t texIndex) {
	assert(texIndex < 2);

	const float kLonEvery = pi * 2.0f / float(kSubDivision);
	const float kLatEvery = pi / float(kSubDivision);

	for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubDivision + lonIndex);

			float lon = lonIndex * kLonEvery;

			//頂点a
			vertexData_[start].position = { cos(lat) * cos(lon),sin(lat),cos(lat) * sin(lon),1.0f };
			vertexData_[start].texcoord = { float(lonIndex) / float(kSubDivision),1.0f - float(latIndex) / kSubDivision };

			//頂点b
			vertexData_[start + 1].position = { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon),1.0f };
			vertexData_[start + 1].texcoord = { vertexData_[start].texcoord.x,vertexData_[start].texcoord.y - 1.0f / float(kSubDivision) };

			//頂点c
			vertexData_[start + 2].position = { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 2].texcoord = { vertexData_[start].texcoord.x + 1.0f / float(kSubDivision),vertexData_[start].texcoord.y };

			//頂点c
			vertexData_[start + 3].position = { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 3].texcoord = { vertexData_[start].texcoord.x + 1.0f / float(kSubDivision),vertexData_[start].texcoord.y };

			//頂点b
			vertexData_[start + 4].position = { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon),1.0f };
			vertexData_[start + 4].texcoord = { vertexData_[start].texcoord.x,vertexData_[start].texcoord.y - 1.0f / float(kSubDivision) };

			//頂点d
			vertexData_[start + 5].position = { cos(lat + kLatEvery) * cos(lon + kLonEvery),sin(lat + kLatEvery), cos(lat + kLatEvery) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 5].texcoord = { vertexData_[start].texcoord.x + 1.0f / float(kSubDivision),vertexData_[start].texcoord.y - 1.0f / float(kSubDivision) };
			
			*materialData_ = material;
			*wvpData_ = wvpData;

			directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
			directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
			directXCommon_->GetCommandList()->SetGraphicsRootShaderResourceView(1, wvpResource_->GetGPUVirtualAddress());
			directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2,engine_->textureSrvHandleGPU_[texIndex])

		}
	}
}