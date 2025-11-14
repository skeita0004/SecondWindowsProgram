#include "SceneManager.h"
#include "../Game/TestScene.h"
#include "../Game/TitleScene.h"
#include "../Game/PlayScene.h"
#include "../Game/ClearScene.h"
#include "../Game/OverScene.h"
#include "Model.h"

SceneManager::SceneManager(GameObject* _parent) :
	GameObject(_parent, "SceneManager"),
	currScene(SceneID::SID_TITLE),
	nextScene(SceneID::SID_TITLE)
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
            using enum SceneID;
			case SceneID::SID_TEST:
				Instantiate<TestScene>(this);
				break;
			case SceneID::SID_TITLE:
				Instantiate<TitleScene>(this);
				break;
			case SceneID::SID_PLAY:
				Instantiate<PlayScene>(this);
				break;
			case SceneID::SID_CLEAR:
                Instantiate<ClearScene>(this);
				break;
			case SceneID::SID_OVER:
                Instantiate<OverScene>(this);
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

void SceneManager::ChangeScene(SceneID _sceneID)
{
	nextScene = _sceneID;
}
