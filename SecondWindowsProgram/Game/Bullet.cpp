#include "Bullet.h"
#include "Player.h"
#include <string>
#include "Model.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/mochi_kinchaku.fbx"};
	int deadCounter = 0;
}

Bullet::Bullet(GameObject* _parent):
	GameObject(_parent, "Bullet"),
	hModel_(-1)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	pSphereCollider_ = new SphereCollider(transform_.position, 1.f);
	AddCollider(pSphereCollider_);
	hModel_ = Model::Load(MODEL_PATH);
}

void Bullet::Update()
{
	transform_.rotate.y = 90;
	transform_.position.x++;


	if (deadCounter >= 60 * 3)
	{
		KillMe();
		deadCounter = 0;
	}

	deadCounter++;
}

void Bullet::Draw()
{
	Model::SetTransForm(hModel_, &transform_);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}
