#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(GameObject* _parent);
	~Enemy();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
	int       hModel_;
	Transform transform_;
	SphereCollider* pSphereCollider_;
};