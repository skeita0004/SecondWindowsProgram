#include "TitleLogo.h"
#include "Model.h"
#include <timeapi.h>
#include <algorithm>
#undef max

TitleLogo::TitleLogo(GameObject* _parent) :
    GameObject(_parent, "TitleLogo"),
    hModel_(-1)
{
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::Init()
{
    hModel_ = Model::Load("Assets/models/title.fbx");

    transform.position = { 0.f, 5.0, 0.f };
    Model::SetTransForm(hModel_, &transform);
}

void TitleLogo::Update()
{
    static float scale = 5;

    scale = std::max(5 * fabsf(sinf((float)timeGetTime() / 100)), 2.f);

    transform.scale = { scale, scale, scale };
}

void TitleLogo::Draw()
{
    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void TitleLogo::Release()
{
}
