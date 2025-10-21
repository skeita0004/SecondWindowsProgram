#pragma once

#include "GameObject.h"

class Orbiter : public GameObject
{
public:
    Orbiter(GameObject* _parent);
    ~Orbiter();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int       hModel_;
    //Transform transform_;
    SphereCollider* pSphereCollider_;
};