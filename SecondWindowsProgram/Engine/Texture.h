#pragma once
//#include <d3d11.h>
#include <string>
#include "Direct3D.h"
#include "DirectXTex.h"

class Texture
{
public:
	Texture();
	~Texture();
	HRESULT Load(std::string _fileName);
	void Release();
    DirectX::XMINT2 GetSize();

	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
	DirectX::XMFLOAT4 GetTextureAspectRatio();
private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	DirectX::TexMetadata metadata_;
};