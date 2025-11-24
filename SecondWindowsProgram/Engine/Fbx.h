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

    bool    GetBonePosition(std::string _boneName,
                            XMFLOAT3*   _pPosition);

    bool    GetBoneRotation(std::string _boneName,
                            XMFLOAT3*   _pRotation);

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
		XMFLOAT3 position;
		XMFLOAT3 uv;
		XMFLOAT3 normal;
        
	};

	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		//XMFLOAT4 specular;
		//XMFLOAT4 ambient;
	};

    struct Bone
    {
        XMMATRIX mBindPose;
        XMMATRIX mNewPose;
        XMMATRIX mDiffPose;
    };

    struct Weight
    {
        XMFLOAT3 posOrigin;
        XMFLOAT3 normalOrigin;
        int*     pBoneIndex;
        float*   pBoneWeight;
    };

	void    InitVertex(fbxsdk::FbxMesh* _mesh);
	void    InitIndex (fbxsdk::FbxMesh* _mesh);
	void    InitConstantBuffer();	//コンスタントバッファ準備
	void    InitMaterial(fbxsdk::FbxNode* _pNode);
    void    InitSkelton(FbxMesh* _pMesh); // スケルトン準備

	DWORD vertexCount_;   //頂点数
	DWORD polygonCount_;  //ポリゴン数
	DWORD* indexCount_;    //インデックス数
	DWORD materialCount_; //マテリアル数

    VERTEX* pVertexData_;

    FbxSkin*     pSkinInfo_;
    FbxCluster** ppCluster_;
    int          numBone_;
    Bone*        pBone_;
    Weight*      pWeight_;


	ID3D11Buffer*         pVertexBuffer_;
	ID3D11Buffer**        pIndexBuffer_;
	ID3D11Buffer*         pConstantBuffer_;
	std::vector<MATERIAL> materialList_;
};