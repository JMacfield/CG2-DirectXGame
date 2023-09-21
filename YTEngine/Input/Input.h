#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInput バージョンを指定

#include <dinput.h>
#include <array>
#include <wrl.h>

#include "WinApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input {
public:
	void Initialize(WinApp* winApp);
	void Update();

	static Input* GetInstance();

	// 押した時
	bool PushKey(uint8_t keyNumber) const;
	// 押している間
	bool PressKey(uint8_t keyNumber) const;
	// 離した時
	bool IsReleaseKey(uint8_t keyNumber) const;

private:
	static Input* input_;

	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;

	std::array<BYTE, 256> keys;
	std::array<BYTE, 256> preKeys;

	int count_;
};