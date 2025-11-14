#include "TestScene.h"
#include "SceneManager.h"
#include "Model.h"
#include "Input.h"
#include "TeaTime.h"
#include "TestLogo.h"

TestScene::TestScene(GameObject* _parent) :
	GameObject(_parent, "TestScene"),
	pSceneManager_(nullptr)
{
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
	pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
    Instantiate<TeaTime>(this);
    Instantiate<TestLogo>(this);
}

void TestScene::Update()
{
    if (Input::IsKeyDown(DIK_P))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_PLAY);
    }
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
