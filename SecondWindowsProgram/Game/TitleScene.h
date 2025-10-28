#pragma once
#include "GameObject.h"

class SceneManager;

class TitleScene : public GameObject
{
public:
    TitleScene(GameObject* _parent);
    ~TitleScene();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    SceneManager* pSceneManager_;
    int           hModel_;
};