#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(GameObject* _parent);
	~Bullet();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
	//Transform transform_;
	int hModel_;
	SphereCollider* pSphereCollider_;
};