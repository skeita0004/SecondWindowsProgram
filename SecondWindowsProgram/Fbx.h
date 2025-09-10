#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Direct3D.h"
#include "Texture.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Fbx
{
public:
	Fbx();
	HRESULT Load(std::string _fileName);
	void    Draw(Transform& _transform);
	void    Release();

private:
	struct CONSTANT_BUFFER // シェーダ側と合わせないと、おかしくなるよ！
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMMATRIX    matW;
		XMFLOAT4    diffuseColor;
		BOOL        isTexture;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		//XMFLOAT4 specular;
		//XMFLOAT4 ambient;
	};

	void    InitVertex(fbxsdk::FbxMesh* _mesh);
	void    InitIndex (fbxsdk::FbxMesh* _mesh);
	void    InitConstantBuffer();	//コンスタントバッファ準備
	void    InitMaterial(fbxsdk::FbxNode* _pNode);

	int vertexCount_;   //頂点数
	int polygonCount_;  //ポリゴン数
	int* indexCount_;    //インデックス数
	int materialCount_; //マテリアル数

	ID3D11Buffer*         pVertexBuffer_;
	ID3D11Buffer**        pIndexBuffer_;
	ID3D11Buffer*         pConstantBuffer_;
	std::vector<MATERIAL> materialList_;


};