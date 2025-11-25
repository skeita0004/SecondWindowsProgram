#include "OverLogo.h"
#include "Picture.h"
#include "Model.h"

OverLogo::OverLogo(GameObject* _parent) :
    GameObject(_parent, "OverLogo"),
    hModel_(-1)
{
}

OverLogo::~OverLogo()
{
}

void OverLogo::Init()
{
    hPicture_ = Picture::Load("Assets/overlogo.png");
    hModel_   = Model::Load("Assets/models/overlogo.fbx");
    transform.scale = XMFLOAT3(5.f, 5.f, 5.f);
}

void OverLogo::Update()
{
}

void OverLogo::Draw()
{
    Picture::SetTransForm(hPicture_, &transform);
    Picture::Draw(hPicture_);

    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void OverLogo::Release()
{
}
