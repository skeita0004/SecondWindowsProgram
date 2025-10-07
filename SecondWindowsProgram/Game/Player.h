#pragma once

#include "GameObject.h"
#include "Fbx.h"

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
	Fbx* pModel_;
	Transform transform_;
};