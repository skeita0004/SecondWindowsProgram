#include "Bullet.h"
#include "Player.h"
#include <string>
#include "Model.h"
#include "SphereCollider.h"
#include "Enemy.h"

namespace
{
	const std::string MODEL_PATH{"Assets/models/mochi_kinchaku.fbx"};
}

Bullet::Bullet(GameObject* _parent):
	GameObject(_parent, "Bullet"),
	hModel_(-1),
    deadCounter_(0)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	SphereCollider* pSphereCollider = new SphereCollider(0.5f);
	AddCollider(pSphereCollider);
	hModel_ = Model::Load(MODEL_PATH);
}

void Bullet::Update()
{
	transform.rotate.y = 90;

    XMVECTOR vPos = XMLoadFloat3(&transform.position);
    XMVECTOR vVelocity = XMLoadFloat3(&velocity_);

    vPos += vVelocity;

    XMStoreFloat3(&transform.position, vPos);
    XMStoreFloat3(&velocity_, vVelocity);

    if (deadCounter_ >= 60 * 3)
	{
		KillMe();
	}
	deadCounter_++;
}

void Bullet::Draw()
{
	Model::SetTransForm(hModel_, &transform);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}

void Bullet::OnCollision(GameObject* _pTarget)
{
    //// 当たったやつが自分を発射したものでなければ消える
    //if (_pTarget != owner_)
    //{
    //    KillMe();
    //}
}
