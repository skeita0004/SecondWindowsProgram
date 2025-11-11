#pragma once

#include "GameObject.h"

class SceneManager : public GameObject
{
public:
	enum struct SceneID : int
	{
		SID_TEST = 0,
		SID_TITLE,
		SID_PLAY,
		SID_CLEAR,
		SID_OVER,
	};

	SceneManager(GameObject* _parent);
	~SceneManager();
	
	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

	void ChangeScene(SceneID _sceneID);

private:
	SceneID currScene;
	SceneID nextScene;
};