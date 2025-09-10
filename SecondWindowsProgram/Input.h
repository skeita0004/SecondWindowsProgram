#pragma once

#include <dInput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	HRESULT Initialize(const HWND& _hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyUp(int keyCode);
	bool IsKeyDown(int keyCode);
	void Release();
};