#include "Enemy.h"
#include "Model.h"

Enemy::Enemy(GameObject* _parent) :
	GameObject(_parent, "Enemy"),
	hModel_(-1),
	transform_(),
	pSphereCollider_(nullptr)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	pSphereCollider_ = new SphereCollider(transform_.position, 1.f);
	hModel_ = Model::Load("Assets/models/Enemy.fbx");
}

void Enemy::Update()
{
	//transform_.position.x = 0.5f;
	transform_.scale = { 0.01, 0.01, 0.01 };
	Model::SetTransForm(hModel_, transform_);
}

void Enemy::Draw()
{
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
