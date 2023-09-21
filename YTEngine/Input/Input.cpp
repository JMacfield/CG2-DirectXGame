#include "Input.h"

void Input::Initialize(WinApp* winApp) {
	// DirectInput オブジェクトを作成
	HRESULT hr = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスを生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	// 入力データの形式をリセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	keys = {};
	preKeys = {};
	count_ = 0;
}

void Input::Update() {
	preKeys = keys;

	// キーボード情報の取得開始
	keyboard->Acquire();
	keys = {};

	// 全てのキーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(keys), &keys);
}

Input* Input::GetInstance() {
	if (!input_) {
		input_ = new Input();
	}

	return input_;
}

bool Input::PushKey(uint8_t keyNumber) const {
	if (!keys[keyNumber] && preKeys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::PressKey(uint8_t keyNumber) const {
	if (keys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::IsReleaseKey(uint8_t keyNumber) const {
	if (keys[keyNumber] && !preKeys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}

Input* Input::input_ = nullptr;