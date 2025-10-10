#include "Input.h"
#include <DirectXMath.h>
#include <vector>
#include <string>

//#define DIK_TRUE 0x80
namespace
{
    using namespace DirectX;

    const DWORD MAX_KEY  { 256 };  // Max keyboard key num
    const BYTE  DIK_TRUE { 0x80 }; // DirectInput flag mask
    const int   PAD_ID   { 0 };
    BYTE currKeyState[MAX_KEY]{}; // ┳━KeyStates
    BYTE prevKeyState[MAX_KEY]{}; // ┛

    XMFLOAT2     mousePosition{};
    DIMOUSESTATE currMouseState{};
    DIMOUSESTATE prevMouseState{};

    LPDIRECTINPUT8       pDInput     { nullptr }; // Device
    LPDIRECTINPUTDEVICE8 pKeyDevice  { nullptr }; // Keyboard
    LPDIRECTINPUTDEVICE8 pMouseDevice{ nullptr }; // Mouse

    XINPUT_STATE currPadState{};
    XINPUT_STATE prevPadState{};

    struct PadAnalog
    {
        XMFLOAT2 lStick;
        XMFLOAT2 rStick;
        float    lTrigger;
        float    rTrigger;
    };
    
    PadAnalog padAnalogVal{};

    void SetPadAnalogVal(PadAnalog* _dist, const XINPUT_GAMEPAD& _analogVal)
    {
        XMFLOAT2& lStick = _dist->lStick;
        XMFLOAT2& rStick = _dist->rStick;

        const SHORT sShortMax = (USHRT_MAX / 2);
        const BYTE  ByteMax   = UCHAR_MAX;

        const float triggerDeadZone = (int)XINPUT_GAMEPAD_TRIGGER_THRESHOLD / ByteMax;
        const float lStickDeadZone = fabsf(
            (int)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / sShortMax);

        const float rStickDeadZone = fabsf(
            (int)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / sShortMax);

        XMVECTOR vLStick{};
        XMVECTOR vRStick{};

        vLStick = XMLoadFloat2(&lStick);
        vRStick = XMLoadFloat2(&rStick);

        vLStick = XMVectorSet(_analogVal.sThumbLX / sShortMax,
                              _analogVal.sThumbLY / sShortMax,
                              0.f, 0.f);

        vRStick = XMVectorSet(_analogVal.sThumbRX / sShortMax,
                              _analogVal.sThumbRY / sShortMax,
                              0.f, 0.f);

        _dist->lTrigger = _analogVal.bLeftTrigger  / ByteMax;
        _dist->rTrigger = _analogVal.bRightTrigger / ByteMax;
        
        if (_dist->lTrigger <= triggerDeadZone)
        {
            _dist->lTrigger = 0.0f;
        }
        else if (_dist->rTrigger <= triggerDeadZone)
        {
            _dist->rTrigger = 0.0f;
        }

        XMStoreFloat2(&lStick, vLStick);
        XMStoreFloat2(&rStick, vRStick);

        // 冗長
        if (fabsf(lStick.x) <= lStickDeadZone)
        {
            lStick.x = 0.0f;
        }
        if (fabsf(lStick.y) <= lStickDeadZone)
        {
            lStick.y = 0.0f;
        }

        if (fabsf(rStick.x) <= rStickDeadZone)
        {
            rStick.x = 0.0f;
        }
        if (fabsf(rStick.y) <= rStickDeadZone)
        {
            rStick.y = 0.0f;
        }
    }

}

namespace Input
{
    using namespace DirectX; // ブロック内にする意味はないと思うけど、一応

    HRESULT Input::Initialize(const HWND& _hWnd)
    {
        const HWND hWnd = _hWnd;
        HRESULT result{};

        DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
        
        pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice,   nullptr);
        pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
        pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE bitor DISCL_FOREGROUND);

        pDInput->CreateDevice(GUID_SysMouse,    &pMouseDevice, nullptr);
        pMouseDevice->SetDataFormat(&c_dfDIMouse);
        pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE bitor DISCL_FOREGROUND);

        return S_OK;
    }

    void Input::Update()
    {
        memcpy(&prevKeyState, &currKeyState, (sizeof(BYTE) * MAX_KEY));
        pKeyDevice->Acquire();
        pKeyDevice->GetDeviceState(sizeof(currKeyState), &currKeyState);

        memcpy(&prevMouseState, &currMouseState, sizeof(DIMOUSESTATE));
        pMouseDevice->Acquire();
        pMouseDevice->GetDeviceState(sizeof(currMouseState), &currMouseState);

        memcpy(&prevPadState, &currPadState, sizeof(XINPUT_STATE));
        XInputGetState(PAD_ID, &currPadState);
        SetPadAnalogVal(&padAnalogVal, currPadState.Gamepad);

        //// mouse pos calculation
        //XMVECTOR mousePosVector{ XMLoadFloat2(&mousePosition)};
        //XMVECTOR mousePosDiffVector{ XMVectorSet(currMouseState.lX, currMouseState.lY, 0.f, 0.f) };
        //XMVectorAdd(mousePosVector, mousePosDiffVector);
        //XMStoreFloat2(&mousePosition, mousePosVector);
    }

#pragma region Key_Board

    bool IsKey(int _keyCode)
    {
        const BYTE currKey = currKeyState[_keyCode];

        if (currKey bitand DIK_TRUE)
        {
            return true;
        }
        return false;
    }

    bool IsKeyUp(int _keyCode)
    {
        const BYTE currKey = currKeyState[_keyCode];
        const BYTE prevKey = prevKeyState[_keyCode];

        if (( ( currKey xor prevKey ) bitand prevKey ) bitand DIK_TRUE)
        {
            return true;
        }

        return false;
    }

    bool IsKeyDown(int _keyCode)
    {
        const BYTE currKey = currKeyState[_keyCode];
        const BYTE prevKey = prevKeyState[_keyCode];

        if (( ( currKey xor prevKey ) bitand currKey ) bitand DIK_TRUE)
        {
            return true;
        }

        return false;
    }
#pragma endregion

#pragma region Mouse

    XMFLOAT2 GetMousePosition()
    {
        return mousePosition;
    }

    void SetMousePosition(const XMFLOAT2& _position)
    {
        mousePosition = _position;
    }

    bool IsMouse(int _mouseButtonCode)
    {
        const BYTE currMouseButton = currMouseState.rgbButtons[_mouseButtonCode];

        if (currMouseButton bitand DIK_TRUE)
        {
            return true;
        }
        return false;
    }

    bool IsMouseUp(int _mouseButtonCode)
    {
        const BYTE currMouseButton = currMouseState.rgbButtons[_mouseButtonCode];
        const BYTE prevMouseButton = prevMouseState.rgbButtons[_mouseButtonCode];

        if (( ( currMouseButton xor prevMouseButton ) bitand prevMouseButton ) bitand DIK_TRUE)
        {
            return true;
        }

        return false;
    }

    bool IsMouseDown(int _mouseButtonCode)
    {
        const BYTE currMouseButton = currMouseState.rgbButtons[_mouseButtonCode];
        const BYTE prevMouseButton = prevMouseState.rgbButtons[_mouseButtonCode];

        if (( ( currMouseButton xor prevMouseButton ) bitand currMouseButton ) bitand DIK_TRUE)
        {
            return true;
        }

        return false;
    }



#pragma endregion

#pragma region Pad

    XMFLOAT2 GetStickValue(const wchar_t _stick)
    {
        switch (_stick)
        {
        case 'l':
            return padAnalogVal.lStick;

        case 'r':
            return padAnalogVal.rStick;

        default:
            break;
        }

        return XMFLOAT2(0, 0);
    }

    float GetTriggerValue(const wchar_t _trigger)
    {
        switch (_trigger)
        {
        case 'l':
            return padAnalogVal.lTrigger;

        case 'r':
            return padAnalogVal.rTrigger;

        default:
            break;
        }

        return 0.f;
    }

    void SetPadVibration(XMFLOAT2 _vibStrength)
    {
        XINPUT_VIBRATION vibration;

        vibration.wLeftMotorSpeed  = USHRT_MAX * _vibStrength.x;
        vibration.wRightMotorSpeed = USHRT_MAX * _vibStrength.y;

        XInputSetState(PAD_ID, &vibration);
    }

    bool IsPad(PAD _padButtonCode)
    {
        int currPadButton = currPadState.Gamepad.wButtons bitand (int)_padButtonCode;

        return currPadButton;
    }

    bool IsPadUp(PAD _padButtonCode)
    {
        int currPadButton = currPadState.Gamepad.wButtons bitand (int)_padButtonCode;
        int prevPadButton = prevPadState.Gamepad.wButtons bitand (int)_padButtonCode;

        if ((currPadButton xor prevPadButton) bitand prevPadButton)
        {
            return true;
        }

        return false;
    }

    bool IsPadDown(PAD _padButtonCode)
    {
        int currPadButton = currPadState.Gamepad.wButtons bitand (int)_padButtonCode;
        int prevPadButton = prevPadState.Gamepad.wButtons bitand (int)_padButtonCode;

        if ((currPadButton xor prevPadButton) bitand currPadButton)
        {
            return true;
        }

        return false;
    }

#pragma endregion

    void Input::Release()
    {
        pMouseDevice->Unacquire();
        SAFE_RELEASE(pMouseDevice);

        pKeyDevice->Unacquire();
        SAFE_RELEASE(pKeyDevice);

        SAFE_RELEASE(pDInput);
    }
}