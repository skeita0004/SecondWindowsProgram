#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>

using namespace DirectX;

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

class Quad
{
public:
	Quad();
	Quad(XMFLOAT4 _pos, float _size, std::vector<VERTEX> _vertices);
	~Quad();
	HRESULT Initialize();
	void Draw(const XMMATRIX& _worldMatrix);
	void Release();

private:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;

	float size_;
	XMFLOAT4 position_;
	std::vector<VERTEX> vertices_;
};
