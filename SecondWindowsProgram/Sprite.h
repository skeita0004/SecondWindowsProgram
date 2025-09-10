#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

class Sprite
{
public:
	Sprite(std::string _filePath, Transform _transform);
	~Sprite();
	HRESULT Initialize();
	void Draw(Transform& _transform);
	void Release();

	void SetTransform(Transform _transform)
	{
		transform_ = _transform;
	}

	Transform GetTransform()
	{
		return transform_;
	}
private:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;

	std::vector<VERTEX> vertices_;
	QuadFace spriteVertices_;
	std::string imageFilePath_;

	Transform transform_;
};
