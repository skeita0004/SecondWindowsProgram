#pragma once

#include "../Engine/GameObject.h"

class PlayScene : public GameObject
{
public:
	PlayScene(GameObject* _parent);
	~PlayScene();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;
};