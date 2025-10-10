#include "TestScene.h"
#include "Input.h"

TestScene::TestScene(GameObject* _parent) :
	GameObject(_parent, "TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
	pSceneManager = FindGameObject<SceneManager>("SceneManager");
}

void TestScene::Update()
{
	if (Input::IsKey(DIK_P))
	{
		pSceneManager->ChangeScene(Scene::SceneID::SID_PLAY);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
