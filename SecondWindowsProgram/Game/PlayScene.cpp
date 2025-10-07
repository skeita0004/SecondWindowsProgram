#include "PlayScene.h"
#include "Player.h"

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
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
	
}

void PlayScene::Release()
{
}
