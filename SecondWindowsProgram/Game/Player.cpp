#include "Player.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/oden.fbx"};
}

Player::Player(GameObject* _parent) :
	GameObject(_parent, "Player"),
	hModel_(-1),
	transform_()
{
}

Player::~Player()
{
}

void Player::Init()
{
	hModel_ = Model::Load(MODEL_PATH);
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

	Model::SetTransForm(hModel_, transform_);
}

void Player::Draw()
{
	Model::Draw(hModel_);
}

void Player::Release()
{
}
