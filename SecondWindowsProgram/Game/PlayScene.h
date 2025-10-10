#pragma once

#include "GameObject.h"
#include "SceneManager.h"

class PlayScene : public GameObject
{
public:
	PlayScene(GameObject* _parent);
	~PlayScene();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
	SceneManager* pSceneManager_;
};