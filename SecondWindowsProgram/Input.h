#pragma once

#include <dInput.h>
#include <Xinput.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib, "Xinput.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}


namespace Input
{
	enum struct MOUSE
	{
		LEFT   = 0,
		RIGHT,
		MIDDLE,
	};

	enum struct PAD
	{
		UP      = XINPUT_GAMEPAD_DPAD_UP,
		DOWN    = XINPUT_GAMEPAD_DPAD_DOWN,
		LEFT    = XINPUT_GAMEPAD_DPAD_LEFT,
		RIGHT   = XINPUT_GAMEPAD_DPAD_RIGHT,
		START   = XINPUT_GAMEPAD_START,
		BACK    = XINPUT_GAMEPAD_BACK,
		LS      = XINPUT_GAMEPAD_LEFT_THUMB,
		RS      = XINPUT_GAMEPAD_RIGHT_THUMB,
		LB      = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RB      = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		A       = XINPUT_GAMEPAD_A,
		B       = XINPUT_GAMEPAD_B,
		X       = XINPUT_GAMEPAD_X,
		Y       = XINPUT_GAMEPAD_Y,

		// Aliases
		SELECT  = BACK,
		LSTICK  = LS,
		RSTICK  = RS,
		L       = LB,
		R       = RB,
	};

	using DirectX::XMFLOAT2;

	HRESULT Initialize(const HWND& _hWnd);
	void Update();

	bool IsKey    (int _keyCode);
	bool IsKeyUp  (int _keyCode);
	bool IsKeyDown(int _keyCode);

	XMFLOAT2 GetMousePosition();
	void     SetMousePosition(const XMFLOAT2& _position);

	bool IsMouse    (int _mouseButtonCode);
	bool IsMouseUp  (int _mouseButtonCode);
	bool IsMouseDown(int _mouseButtonCode);

	XMFLOAT2 GetStickValue(const wchar_t _stick);
	float    GetTriggerValue(const wchar_t _trigger);
	
	// x-->l, y-->r
	void SetPadVibration(XMFLOAT2 _vibStrength = XMFLOAT2(0, 0));

	bool IsPad(PAD _padButtonCode);
	bool IsPadUp(PAD _padButtonCode);
	bool IsPadDown(PAD _padButtonCode);

	void Release();
};