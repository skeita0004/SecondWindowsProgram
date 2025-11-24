#pragma once
#include "GameObject.h"

class OverLogo : public GameObject
{
public:
    OverLogo(GameObject* _parent);
    ~OverLogo();

    void Init()    override;
    void Update()  override;
    void Draw()    override;
    void Release() override;

private:
    int hPicture_;
};
