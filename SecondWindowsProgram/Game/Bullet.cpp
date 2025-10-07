#include "Bullet.h"
#include "Player.h"
#include <string>

namespace
{
	const std::string MODEL_PATH{"Assets/models/mochi_kinchaku.fbx"};
	int deadCounter = 0;
}

Bullet::Bullet(GameObject* _parent):
	GameObject(_parent, "Bullet"),
	transform_(),
	pModel_(nullptr)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	pModel_ = new Fbx();
	pModel_->Load(MODEL_PATH);
}

void Bullet::Update()
{
	transform_.rotate.y = 90;
	transform_.position.z--;

	if (deadCounter >= 60 * 3)
	{
		KillMe();
		deadCounter = 0;
	}

	deadCounter++;
}

void Bullet::Draw()
{
	pModel_->Draw(transform_);
}

void Bullet::Release()
{
	SAFE_RELEASE(pModel_);
}
