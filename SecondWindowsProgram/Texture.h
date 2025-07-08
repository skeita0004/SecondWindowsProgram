#pragma once
//#include <d3d11.h>
#include <string>
#include "Direct3D.h"

class Texture
{
public:
	Texture();
	~Texture();
	HRESULT Load(std::string _fileName);
	void Release();

	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
};