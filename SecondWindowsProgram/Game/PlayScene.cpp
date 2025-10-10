#include "PlayScene.h"
#include "Player.h"
#include "Input.h"

PlayScene::PlayScene(GameObject* _parent) :
	GameObject(_parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	Instantiate<Player>(this);
	pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
}

void PlayScene::Update()
{
	if (Input::IsKey(DIK_T))
	{
		pSceneManager_->ChangeScene(Scene::SceneID::SID_TEST);
	}
}

void PlayScene::Draw()
{
	
}

void PlayScene::Release()
{
}
