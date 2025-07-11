#pragma once

#include <d3d11.h>
#include <cassert>

#include <type_traits>
#include <utility>

#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(ptr)   \
		if (ptr != nullptr) \
		{                   \
			ptr->Release(); \
            ptr = nullptr;  \
		}                   \

template <typename PTYPE>
void SafeDelete(PTYPE * _ptr)
{
	if (_ptr != nullptr)
	{
		delete _ptr;
		_ptr = nullptr;
	}
}


struct VERTEX
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR uv;
};

struct QuadFace
{
	VERTEX topLeft;
	VERTEX topRight;
	VERTEX bottomRight;
	VERTEX bottomLeft;
};

namespace Direct3D
{
	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	extern ID3D11Device* pDevice;	    	//デバイス
	extern ID3D11DeviceContext* pContext;	    	//デバイスコンテキスト
	extern IDXGISwapChain* pSwapChain;		    //スワップチェイン
	extern ID3D11RenderTargetView* pRenderTargetView;	//レンダーターゲットビュー
};