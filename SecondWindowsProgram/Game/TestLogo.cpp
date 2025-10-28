#include "TestLogo.h"
#include "Model.h"
#include <timeapi.h>
#include <algorithm>
#undef max

TestLogo::TestLogo(GameObject* _parent) :
    GameObject(_parent, "TestLogo"),
    hModel_(-1)
{
}

TestLogo::~TestLogo()
{
}

void TestLogo::Init()
{
    hModel_ = Model::Load("Assets/models/title.fbx");

    transform.position = { 0.f, 5.0, 0.f };
    Model::SetTransForm(hModel_, &transform);
}

void TestLogo::Update()
{
    static float scale = 5;

    scale = std::max(5 * fabsf(sinf((float)timeGetTime() / 100)), 2.f);

    transform.scale = {scale, scale, scale };
}

void TestLogo::Draw()
{
    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void TestLogo::Release()
{
}
