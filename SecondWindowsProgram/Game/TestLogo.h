#pragma once
#include "GameObject.h"

class TestLogo : public GameObject
{
public:
    TestLogo(GameObject* _parent);
    ~TestLogo();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int       hModel_;
};
