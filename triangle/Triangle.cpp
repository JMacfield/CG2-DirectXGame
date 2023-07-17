#include "Triangle.h"
#include <assert.h>
#include "YTEngine.h"

void Triangle::Initialize(DirectXCommon* direct, YTEngine* engine) {
	direct_ = direct;
	engine_ = engine;

	SettingVertex();
	SetColor();
	TransformMatrix();
}

void Triangle::SetColor() {
	materialResource_ = CreateBufferResource(direct_->GetDevice(),
		sizeof(VertexData));

	materialResource_->Map(0, nullptr,
		reinterpret_cast<void**>(&materialData_));
}

void Triangle::TransformMatrix() {
	wvpResource_ = CreateBufferResource(direct_->GetDevice(), sizeof(Matrix4x4));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();
}

void Triangle::Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& wvpData) {
	vertexData_[0].position = a;
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = b;
	vertexData_[1].texcoord = { 0.5f,0.0f };

	vertexData_[2].position = c;
	vertexData_[2].texcoord = { 1.0f,1.0f };

	*materialData_ = material;
	*wvpData_ = wvpData;

	direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	direct_->GetCommandList()->SetGraphicsRootDescriptorTable(2, engine_->GetSrvHandleGPU());

	direct_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::Finalize() {
	wvpResource_->Release();
	materialResource_->Release();
	vertexResource_->Release();
}

void Triangle::SettingVertex() {
	vertexResource_ = CreateBufferResource(direct_->GetDevice(),
		sizeof(VertexData) * 3);
	vertexBufferView_.BufferLocation =
		vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	vertexResource_->Map(0, nullptr,
		reinterpret_cast<void**>(&vertexData_));
}

ID3D12Resource* Triangle::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;

	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT hr;

	ID3D12Resource* resource = nullptr;

	hr = device->CreateCommittedResource(&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		,IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}