#include "YTEngine.h"
#include <cassert>

IDxcBlob* YTEngine::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	direct_->SetHr(dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource));
	
	assert(SUCCEEDED(direct_->GetHr()));
	
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od", 
		L"-Zpr",
	};

	IDxcResult* shaderResult = nullptr;
	direct_->SetHr(dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)
	));

	assert(SUCCEEDED(direct_->GetHr()));

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}

	IDxcBlob* shaderBlob = nullptr;
	direct_->SetHr(shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));
	assert(SUCCEEDED(direct_->GetHr()));
	
	Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));
	
	shaderSource->Release();
	shaderResult->Release();
	
	return shaderBlob;
}

void YTEngine::InitializeDxcCompiler() {
	HRESULT hr;
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
	//現時点でincludeはしないが、includeに対応するための設定を行っていく
	includeHandler_ = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void YTEngine::CreateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;

	HRESULT hr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);

	if (FAILED(direct_->GetHr())) {
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	
	rootSignature_ = nullptr;
	hr = direct_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void YTEngine::CreateInputlayOut() {
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void YTEngine::SettingBlendState() {
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void YTEngine::SettingRasterizerState() {
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	vertexShaderBlob_ = CompileShader(L"./shader/Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);


	pixelShaderBlob_ = CompileShader(L"./shader/Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);
}

void YTEngine::InitializePSO() {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_;
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;

	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
		vertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
		pixelShaderBlob_->GetBufferSize() };

	graphicsPipelineStateDesc.BlendState = blendDesc_;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;
	
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	
	graphicsPipelineState_ = nullptr;
	HRESULT hr = direct_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

//void YTEngine::SettingVertex() {
//	D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
//	uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	D3D12_RESOURCE_DESC vertexResourceDesc{};
//	
//	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	vertexResourceDesc.Width = sizeof(Vector4) * 3;
//	
//	vertexResourceDesc.Height = 1;
//	vertexResourceDesc.DepthOrArraySize = 1;
//	vertexResourceDesc.MipLevels = 1;
//	vertexResourceDesc.SampleDesc.Count = 1;
//	
//	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	HRESULT hr;
//
//	hr = direct_->GetDevice()->CreateCommittedResource(&uplodeHeapProperties, D3D12_HEAP_FLAG_NONE,
//		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&vertexResource_));
//	assert(SUCCEEDED(hr));
//	
//	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
//	
//	vertexBufferView_.SizeInBytes = sizeInBytes;
//	
//	vertexBufferView_.StrideInBytes = sizeof(Vector4);
//	
//	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
//}

void YTEngine::SettingViewPort() {
	viewPort_.Width = WinApp::kClientWidth;
	viewPort_.Height = WinApp::kClientHeight;
	viewPort_.TopLeftX = 0;
	viewPort_.TopLeftY = 0;
	viewPort_.MinDepth = 0.0f;
	viewPort_.MaxDepth = 1.0f;
}

void YTEngine::SettingScissor() {
	scissorRect_.left = 0;
	scissorRect_.right = WinApp::kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApp::kClientHeight;
}

void YTEngine::variableInitialize() {
	data1[0] = { -0.5f,-0.1f,0.0f,1.0f };
	data2[0] = { -0.45f,0.1f,0.0f,1.0f };
	data3[0] = { -0.4f,-0.1f,0.0f,1.0f };

	//data1[1] = { -0.2f,-0.3f,0.0f,1.0f };
	//data2[1] = { -0.15f,-0.1f,0.0f,1.0f };
	//data3[1] = { -0.1f,-0.3f,0.0f,1.0f };
	
	data1[1] = { -0.4f,-0.7f,0.0f,1.0f };
	data2[1] = { -0.35f,-0.5f,0.0f,1.0f };
	data3[1] = { -0.3f,-0.7f,0.0f,1.0f };
	
	//data1[3] = { 0.2f,0.7f,0.0f,1.0f };
	//data2[3] = { 0.15f,0.5f,0.0f,1.0f };
	//data3[3] = { 0.1f,0.7f,0.0f,1.0f };
	
	//data1[4] = { 0.6f, 0.9f,0.0f,1.0f };
	//data2[4] = { 0.55f, 0.7f,0.0f,1.0f };
	//data3[4] = { 0.5f, 0.9f,0.0f,1.0f };
	
	//data1[5] = { -0.1f,0.7f,0.0f,1.0f };
	//data2[5] = { -0.05f,0.9f,0.0f,1.0f };
	//data3[5] = { -0.0f,0.7f,0.0f,1.0f };
	
	//data1[6] = { 0.4f,-0.5f,0.0f,1.0f };
	//data2[6] = { 0.35f,-0.7f,0.0f,1.0f };
	//data3[6] = { 0.3f,-0.5f,0.0f,1.0f };
	
	//data1[7] = { 0.2f,-0.3f,0.0f,1.0f };
	//data2[7] = { 0.15f,-0.5f,0.0f,1.0f };
	//data3[7] = { 0.1f,-0.3f,0.0f,1.0f };
	
	data1[2] = { -0.6f,0.1f,0.0f,1.0f };
	data2[2] = { -0.55f,0.3f,0.0f,1.0f };
	data3[2] = { -0.5f,0.1f,0.0f,1.0f };
	
	//data1[9] = { 0.75f,-0.5f,0.0f,1.0f };
	//data2[9] = { 0.75f,-0.7f,0.0f,1.0f };
	//data3[9] = { 0.6f,-0.5f,0.0f,1.0f };

	for (int i = 0; i < 3; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(direct_,data1[i],data2[i],data3[i]);
	}
}

void YTEngine::Initialize(WinApp* win, int32_t width, int32_t height) {
	direct_->Initialize(win, win->kClientWidth, win->kClientHeight);

	InitializeDxcCompiler();


	CreateRootSignature();
	CreateInputlayOut();


	SettingBlendState();

	SettingRasterizerState();

	InitializePSO();

	SettingViewPort();

	SettingScissor();
}


void YTEngine::BeginFrame() {
	direct_->PreDraw();
	direct_->GetCommandList()->RSSetViewports(1, &viewPort_);
	direct_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);
	
	direct_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	direct_->GetCommandList()->SetPipelineState(graphicsPipelineState_);
}

void YTEngine::EndFrame() {
	direct_->PostDraw();
}

void YTEngine::Finalize() {
	for (int i = 0; i < 3; i++) {
		triangle[i]->Finalize();
	}
	graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_) {
		errorBlob_->Release();
	}
	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
	direct_->Finalize();
}

void YTEngine::Update() {

}

void YTEngine::Draw() {
	for (int i = 0; i < 3; i++) {
		triangle[i]->Draw();
	}
}

WinApp* YTEngine::win_;
DirectXCommon* YTEngine::direct_;