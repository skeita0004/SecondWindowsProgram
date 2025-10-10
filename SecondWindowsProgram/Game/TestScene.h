#pragma once

#include "GameObject.h"
#include "SceneManager.h"

class TestScene : public GameObject
{
public:
	TestScene(GameObject* _parent);
	~TestScene();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
	SceneManager* pSceneManager;
};