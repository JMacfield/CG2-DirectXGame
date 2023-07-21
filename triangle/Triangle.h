#pragma once

#include "DirectXCommon.h"
#include "Vector4.h"

class YTEngine;

class Triangle
{
public:
	void Initialize(DirectXCommon* direct, const Vector4& a, const Vector4& b, const Vector4& c);
	void Draw();
	void Finalize();

private:
	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c);

private:
	YTEngine* Engine;
	DirectXCommon* direct_;
	Vector4* vertexData_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};