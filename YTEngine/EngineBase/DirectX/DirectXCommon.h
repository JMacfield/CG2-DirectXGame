#pragma once

#include <chrono>
#include <cstdint>
#include <dxgi1_6.h>

#include "WinApp.h"
#include "ConvertString.h"

#include "Externals/DirectXTex/DirectXTex.h"

class DirectXCommon {
public:
	void Initialize(WinApp* winApp, int32_t backBufferWidth = WinApp::kClientWidth,
		int32_t backBufferHeight = WinApp::kClientHeight);

	void PreDraw();
	void PostDraw();

	void ClearRenderTarget();

	void Finalize();

	HRESULT GetHr() { return hr_; }
	void SetHr(HRESULT a) { this->hr_ = a; }

	ID3D12Device* GetDevice() { return device_; }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	WinApp* GetWin() { return winApp_; }

	UINT GetBackBufferCount() { return swapChainDesc_.BufferCount; }

	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	ID3D12DescriptorHeap* GetSrvHeap() { return srvDescriptorHeap_; }
	ID3D12DescriptorHeap* GetDsvHeap() { return dsvDescriptorHeap_; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc_; }

private:
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	ID3D12DescriptorHeap* srvDescriptorHeap_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;
	ID3D12DescriptorHeap* rtvDescriptorHeap_;

	WinApp* winApp_;
	IDXGIAdapter4* useAdapter_;
	IDXGIFactory7* dxgiFactory_;
	ID3D12Device* device_;

	ID3D12CommandQueue* commandQueue_;
	ID3D12CommandAllocator* commandAllocator_;
	ID3D12GraphicsCommandList* commandList_;

	IDXGISwapChain4* swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

	ID3D12Resource* backBuffers_[2];
	int32_t backBufferWidth_;
	int32_t backBufferHeight_;

	UINT64 fenceVal_;
	D3D12_RESOURCE_BARRIER barrier_{};
	ID3D12Fence* fence_;
	HANDLE fenceEvent_;

	HRESULT hr_;

	ID3D12Resource* depthStencilResource_;

private:
	void InitializeDXGIDevice();
	void InitializeCommand();

	void CreateSwapChain();
	void CreateFinalRenderTargets();
	void CreateFence();
	void CreateSrvHeap();

	ID3D12Resource* CreateDepthStencilResource(ID3D12Device* deive, int32_t width, int32_t height);
	void CreateDepthStencil();
};