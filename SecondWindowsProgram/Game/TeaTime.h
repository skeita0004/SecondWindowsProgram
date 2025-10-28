#pragma once
#include "GameObject.h"

class TeaTime : public GameObject
{
public:
    TeaTime(GameObject* _parent);
    ~TeaTime();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int       hModel_;
};