#include "Input.h"

namespace
{
    LPDIRECTINPUT8   pDInput = nullptr;
    LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
    BYTE keyState[256] = { 0 };
}

HRESULT Input::Initialize(const HWND& _hWnd)
{

    const HWND hWnd = _hWnd; // 変更したら（できないようにしたけど）どうなる？

    if (FAILED(
        DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr) ||
        pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr) ||
        pKeyDevice->SetDataFormat(&c_dfDIKeyboard) ||
        pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
    {
        return E_FAIL;
    }
 
}

void Input::Update()
{
    pKeyDevice->Acquire();
    pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
}

bool Input::IsKey(int keyCode)
{
    return false;
}

void Input::Release()
{
    SAFE_RELEASE(pDInput);
}
