#include "Direct3D.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Camera.h"
#include "SafeCleaning.h"

using namespace DirectX;

namespace Direct3D
{	// MSの命令で初期化を行うので、何もしなくても大丈夫（あとでチェックする）
	ID3D11Device* pDevice;                     //デバイス
	ID3D11DeviceContext* pContext;             //デバイスコンテキスト
	IDXGISwapChain* pSwapChain;                //スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView; //レンダーターゲットビュー
	ID3D11Texture2D* pDepthStencil;            //深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView; //深度ステンシルビュー

	struct SHADER_BANDLE
	{
		ID3D11VertexShader* pVertexShader;	//頂点シェーダー
		ID3D11PixelShader* pPixelShader;		//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout;	//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState;	//ラスタライザー
	};

	ID3D11VertexShader* pVertexShader = nullptr;	//頂点シェーダー
	ID3D11PixelShader* pPixelShader = nullptr;		//ピクセルシェーダー
	ID3D11InputLayout* pVertexLayout = nullptr;	//頂点インプットレイアウト
	ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー

	SHADER_BANDLE shaderBundle[SHADER_MAX];

}

HRESULT Direct3D::Initialize(const int winW, const int winH, HWND hWnd)
{
	///////////////////////////いろいろ準備するための設定///////////////////////////////
//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc{};

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	if (FAILED(D3D11CreateDeviceAndSwapChain( // デバイスとスワップチェーンを一度に作成
		nullptr,					// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,					// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		D3D11_CREATE_DEVICE_DEBUG,							// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,					// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,							// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
		&scDesc,					// 上でいろいろ設定した構造体
		&pSwapChain,				// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,					// 無事完成したDeviceアドレスが返ってくる
		&level,						// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext)))// 無事完成したContextのアドレスが返ってくる
	{
		MessageBox(nullptr, L"デバイスとかの設定に失敗しました", L"鰓ー", MB_OK);
	}

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer{};

	if (FAILED(
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		MessageBox(nullptr, L"バックバッファの取得は失敗に終わりました。", L"鰓ー", MB_OK);
		return -1;
	}

	if (FAILED(
		//レンダーターゲットビューを作成
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView)))
	{
		MessageBox(nullptr, L"レンダーターゲットビューを作成できなかった", L"鰓ー", MB_OK);

		return -1;
	}

	//一時的にバックバッファを取得しただけなので解放
	pBackBuffer->Release();

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上

	//深度ステンシルビューの作成
	//pDepthStencil = nullptr;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winW;
	descDepth.Height = winH;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定(ポリゴンのトポロジ)
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);             // 描画先を設定(レンダリング先は複数指定できたりする)
	pContext->RSSetViewports(1, &vp);                                         // ビューポートを設定

	//シェーダー準備
	HRESULT hResult;
	hResult = InitShader3D();
	if (FAILED(hResult))
	{
		return hResult;
	}

	return hResult;
}

void Direct3D::BeginDraw()
{
	//背景の色
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//R,G,B,A

	//画面をクリア
	Direct3D::pContext->ClearRenderTargetView(Direct3D::pRenderTargetView, clearColor);

	Camera::Update();

	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	Direct3D::pSwapChain->Present(0, 0);
}

void Direct3D::Release()
{
    SafeCleaning::SafeRelease(pRasterizerState);
    SafeCleaning::SafeRelease(pVertexLayout);
    SafeCleaning::SafeRelease(pPixelShader);
    SafeCleaning::SafeRelease(pVertexShader);

    SafeCleaning::SafeRelease(pRenderTargetView);
    SafeCleaning::SafeRelease(pSwapChain);
    SafeCleaning::SafeRelease(pContext);
    SafeCleaning::SafeRelease(pDevice);
}

HRESULT Direct3D::InitShader()
{
	if (FAILED(InitShader3D()))
	{
		return E_FAIL;
	}

	if (FAILED(InitShader2D()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//シェーダー準備
HRESULT Direct3D::InitShader3D()
{
    DWORD vectorSize = sizeof(XMFLOAT3);

	HRESULT hResult;
	auto& shader = shaderBundle[SHADER_3D];

	WCHAR cwd[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, cwd);
	OutputDebugString(cwd);

	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	hResult = D3DCompileFromFile(L"Engine/Shader/Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	
	if FAILED(hResult)
	{
		return hResult;
	}

	hResult = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(),
										  pCompileVS->GetBufferSize(),
										  NULL,
										  &shader.pVertexShader);
	
	if FAILED(hResult)
	{
		return hResult;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, vectorSize, D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0}, // 法線
	};

	hResult = pDevice->CreateInputLayout(layout,
										 sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
										 pCompileVS->GetBufferPointer(),
										 pCompileVS->GetBufferSize(),
										 &shader.pVertexLayout );
	if FAILED(hResult)
	{
		return hResult;
	}

	SafeCleaning::SafeRelease(pCompileVS);
	
	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);

	hResult = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(),
										 pCompilePS->GetBufferSize(),
										 NULL,
										 &shader.pPixelShader);
	if FAILED(hResult)
	{
		return hResult;
	}

	SafeCleaning::SafeRelease(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE; // 逆時計回りが表？なるほどね。

	hResult = pDevice->CreateRasterizerState(&rdc, &shader.pRasterizerState );
	if FAILED(hResult)
	{
		return hResult;
	}
	
	SetShader(SHADER_3D);
	return hResult;
}

HRESULT Direct3D::InitShader2D()
{
    DWORD vectorSize = sizeof(XMFLOAT3);

	HRESULT hResult;
	auto& shader = shaderBundle[SHADER_2D];
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hResult = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(),
										  pCompileVS->GetBufferSize(), 
										  NULL, 
										  &shader.pVertexShader);

	if FAILED(hResult)
	{
		return hResult;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, vectorSize, D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
	};

	hResult = pDevice->CreateInputLayout(layout,
										 sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
										 pCompileVS->GetBufferPointer(),
										 pCompileVS->GetBufferSize(),
										 &shader.pVertexLayout);
	
	if FAILED(hResult)
	{
		return hResult;
	}

	SafeCleaning::SafeRelease(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);

	hResult = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(),
										 pCompilePS->GetBufferSize(),
										 NULL,
										 &shader.pPixelShader);
	
	if FAILED(hResult)
	{
		return hResult;
	}

	SafeCleaning::SafeRelease(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hResult = pDevice->CreateRasterizerState(&rdc, &shader.pRasterizerState);

	if FAILED(hResult)
	{
		return hResult;
	}

	SetShader(SHADER_2D);
	return hResult;
}

void Direct3D::SetShader(SHADER_TYPE _type)
{
	const SHADER_BANDLE& shader = shaderBundle[_type]; // 参照変数はこういう使い方ができる。

	pContext->VSSetShader(shader.pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shader.pPixelShader, NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(shader.pVertexLayout);	//頂点インプットレイアウト
	pContext->RSSetState(shader.pRasterizerState);		//ラスタライザー
}
