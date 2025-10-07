#pragma once
#include "GameObject.h"
#include "Fbx.h"

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
	Transform transform_;
	Fbx* pModel_;
};