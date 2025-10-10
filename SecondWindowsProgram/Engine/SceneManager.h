#pragma once

#include "GameObject.h"

namespace Scene
{
	enum struct SceneID
	{
		SID_TEST = 0,
		SID_PLAY,
		SID_TITLE,
		SID_OVER,
		SID_CLEAR,
	};
}

class SceneManager : public GameObject
{
public:
	SceneManager(GameObject* _parent);
	~SceneManager();
	
	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

	void ChangeScene(Scene::SceneID _sceneID);

private:
	Scene::SceneID currScene;
	Scene::SceneID nextScene;
};