#include "Texture.h"
#include <wincodec.h>
#include <DirectXTex.h>

// DirectXTexのライブラリをリンク
#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;
using std::string;

Texture::Texture() :
	pSampler_(nullptr),
	pSRV_(nullptr),
	metadata_()
{
}

Texture::~Texture()
{
}

// 引数でwstringは、呼ぶときに支障をきたす！
HRESULT Texture::Load(string _fileName)
{
	ScratchImage image;   //画像本体

	HRESULT hr;

	//実際に読んでゆくぅ
	std::wstring fileName{ _fileName.begin(), _fileName.end() };
	
	hr = LoadFromWICFile(fileName.c_str(), WIC_FLAGS::WIC_FLAGS_NONE,
		&metadata_, image);

	if (FAILED(hr))
	{
		return S_FALSE;
	}

	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // 補間なし
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; // 繰り返すかどうか、この場合、端の色が引き延ばされる
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);

	//シェーダーリソースビュー

	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;

	hr = CreateShaderResourceView(Direct3D::pDevice,
		image.GetImages(), image.GetImageCount(), metadata_, &pSRV_);
}

void Texture::Release()
{
    SafeCleaning::SafeRelease(pSampler_);
    SafeCleaning::SafeRelease(pSRV_);
}

XMFLOAT4 Texture::GetTextureAspectRatio()
{
	XMFLOAT4 aspectRatio{};
	aspectRatio.x = metadata_.width;
	aspectRatio.y = metadata_.height;

	if (aspectRatio.x < aspectRatio.y)
	{
		aspectRatio.y /= aspectRatio.x; // 共通の分母だから、分母を先に変更するとおかしくなっちゃうよ
		aspectRatio.x /= aspectRatio.x;
	}
	else if (aspectRatio.y < aspectRatio.x)
	{
		aspectRatio.x /= aspectRatio.y;
		aspectRatio.y /= aspectRatio.y;
	}
	return aspectRatio;
}
