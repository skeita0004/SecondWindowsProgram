#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//位置、向き、拡大率などを管理するクラス
class Transform
{
private:
	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;	//拡大行列

public:
	XMFLOAT3 position;	//位置
	XMFLOAT3 rotate;	//向き
	XMFLOAT3 scale;	//拡大率

	//コンストラクタ
	Transform();
	Transform(XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale);

	//デストラクタ
	~Transform();

	//各行列の計算
	void Calculation();

	//ワールド行列を取得
	const XMMATRIX GetWorldMatrix();
	const XMMATRIX GetNormalMatrix();
};