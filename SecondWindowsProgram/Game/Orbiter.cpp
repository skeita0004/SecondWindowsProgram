#include "Orbiter.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"

namespace
{
    const std::string MODEL_PATH{ "Assets/models/mochi_kinchaku.fbx" };
}

Orbiter::Orbiter(GameObject* _parent) :
    GameObject(_parent, "Orbiter"),
    hModel_(-1),
    pSphereCollider_(nullptr)
{
}

Orbiter::~Orbiter()
{
}

void Orbiter::Init()
{
    // pSphereCollider_ = new SphereCollider(transform_.position, 1.f);
    //transform_.pParent = GetParent()->GetTransform();
    hModel_ = Model::Load(MODEL_PATH);
    //transform_.rotate.y = 90;
    // Model::SetTransForm(hModel_, &transform_);
}

void Orbiter::Update()
{
    transform_.scale = {0.7, 0.7, 0.7};
    //transform_.rotate.y += ;

}

void Orbiter::Draw()
{
    Model::SetTransForm(hModel_, &transform_);
    Model::Draw(hModel_);
}

void Orbiter::Release()
{
}
