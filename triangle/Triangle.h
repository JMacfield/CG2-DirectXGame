#pragma once

#include "DirectXCommon.h"
#include "Vector4.h"

class ArnEngine;

class Triangle
{
public:
	void Initialize(DirectXCommon* direct);
	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4 & material);
	void Finalize();

private:
	void SettingVertex();
	void SetColor();

private:
	ID3D12Resource* materialResource_;

	ArnEngine* Engine;
	DirectXCommon* direct_;
	Vector4* vertexData_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte);
	Vector4* materialData_;
};