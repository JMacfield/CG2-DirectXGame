#pragma once

#include "DirectXCommon.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "Triangle.h"
#include <Transform.h>

#pragma comment(lib,"dxcompiler.lib")

class YTEngine {
public:
	void variableInitialize();
	void Initialize(WinApp* win, int32_t width, int32_t height);
	void BeginFrame();
	void EndFrame();
	void Finalize();
	void Update();
	void Draw();

	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return textureSrvHandleGPU_; }

private:
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	static WinApp* win_;
	static DirectXCommon* direct_;

	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;
	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;
	ID3D12RootSignature* rootSignature_;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_{};
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	ID3D12PipelineState* graphicsPipelineState_;

	D3D12_VIEWPORT viewPort_{};
	D3D12_RECT scissorRect_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[2];

	Vector4 vertexData_;

	Transform vertexTransform_;
	Transform cameraTransform_;

	Matrix4x4 worldMatrix_;

	Triangle* triangle[3];
	Vector4 data1[3];
	Vector4 data2[3];
	Vector4 data3[3];

	Vector4 material[3];

	ID3D12Resource* textureResource;

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void InitializeDxcCompiler();
	void CreateRootSignature();
	void CreateInputlayOut();
	void SettingBlendState();
	void SettingRasterizerState();
	void InitializePSO();

	void SettingViewPort();
	void SettingScissor();

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	DirectX::ScratchImage SendTexture(const std::string& filePath);
	void LoadTexture(const std::string& filePath);
};