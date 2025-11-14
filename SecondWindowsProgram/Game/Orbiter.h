#pragma once

#include "GameObject.h"

class Player;

class Orbiter : public GameObject
{
public:

    Orbiter(GameObject* _parent);
    ~Orbiter();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

    void SetPosOffset(const XMFLOAT3& _posOffset)
    {
        posOffset_ = _posOffset;
    }

private:
    int             hModel_;
    SphereCollider* pSphereCollider_;
    XMFLOAT3        posOffset_; // 親の位置から見た位置
    Player*         pPlayer_;
};