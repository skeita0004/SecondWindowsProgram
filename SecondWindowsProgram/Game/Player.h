#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(GameObject* _parent);
	~Player();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
	int       hModel_;
	Transform transform_;
	SphereCollider* pSphereCollider_;
};