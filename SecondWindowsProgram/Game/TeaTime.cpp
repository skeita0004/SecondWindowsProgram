#include "TeaTime.h"
#include "Model.h"

TeaTime::TeaTime(GameObject* _parent) :
    GameObject(_parent, "TeaTime"),
    hModel_(-1)
{
}

TeaTime::~TeaTime()
{
}

void TeaTime::Init()
{
    hModel_ = Model::Load("Assets/models/teaset.fbx");

    transform.position = {0.f, -5.0, 0.f};
    transform.rotate.z = 20;
    transform.scale = { 2.5f, 2.5f, 2.5f };
    Model::SetTransForm(hModel_, &transform);
}

void TeaTime::Update()
{
    transform.rotate.y -= 2.0f;
}

void TeaTime::Draw()
{
    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void TeaTime::Release()
{
}
