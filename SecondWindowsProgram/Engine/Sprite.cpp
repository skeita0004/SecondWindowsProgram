#include "Sprite.h"
#include <cassert>
#include "Camera.h"
#include "SafeCleaning.h"
#include "Screen.h"

using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;

Sprite::Sprite() :
	pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
}

Sprite::~Sprite()
{
}

HRESULT Sprite::Load(const std::string& _fileName)
{
    HRESULT hResult{};

	pTexture_ = new Texture();
	hResult = pTexture_->Load(_fileName);
    if (FAILED(hResult))
    {
        MessageBox(nullptr, L"Failed Load(Texture)", L"ERROR!", MB_OK);
        return hResult;
    }

    hResult = InitVertex();
    hResult = InitIndex();
    hResult = InitConstantBuffer();

    return S_OK;
}

void Sprite::Draw(Transform& _transform, RECT _rect, float _alpha)
{
	Direct3D::SetShader(SHADER_2D);
    Direct3D::SetZBufferEnable(false);

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

    // コンスタントバッファ
    XMMATRIX matTrim{ XMMatrixScaling(static_cast<float>(_rect.right),
                                   static_cast<float>(_rect.bottom),
                                   1.f) };

    XMMATRIX matView{ XMMatrixScaling(1.0f / Screen::WIDTH,
                                   1.0f / Screen::HEIGHT,
                                   1.0f) };
    
    XMMATRIX matWorld   {_transform.GetWorldMatrix() * matTrim * matView};

    XMFLOAT2 textureSize
    {
        static_cast<float>(pTexture_->GetSize().x),
        static_cast<float>(pTexture_->GetSize().y)
    };

    XMMATRIX matTexTranslate = XMMatrixTranslation(
        static_cast<float>(_rect.left) / textureSize.x,
        static_cast<float>(_rect.top)  / textureSize.y,
        0.0f);

    XMMATRIX matTexScale = XMMatrixScaling(
        static_cast<float>(_rect.right) / textureSize.x,
        static_cast<float>(_rect.bottom) / textureSize.y,
        1.0f);

    XMMATRIX matTexel = matTexScale * matTexTranslate;

    D3D11_MAPPED_SUBRESOURCE dataSR{};
    ConstantBuffer2D cb
    {
        .matW          { XMMatrixTranspose(matWorld) },
        .matUvTransform{ XMMatrixTranspose(matTexel) },
        .color         { XMFLOAT4(1, 1, 1, _alpha) }
    };
	// インデックスバッファーをセット
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataSR);	// GPUからのデータアクセスを止める
	memcpy_s(dataSR.pData, dataSR.RowPitch, (void*)( &cb ), sizeof(cb));	// データの値を送る

    // テクスチャサンプラ
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	Direct3D::pContext->DrawIndexed(6, 0, 0);

    Direct3D::SetZBufferEnable(true);
	Direct3D::SetShader(SHADER_3D);
}

void Sprite::Release()
{
	SafeCleaning::SafeRelease(pTexture_);
	SafeCleaning::SafeDelete(pTexture_);

	SafeCleaning::SafeRelease(pConstantBuffer_);
	SafeCleaning::SafeRelease(pIndexBuffer_);
	SafeCleaning::SafeRelease(pVertexBuffer_);
}

XMINT2 Sprite::GetSize()
{
    return pTexture_->GetSize();
}

HRESULT Sprite::InitVertex()
{
    HRESULT result{};

    const Vertex2D vertices[]
    {
        { XMFLOAT2(-1.f,  1.f), XMFLOAT2(0.f, 0.f) }, // 左上
        { XMFLOAT2( 1.f,  1.f), XMFLOAT2(1.f, 0.f) }, // 右上
        { XMFLOAT2(-1.f, -1.f), XMFLOAT2(0.f, 1.f) }, // 左下
        { XMFLOAT2( 1.f, -1.f), XMFLOAT2(1.f, 1.f) }  // 右下
    };

    D3D11_BUFFER_DESC vertexBD
    {
        .ByteWidth          {sizeof(vertices)         },
        .Usage              {D3D11_USAGE_DEFAULT      },
        .BindFlags          {D3D11_BIND_VERTEX_BUFFER },
        .CPUAccessFlags     {0},
        .MiscFlags          {0},
        .StructureByteStride{0},
    };

    D3D11_SUBRESOURCE_DATA vertexData
    {
        .pSysMem         {vertices},
        .SysMemPitch     {0},
        .SysMemSlicePitch{0}
    };

    result = Direct3D::pDevice->CreateBuffer(&vertexBD, &vertexData, &pVertexBuffer_);
    if (FAILED(result))
    {
        MessageBox(nullptr, L"Failed : CreateBuffer(vertex)", L"ERROR!", MB_OK);
        return result;
    }
    return result;
}

HRESULT Sprite::InitIndex()
{
    HRESULT result{};
    const int index[]{ 2, 1, 0,  2, 1, 3 };

    D3D11_BUFFER_DESC indexBD
    {
        .ByteWidth     {sizeof(index)},
        .Usage         {D3D11_USAGE_DEFAULT},
        .BindFlags     {D3D11_BIND_INDEX_BUFFER},
        .CPUAccessFlags{0},
        .MiscFlags     {0},
    };

    D3D11_SUBRESOURCE_DATA indexData
    {
        .pSysMem         {index},
        .SysMemPitch     {0},
        .SysMemSlicePitch{0}
    };

    result = Direct3D::pDevice->CreateBuffer(&indexBD, &indexData, &pIndexBuffer_);
    if (FAILED(result))
    {
        MessageBox(nullptr, L"Failed : CreateBuffer(index)", L"ERROR!", MB_OK);
        return result;
    }

    return result;
}

HRESULT Sprite::InitConstantBuffer()
{
    HRESULT result{};

    D3D11_BUFFER_DESC constantBD
    {
        .ByteWidth          {sizeof(ConstantBuffer2D)},
        .Usage              {D3D11_USAGE_DYNAMIC},
        .BindFlags          {D3D11_BIND_CONSTANT_BUFFER},
        .CPUAccessFlags     {D3D11_CPU_ACCESS_WRITE},
        .MiscFlags          {0},
        .StructureByteStride{0}
    };

    result = Direct3D::pDevice->CreateBuffer(&constantBD, nullptr, &pConstantBuffer_);
    if (FAILED(result))
    {
        MessageBox(nullptr, L"Failed : CreateBuffer(constant buffer)", L"ERROR!", MB_OK);
        return result;
    }

    return result;
}
