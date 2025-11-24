#include "ClearScene.h"
#include "Sprite.h"
#include "Player.h"
#include "SceneManager.h"
#include "Input.h"
#include "Model.h"
#include "ClearLogo.h"

ClearScene::ClearScene(GameObject* _parent) :
    GameObject(_parent, "ClearScene"),
    pSceneManager_()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
    Instantiate<Player>(this);
    Instantiate<ClearLogo>(this);
}

void ClearScene::Update()
{
    if (Input::IsKeyDown(DIK_T))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_TITLE);
    }
}

void ClearScene::Draw()
{
}

void ClearScene::Release()
{
}
