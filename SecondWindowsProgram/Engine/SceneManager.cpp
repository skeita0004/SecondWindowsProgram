#include "SceneManager.h"
#include "../Game/TestScene.h"
#include "../Game/PlayScene.h"
#include "Model.h"

SceneManager::SceneManager(GameObject* _parent) :
	GameObject(_parent, "SceneManager"),
	currScene(Scene::SceneID::SID_TEST),
	nextScene(Scene::SceneID::SID_TEST)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	if (currScene != nextScene)
	{
		DeleteAllChildren();

		Model::AllRelease();

		switch (nextScene)
		{
			case Scene::SceneID::SID_TEST:
				Instantiate<TestScene>(this);
				break;
			case Scene::SceneID::SID_PLAY:
				Instantiate<PlayScene>(this);
				break;
			case Scene::SceneID::SID_TITLE:
				break;
			case Scene::SceneID::SID_OVER:
				break;
			case Scene::SceneID::SID_CLEAR:
				break;
			default:
				break;
		}
		currScene = nextScene;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(Scene::SceneID _sceneID)
{
	nextScene = _sceneID;
}
