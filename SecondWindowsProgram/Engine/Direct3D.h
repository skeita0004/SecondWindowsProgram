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
inline void SafeDelete(PTYPE * _ptr)
{
	if (_ptr != nullptr)
	{
		delete _ptr;
		_ptr = nullptr;
	}
}

enum SHADER_TYPE
{
	SHADER_3D,
	SHADER_2D,
	SHADER_MAX
};

// ここにあるのはけしからん
/// @brief ポリゴンの一頂点を定義する構造体
struct VERTEX
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR uv;
	DirectX::XMVECTOR normal;
};

// ここにあるのはけしからん
/// @brief 四角形の頂点を定義する構造体
struct QuadFace
{
	VERTEX topLeft;
	VERTEX topRight;
	VERTEX bottomRight;
	VERTEX bottomLeft;
};

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	DirectX::XMMATRIX matWVP;
	DirectX::XMMATRIX matNormal;
	DirectX::XMMATRIX matW;
};

namespace Direct3D
{
	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();

	/// @brief 指定されたタイプのシェーダを作成する
	/// @param _type シェーダのタイプ
	void SetShader(SHADER_TYPE _type);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	extern ID3D11Device* pDevice;                     //デバイス
	extern ID3D11DeviceContext* pContext;             //デバイスコンテキスト
	extern IDXGISwapChain* pSwapChain;                //スワップチェイン
	extern ID3D11RenderTargetView* pRenderTargetView; //レンダーターゲットビュー
	extern ID3D11Texture2D* pDepthStencil;			//深度ステンシル
	extern ID3D11DepthStencilView* pDepthStencilView;		//深度ステンシルビュー

};