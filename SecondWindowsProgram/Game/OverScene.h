#pragma once
#include "GameObject.h"

class SceneManager;

class OverScene : public GameObject
{
public:
    OverScene(GameObject* _parent);
    ~OverScene();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    SceneManager* pSceneManager_;
};