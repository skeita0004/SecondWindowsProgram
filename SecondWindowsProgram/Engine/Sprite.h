#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

    HRESULT Load(const std::string& _fileName);
	void Draw(Transform& _transform, RECT _rect, float _alpha);
	void Release();

    XMINT2 GetSize();

private:
    struct Vertex2D
    {
        XMFLOAT2 position;
        XMFLOAT2 uv;
    };

    struct ConstantBuffer2D
    {
        XMMATRIX matW;
        XMMATRIX matUvTransform;
        XMFLOAT4 color;
    };

    HRESULT InitVertex();
    HRESULT InitIndex();
    HRESULT InitConstantBuffer();

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   // インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;
	std::vector<Vertex2D> vertices_;
};
