#include "PlayScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Input.h"

PlayScene::PlayScene(GameObject* _parent) :
	GameObject(_parent, "PlayScene"),
	pSceneManager_(nullptr)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	
	pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
}

void PlayScene::Update()
{
	if (Input::IsKey(DIK_T))
	{
		pSceneManager_->ChangeScene(SceneManager::SceneID::SID_TEST);
	}
}

void PlayScene::Draw()
{
	
}

void PlayScene::Release()
{
}
