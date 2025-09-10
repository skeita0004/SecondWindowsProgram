#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

using namespace DirectX;

class Quad
{
public:
	//Quad();
	Quad(Transform _transform, QuadFace _vertices);
	~Quad();
	HRESULT Initialize();
	void Draw(Transform& _transform);
	void Release();

private:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;

	Transform transform_;

	float size_;
	XMFLOAT4 position_;
	std::vector<VERTEX> vertices_;
};
