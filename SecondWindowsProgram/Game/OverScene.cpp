#include "OverScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "OverLogo.h"

OverScene::OverScene(GameObject* _parent) :
    GameObject(_parent, "OverScene"),
    pSceneManager_()
{
}

OverScene::~OverScene()
{
}

void OverScene::Init()
{
    pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
    Instantiate<OverLogo>(this);
}

void OverScene::Update()
{
    if (Input::IsKeyDown(DIK_T))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_TITLE);
    }
}

void OverScene::Draw()
{
}

void OverScene::Release()
{
}
