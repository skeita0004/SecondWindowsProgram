#include "Player.h"
#include <string>
#include "Input.h"
#include "Bullet.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/oden.fbx"};
}

Player::Player(GameObject* _parent) :
	GameObject(_parent, "Player"),
	pModel_(nullptr),
	transform_()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pModel_ = new Fbx();
	pModel_->Load(MODEL_PATH);
}

void Player::Update()
{
	transform_.rotate.y--;

	if (Input::IsKeyDown(DIK_SPACE))
	{
		Instantiate<Bullet>(this);
	}

	if (Input::IsKey(DIK_A))
	{
		transform_.position.x -= 0.2f;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position.x += 0.2f;
	}
}

void Player::Draw()
{
	pModel_->Draw(transform_);
}

void Player::Release()
{
	pModel_->Release();
}
