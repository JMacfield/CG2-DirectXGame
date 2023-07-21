#include "ImGuiManager.h"

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon_->GetDevice(),
		dxCommon_->GetBackBufferCount(),
		dxCommon_->GetRtvDesc().Format,
		dxCommon_->GetSrvHeap(),
		dxCommon_->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon_->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::Finalize() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ID3D12DescriptorHeap* descriptorHeaps[] = { dxCommon_->GetSrvHeap() };
	dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void ImGuiManager::End() {
	ImGui::Render();
}

void ImGuiManager::Draw() {
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon_->GetCommandList());
}