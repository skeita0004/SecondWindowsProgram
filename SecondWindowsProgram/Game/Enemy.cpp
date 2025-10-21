#include "Enemy.h"
#include "Model.h"

Enemy::Enemy(GameObject* _parent) :
	GameObject(_parent, "Enemy"),
	hModel_(-1),
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
	transform_.position.x = 10.f;
	Model::SetTransForm(hModel_, &transform_);
}

void Enemy::Update()
{
	static float r{};

	r += 0.1;

	transform_.rotate.y = r;
	transform_.scale.x = sinf(r) * 3;
	transform_.scale.y = sinf(r) * 3;
	transform_.scale.z = sinf(r) * 3;
	//transform_.position.x = 0.5f;
	//transform_.scale = { 0.01, 0.01, 0.01 };
}

void Enemy::Draw()
{
	Model::SetTransForm(hModel_, &transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
