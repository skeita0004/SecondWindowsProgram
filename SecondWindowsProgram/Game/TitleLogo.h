#pragma once
#include "GameObject.h"

class TitleLogo : public GameObject
{
public:
    TitleLogo(GameObject* _parent);
    ~TitleLogo();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int       hModel_;
};
