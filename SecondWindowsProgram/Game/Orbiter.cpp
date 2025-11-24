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
    hModel_(-1)
{
}

Orbiter::~Orbiter()
{
}

void Orbiter::Init()
{
    hModel_ = Model::Load(MODEL_PATH);
    transform.scale = {0.7, 0.7, 0.7};
}

void Orbiter::Update()
{
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
