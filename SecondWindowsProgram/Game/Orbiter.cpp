#include "Orbiter.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"
#include "Player.h"

namespace
{
    const std::string MODEL_PATH{ "Assets/models/mochi_kinchaku.fbx" };
}

Orbiter::Orbiter(GameObject* _parent) :
    GameObject(_parent, "Orbiter"),
    hModel_(-1),
    pSphereCollider_(nullptr),
    posOffset_(),
    pPlayer_(nullptr)
{
}

Orbiter::~Orbiter()
{
}

void Orbiter::Init()
{
    pPlayer_ = static_cast<Player*>(GetParent());
    hModel_ = Model::Load(MODEL_PATH);
    transform.scale = {0.7, 0.7, 0.7};
}

void Orbiter::Update()
{
    transform.position.y = 5.f;
    XMVECTOR vParentPos = XMLoadFloat3(&pPlayer_->GetTransform()->position);
    XMVECTOR vPos       = XMLoadFloat3(&transform.position);
    XMVECTOR vOffset    = XMLoadFloat3(&posOffset_);

    vPos = vParentPos + vOffset;

    XMStoreFloat3(&transform.position, vPos);

    transform.rotate.y++;
}

void Orbiter::Draw()
{
    Model::SetTransForm(hModel_, &transform);
    Model::Draw(hModel_);
}

void Orbiter::Release()
{
}
