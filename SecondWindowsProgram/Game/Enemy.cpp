#include "Enemy.h"
#include "Model.h"
#include "SphereCollider.h"
#include <ctime>
#include "Bullet.h"

Enemy::Enemy(GameObject* _parent) :
	GameObject(_parent, "Enemy"),
	hModel_(-1),
    pSceneManager_()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
    SphereCollider* pSphereCollider = new SphereCollider(3.f);
    AddCollider(pSphereCollider);
	hModel_ = Model::Load("Assets/models/Enemy.fbx");
    transform.position.x = 0.f;
    transform.position.y = 0.f;
    transform.position.z = 50.f;
	Model::SetTransForm(hModel_, &transform);

    pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Enemy::Update()
{
	static float r{};
    static float x{};
	r += 0.1;
    x += 0.05f;
	transform.rotate.y = r;
	transform.scale.x = sinf(r) * 3;
	transform.scale.y = sinf(r) * 3;
	transform.scale.z = sinf(r) * 3;

    transform.position.x = sinf(x) * 30;

    int shotRoulette = rand() % 50;

    // ランダムな間隔で撃つ
    // ルーレットで3が出たら撃つ（クソコードでは？）
    if (/*shotRoulette == 3*/ true)
    {
         Bullet* pBullet = Instantiate<Bullet>(this->GetParent());
         pBullet->SetObjectName("EnemyBullet");
         pBullet->SetOwner(this);
         pBullet->SetVelocity(XMFLOAT3(0.f, 0.f, -1.f));

         pBullet->SetPosition(transform.position);
    }
}

void Enemy::Draw()
{
	Model::SetTransForm(hModel_, &transform);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "PlayerBullet")
    {
        KillMe();

        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_CLEAR);
    }
}
