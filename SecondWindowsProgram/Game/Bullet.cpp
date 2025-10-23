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
	SphereCollider* pSphereCollider = new SphereCollider(transform_.position, 1.f);
	AddCollider(pSphereCollider);
	hModel_ = Model::Load(MODEL_PATH);
}

void Bullet::Update()
{
	transform_.rotate.y = 90;

    XMVECTOR vPos = XMLoadFloat3(&transform_.position);
    XMVECTOR vVelocity = XMLoadFloat3(&velocity_);

    vPos += vVelocity;

    XMStoreFloat3(&transform_.position, vPos);
    XMStoreFloat3(&velocity_, vVelocity);

    

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
