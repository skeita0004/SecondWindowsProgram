#include "Fbx.h"
#include "Camera.h"
#include <filesystem>
#include <sstream>
#include "SafeCleaning.h"

namespace fs = std::filesystem;

namespace
{

}

Fbx::Fbx() :
    pVertexBuffer_(nullptr),
    pIndexBuffer_(nullptr),
    pConstantBuffer_(nullptr),
    vertexCount_(0),
    polygonCount_(0),
	indexCount_(0),
	materialCount_(0),
	materialList_()
{
}

HRESULT Fbx::Load(std::string _fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	if (not(fbxImporter->Initialize(_fileName.c_str(), -1, pFbxManager->GetIOSettings())))
	{
		return E_FAIL;
	}

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = mesh->GetPolygonCount();	    //ポリゴンの数
	materialCount_ = pNode->GetMaterialCount();     // マテリアルの数

	wchar_t defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// TTTOOODDDOOO：引数からとれよ！
	SetCurrentDirectory(L"Assets/models/");

	indexCount_ = new int[materialCount_];

	InitVertex(mesh);		//頂点バッファ準備
	InitIndex(mesh);
	InitConstantBuffer();
	InitMaterial(pNode);

	SetCurrentDirectory(defaultCurrentDir);

	//マネージャ解放
	pFbxManager->Destroy();

	return S_OK;
}

//頂点バッファ準備
void Fbx::InitVertex(fbxsdk::FbxMesh* _mesh)
{
	HRESULT hResult;
	//頂点情報を入れる配列
	pVertexData_ = new VERTEX[vertexCount_];

	//全ポリゴン
	for (int poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = _mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = _mesh->GetControlPointAt(index);
			pVertexData_[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

#if false
#define WRONG_CODE
			//頂点のUV
			FbxLayerElementUV* pUV = _mesh->GetLayer(0)->GetUVs();
			int uvIndex = _mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)( 1.0f - uv.mData[1] ), 0.0f, 0.0f);
#endif

#ifndef WRONG_CODE
			///*GPTs CODE*/
			FbxLayerElementUV* pUV = _mesh->GetLayer(0)->GetUVs();
			if (pUV)
			{
				FbxVector2 uv;
				bool unmapped = false;

				if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint)
				{
					int controlPointIndex = _mesh->GetPolygonVertex(poly, vertex);

					if (pUV->GetReferenceMode() == FbxLayerElement::eDirect)
					{
						uv = pUV->GetDirectArray().GetAt(controlPointIndex);
					}
					else if (pUV->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						int id = pUV->GetIndexArray().GetAt(controlPointIndex);
						uv = pUV->GetDirectArray().GetAt(id);
					}
				}
				else if (pUV->GetMappingMode() == FbxLayerElement::eByPolygonVertex)
				{
					int uvIndex = _mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);

					if (pUV->GetReferenceMode() == FbxLayerElement::eDirect ||
						pUV->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						uv = pUV->GetDirectArray().GetAt(uvIndex);
					}
				}

				pVertexData_[index].uv = XMVectorSet((float)uv[0], 1.0f - (float)uv[1], 0.0f, 0.0f);
			}
			/*END GPTs CODE*/
#endif

			//頂点の法線
			FbxVector4 Normal;
			_mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			pVertexData_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}

	// 頂点バッファ作成
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = static_cast<UINT>( sizeof(VERTEX) * vertexCount_ );
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertexData_;

	hResult = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"頂点がなんか変です", L"エラー", MB_OK);
	}

	//delete[] pVertexData_;
}

void Fbx::InitIndex(fbxsdk::FbxMesh* _mesh)
{
	HRESULT hResult;

	pIndexBuffer_ = new ID3D11Buffer* [materialCount_];

	int* index = new int[polygonCount_ * 3];

	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;

		//全ポリゴン
		for (int poly = 0; poly < polygonCount_; poly++)
		{

			FbxLayerElementMaterial* mtl = _mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3頂点分
				for (int vertex = 0; vertex < 3; vertex++)
				{
					index[count] = _mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}

		// インデックスバッファを生成する
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		hResult = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hResult))
		{
			MessageBox(nullptr, L"インデックスバッファがなんか変です", L"鰓ー", MB_OK);
		}
		indexCount_[i] = count;
	}

    SafeCleaning::SafeDeleteArray(index);
}

void Fbx::InitConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(Fbx::CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
}

void Fbx::InitMaterial(fbxsdk::FbxNode* _pNode)
{
	//pMaterialList_ = new MATERIAL[materialCount_];
	materialList_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++)
	{
		//i番目のマテリアル情報を取得
		FbxSurfaceMaterial* pMaterial = _pNode->GetMaterial(i);

		//テクスチャ情報
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//テクスチャの枚数
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//テクスチャあり
		if (fileTextureCount > 0)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			fs::path txFilePath(textureFilePath);
			std::string txFileName = txFilePath.filename().string();
			//std::ostringstream pathOss;


			//pathOss << "Assets/models/" << txFileName;

			//txFileName = pathOss.str();
			                                                 //にどでま
			fs::path newFilePath(txFileName); // にどでまでは？二度手間

			if (fs::is_regular_file(newFilePath))
			{
				//ファイルからテクスチャ作成
				materialList_[i].pTexture = new Texture();
			
				if (FAILED(materialList_[i].pTexture->Load(txFileName)))
				{
					MessageBox(nullptr, L"テクスチャの読み込みに失敗しました。", L"エラー", MB_OKCANCEL | MB_ICONWARNING);
				}
				
			}
			else
			{
				// ﾌｧｲﾙなし
			}
		}
		else	//テクスチャ無し
		{
			materialList_[i].pTexture = nullptr;

			//マテリアルの色
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)_pNode->GetMaterial(i);
			FbxDouble3  diffuse = pMaterial->Diffuse;
			materialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		}
	}
}

void Fbx::InitSkelton(FbxMesh* _pMesh)
{
    const int ROOT_INDEX{0};

    // デフォーマ情報を取得
    FbxDeformer* pDeformer = _pMesh->GetDeformer(ROOT_INDEX);
    if (pDeformer == nullptr)
    {
        return;
    }

    pSkinInfo_ = static_cast<FbxSkin*>(pDeformer);

    struct PolyIndex
    {
        int* polyIndex;
        int* vertexIndex;
        int  numRef;
    };

    const int POLYGON_PER_VERTEX{3};
    PolyIndex* polyTable = new PolyIndex[vertexCount_];
    for (DWORD i = 0; i < vertexCount_; i++)
    {
        // 領域確保
        polyTable[i].polyIndex   = new int[polygonCount_ * POLYGON_PER_VERTEX];
        polyTable[i].vertexIndex = new int[polygonCount_ * POLYGON_PER_VERTEX];
        polyTable[i].numRef      = 0;

        ZeroMemory(polyTable[i].polyIndex,   sizeof(polyTable[i].polyIndex));
        ZeroMemory(polyTable[i].vertexIndex, sizeof(polyTable[i].vertexIndex));

        for (DWORD j = 0; j < polygonCount_; j++)
        {
            for (int k = 0; k < POLYGON_PER_VERTEX; k++)
            {
                if (_pMesh->GetPolygonVertex(j, k) == i)
                {
                    polyTable[i].polyIndex[polyTable[i].numRef]   = j;
                    polyTable[i].vertexIndex[polyTable[i].numRef] = k;
                    polyTable[i].numRef++;
                }
            }
        }
    }

    numBone_ = pSkinInfo_->GetClusterCount();
    ppCluster_ = new FbxCluster*[numBone_];
    for (int i = 0; i < numBone_; i++)
    {
        ppCluster_[i] = pSkinInfo_->GetCluster(i);
    }

    // ボーン分のウェイト情報の準備
    pWeight_ = new Weight[vertexCount_];
    for (DWORD i = 0; i < vertexCount_; i++)
    {
        pWeight_[i].posOrigin = pVertexData_[i].position;
        pWeight_[i].normalOrigin = pVertexData_[i].normal;
        pWeight_[i].pBoneIndex = new int[numBone_];
        pWeight_[i].pBoneWeight = new float[numBone_];
        for (int j = 0; j < numBone_; j++)
        {
            pWeight_[i].pBoneIndex[j] = -1;
            pWeight_[i].pBoneWeight[j] = 0.0f;
        }
    }

    for (int i = 0; i < numBone_; i++)
    {
        int  numIndex = ppCluster_[i]->GetControlPointIndicesCount();
        int* piIndex  = ppCluster_[i]->GetControlPointIndices();
        double* pdWeight = ppCluster_[i]->GetControlPointWeights();

        for (int j = 0; j < numIndex; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if (k >= numBone_)
                {
                    break;
                }

                if (pdWeight[j] > pWeight_[piIndex[k]].pBoneWeight[k])
                {
                    for (int m = numBone_ - 1; m > k; m--)
                    {
                        pWeight_[piIndex[j]].pBoneIndex[m] = pWeight_[piIndex[j]].pBoneIndex[m - 1];
                        pWeight_[piIndex[j]].pBoneWeight[m] = pWeight_[piIndex[j]].pBoneWeight[m - 1];
                    }
                    pWeight_[piIndex[j]].pBoneIndex[k] = i;
                    pWeight_[piIndex[j]].pBoneWeight[k] = static_cast<float>(pdWeight[k]);
                    break;
                }
            }
        }
    }

    pBone_ = new Bone[numBone_];
    for (int i = 0; i < numBone_; i++)
    {
        FbxAMatrix matrix;
        ppCluster_[i]->GetTransformLinkMatrix(matrix);

        XMFLOAT4X4 pose;
        for (DWORD x = 0; x < 4; x++)
        {
            for (DWORD y = 0; y < 4; y++)
            {
                pose(x, y) = static_cast<float>(matrix.Get(x, y));
            }
        }
        pBone_[i].mBindPose = XMLoadFloat4x4(&pose);
    }

    for (DWORD i = 0; i < vertexCount_; i++)
    {
        SafeCleaning::SafeDeleteArray(polyTable[i].polyIndex);
        SafeCleaning::SafeDeleteArray(polyTable[i].vertexIndex);
    }
    SafeCleaning::SafeDeleteArray(polyTable);
}

void Fbx::Draw(Transform& _transform)
{
	Direct3D::SetShader(SHADER_3D);

	for (int i = 0; i < materialCount_; i++)
	{
		XMMATRIX worldMat = _transform.GetWorldMatrix(); // ここで計算されるのですわ～
		D3D11_MAPPED_SUBRESOURCE pdata;
		Fbx::CONSTANT_BUFFER cb{};
		cb.matWVP = XMMatrixTranspose(worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matNormal = XMMatrixTranspose(_transform.GetNormalMatrix());
		cb.matW = worldMat;
		cb.diffuseColor = materialList_[i].diffuse;
		cb.isTexture = (BOOL)(materialList_[i].pTexture != nullptr);

		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)( &cb ), sizeof(cb));	// データの値を送る

		if (materialList_[i].pTexture != nullptr)
		{
			ID3D11SamplerState* pSampler = materialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = materialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}

		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

		//頂点バッファ
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		// インデックスバッファーをセット
		//stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void Fbx::Release()
{
    SafeCleaning::SafeRelease(pVertexBuffer_);

    for (int i = 0; i < materialCount_; i++)
    {
        SafeCleaning::SafeRelease(pIndexBuffer_[i]);
    }

    SafeCleaning::SafeDeleteArray(pIndexBuffer_);
    SafeCleaning::SafeRelease(pConstantBuffer_);
}

