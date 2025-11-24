#include "ClearLogo.h"
#include "Model.h"

ClearLogo::ClearLogo(GameObject* _parent) :
    GameObject(_parent, "ClearLogo"),
    hModel_(-1)
{
}

ClearLogo::~ClearLogo()
{
}

void ClearLogo::Init()
{
    const float SCALE = 3.f;
    hModel_ = Model::Load("Assets/models/clear_logo.fbx");
    transform.position = XMFLOAT3(0.f, 50.f, 0.f);
    transform.scale = XMFLOAT3(SCALE, SCALE, SCALE);
}

void ClearLogo::Update()
{
    if (transform.position.y >= 0.f)
    {
        transform.position.y -= 1.f;
    }
}

void ClearLogo::Draw()
{
    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void ClearLogo::Release()
{
}
