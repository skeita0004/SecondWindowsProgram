#pragma once
#include "GameObject.h"

class ClearLogo : public GameObject
{
public:
    ClearLogo(GameObject* _parent);
    ~ClearLogo();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int       hModel_;
};
