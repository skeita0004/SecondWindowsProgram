#include "Enemy.h"
#include "Model.h"
#include "SphereCollider.h"

Enemy::Enemy(GameObject* _parent) :
	GameObject(_parent, "Enemy"),
	hModel_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
    SphereCollider* pSphereCollider = new SphereCollider(0.5f);
    AddCollider(pSphereCollider);
	hModel_ = Model::Load("Assets/models/Enemy.fbx");
	transform.position.x = 10.f;
	Model::SetTransForm(hModel_, &transform);
}

void Enemy::Update()
{
	static float r{};

	r += 0.1;

	transform.rotate.y = r;
	transform.scale.x = sinf(r) * 3;
	transform.scale.y = sinf(r) * 3;
	transform.scale.z = sinf(r) * 3;
	//transform_.position.x = 0.5f;
	//transform_.scale = { 0.01, 0.01, 0.01 };
}

void Enemy::Draw()
{
	Model::SetTransForm(hModel_, &transform);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
