#include "Input.h"

//#define DIK_TRUE 0x80

namespace Input
{

    LPDIRECTINPUT8   pDInput = nullptr;
    LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
    const DWORD MAX_KEY{ 256 };
    const BYTE DIK_TRUE{ 0x80 };
    BYTE keyState[MAX_KEY] = { 0 };
    BYTE prevKeyState[MAX_KEY] = { 0 };

    HRESULT Input::Initialize(const HWND& _hWnd)
    {
        const HWND hWnd = _hWnd; // 変更したら（できないようにしたけど）どうなる？

        DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
        pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
        pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
        pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

        //    )
        //if (FAILED(
        //{
        //    return E_FAIL;
        //}
        return S_OK;
    }

    void Input::Update()
    {
        memcpy(&prevKeyState, &keyState, (sizeof(BYTE) * MAX_KEY));
        pKeyDevice->Acquire();
        pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
    }

    bool Input::IsKey(int keyCode)
    {
        if (keyState[keyCode] & DIK_TRUE)
        {
            return true;
        }
        return false;
    }

    bool IsKeyUp(int keyCode)
    {
        int key = keyState[keyCode];
        int prevKey = prevKeyState[keyCode];

        if(((key xor prevKey) and_eq prevKey) & DIK_TRUE);

        return false;

        // 0001
        // 0000
        // 0001
        // 0001
        // 0001
        //
    }

    bool IsKeyDown(int keyCode)
    {
        return false;
    }

    void Input::Release()
    {
        SAFE_RELEASE(pKeyDevice);
        SAFE_RELEASE(pDInput);
    }

}