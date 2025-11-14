#pragma once
#include "GameObject.h"

class SceneManager;

class ClearScene : public GameObject
{
public:
    ClearScene(GameObject* _parent);
    ~ClearScene();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    SceneManager* pSceneManager_;
    int hModel_;
};