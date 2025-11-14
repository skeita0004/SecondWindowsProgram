#include "TitleScene.h"
#include "SceneManager.h"
#include "Model.h"
#include "Input.h"
#include "TeaTime.h"
#include "TitleLogo.h"

TitleScene::TitleScene(GameObject* _parent) :
    GameObject(_parent, "TitleScene"),
    pSceneManager_(nullptr),
    hModel_(-1)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
    pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
    Instantiate<TeaTime>(this);
    Instantiate<TitleLogo>(this);
}

void TitleScene::Update()
{
    if (Input::IsKeyDown(DIK_P))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_PLAY);
    }
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
