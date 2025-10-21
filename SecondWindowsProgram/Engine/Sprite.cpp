#include "Sprite.h"
#include <cassert>
#include "Camera.h"
#include "SafeCleaning.h"

using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;

Sprite::Sprite(std::string _filePath, Transform _transform) :
	transform_(_transform),
	pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr),
	imageFilePath_(_filePath)
{
	pTexture_ = new Texture();

	XMFLOAT3 position = transform_.position;
	XMFLOAT3 size = transform_.scale;

	spriteVertices_ =
	{
		{XMVectorSet(position.x + -size.x, position.y +  size.y, position.z + -size.z, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)}, // 左上
		{XMVectorSet(position.x +  size.x, position.y +  size.y, position.z + -size.z, 1.0f), XMVectorSet(1.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)}, // 右上
		{XMVectorSet(position.x +  size.x, position.y + -size.y, position.z + -size.z, 1.0f), XMVectorSet(1.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)}, // 右下
		{XMVectorSet(position.x + -size.x, position.y + -size.y, position.z + -size.z, 1.0f), XMVectorSet(0.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)}, // 左下
	};

	vertices_.clear();
	vertices_.push_back(spriteVertices_.topLeft);
	vertices_.push_back(spriteVertices_.topRight);
	vertices_.push_back(spriteVertices_.bottomRight);
	vertices_.push_back(spriteVertices_.bottomLeft);
}

Sprite::~Sprite()
{
}

HRESULT Sprite::Initialize()
{
	HRESULT hResult;

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = static_cast<UINT>( sizeof(VERTEX) * vertices_.size() );
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();

	hResult = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点がなんか変です", L"鰓ー", MB_OK);
		return hResult;
	}

	int index[] = { 0, 2, 3, 0, 1, 2 };

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hResult = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"インデックスバッファがなんか変です", L"鰓ー", MB_OK);
		return hResult;
	}

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);

	pTexture_ = new Texture();
	pTexture_->Load(imageFilePath_);

	return hResult;
}

void Sprite::Draw(Transform& _transform)
{
	Direct3D::SetShader(SHADER_2D);
	XMMATRIX worldMat = _transform.GetWorldMatrix();

	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb{};
	cb.matWVP = XMMatrixTranspose(worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matW = worldMat;
	cb.matNormal = XMMatrixInverse(nullptr, cb.matW);

	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)( &cb ), sizeof(cb));	// データの値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	//stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(6, 0, 0);
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
