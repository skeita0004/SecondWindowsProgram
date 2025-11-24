#include "OverLogo.h"
#include "Picture.h"

OverLogo::OverLogo(GameObject* _parent) :
    GameObject(_parent, "OverLogo")
{
}

OverLogo::~OverLogo()
{
}

void OverLogo::Init()
{
    hPicture_ = Picture::Load("Assets/overlogo.png");
}

void OverLogo::Update()
{
}

void OverLogo::Draw()
{
    Picture::SetTransForm(hPicture_, &transform);
    Picture::Draw(hPicture_);
}

void OverLogo::Release()
{
}
