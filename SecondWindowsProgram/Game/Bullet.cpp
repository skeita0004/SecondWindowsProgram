#include "Bullet.h"
#include "Player.h"
#include <string>
#include "Model.h"
#include "SphereCollider.h"
#include "Enemy.h"

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

	if (deadCounter >= 60 * 3)
	{
		KillMe();
		deadCounter = 0;
	}

	deadCounter++;

    // そして、ここで当たった敵が死んでいることを確認できたら、サヨナラするといい。
    if (isHit_ && FindGameObject<Enemy>(targetName_) == nullptr)
    {
        KillMe();
    }
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
    //isHit_ = false;
    std::string targetName = _pTarget->GetObjectName();
    std::string myName     = this->GetObjectName();

    //// ここで、当たった敵の名前をとっておく
    //if ((targetName == "Enemy"  and myName == "PlayerBullet") or
    //    (targetName == "Player" and myName == "EnemyBullet"))
    //{
    //    targetName_ = targetName;
    //    isHit_ = true;
    //}

    if (myName != targetName)
    {
        if (myName == "EnemyBullet" and targetName == "Player")
        {
             KillMe();
        }
        if (myName == "PlayerBullet" and targetName == "Enemy")
        {
            KillMe();
        }
        if (myName == "PlayerBullet" and targetName == "EnemyBullet")
        {
            KillMe();
        }
    }
}
