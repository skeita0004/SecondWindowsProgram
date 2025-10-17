#include "Player.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"
#include "Orbiter.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/oden.fbx"};
}

Player::Player(GameObject* _parent) :
	GameObject(_parent, "Player"),
	hModel_(-1),
	transform_(),
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

    pOrbiterLeft_ ->SetPosition(XMFLOAT3{ 3, 3, 0});
    pOrbiterRight_->SetPosition(XMFLOAT3{-3, 3, 0});

    if (Input::IsKeyDown(DIK_SPACE))
    {
        Bullet* pBullet = Instantiate<Bullet>(this);
        //pBullet->Settransform
	}

	if (Input::IsKey(DIK_A))
	{
		transform_.position.x -= 0.2f;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position.x += 0.2f;
	}

	Model::SetTransForm(hModel_, transform_);
}

void Player::Draw()
{
	Model::Draw(hModel_);
}

void Player::Release()
{
}
