#include "Player.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"
#include "Orbiter.h"
#include "Camera.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/oden.fbx"};
}

Player::Player(GameObject* _parent) :
    GameObject(_parent, "Player"),
	hModel_(-1),
	pSphereCollider_(nullptr)
{
}

Player::~Player()
{
}

void Player::Init()
{
	pSphereCollider_ = new SphereCollider(transform_.position, 1.f);
	hModel_ = Model::Load(MODEL_PATH);
    pOrbiterLeft_ = Instantiate<Orbiter>(this);
    pOrbiterRight_ = Instantiate<Orbiter>(this);
}

void Player::Update()
{
	transform_.rotate.y--;

    //XMFLOAT3 posROrb{};
    //XMFLOAT3 posLOrb{};
    //
    //XMVECTOR vPos = XMLoadFloat3(&transform_.position);
    //XMVECTOR vPosROrb = XMVectorSet(3, 3, 0, 0);
    //XMVECTOR vPosLOrb = XMVectorSet(-3, 3, 0, 0);

    //vPosROrb += vPos;
    //vPosLOrb += vPos;

    //XMStoreFloat3(&posROrb, vPosROrb);
    //XMStoreFloat3(&posLOrb, vPosLOrb);

    pOrbiterLeft_ ->SetPosition({-3, 3, 0});
    pOrbiterRight_->SetPosition({3, 3, 0});


    if (Input::IsKeyDown(DIK_SPACE))
    {
        Bullet* pBullet = Instantiate<Bullet>(this->GetParent());
        pBullet->SetPosition(transform_.position);
        pBullet->SetVelocity({1.f, 0, 0});
	}

	if (Input::IsKey(DIK_A))
	{
		transform_.position.x -= 0.2f;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position.x += 0.2f;
	}
    if (Input::IsKey(DIK_W))
    {
        transform_.position.z += 0.2f;
    }
    if (Input::IsKey(DIK_S))
    {
        transform_.position.z -= 0.2f;
    }
	//Camera::SetTarget(XMLoadFloat3(&transform_.position));
}

void Player::Draw()
{
    Model::SetTransForm(hModel_, &transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
