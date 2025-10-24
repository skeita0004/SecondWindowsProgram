#include "Transform.h"

namespace
{
    XMMATRIX matTranslate = XMMatrixIdentity();
    XMMATRIX matRotate    = XMMatrixIdentity();
    XMMATRIX matScale     = XMMatrixIdentity();
}

Transform::Transform() :
    //matTranslate_(XMMatrixIdentity()),
    //matRotate_(XMMatrixIdentity()),
    //matScale_(XMMatrixIdentity()),
    position({0, 0, 0}),
    rotate({0, 0, 0}),
    scale({1, 1, 1}),
    pParent(nullptr)
{
}

Transform::Transform(XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale) :
    //matTranslate_(XMMatrixIdentity()),
    //matRotate_(XMMatrixIdentity()),
    //matScale_(XMMatrixIdentity()),
    position(_pos),
    rotate(_rot),
    scale(_scale),
    pParent(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
    //matTranslate_ = XMMatrixTranslation(position.x, position.y, position.z);

    //XMMATRIX matRotateX{};
    //XMMATRIX matRotateY{};
    //XMMATRIX matRotateZ{};
    //matRotateX = XMMatrixRotationX(XMConvertToRadians(rotate.x));
    //matRotateY = XMMatrixRotationY(XMConvertToRadians(rotate.y));
    //matRotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate.z));
    //matRotate_ = matRotateZ * matRotateX * matRotateY;

    //matScale_ = XMMatrixScaling(scale.x, scale.y, scale.z);

    matTranslate = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX matRotateX{};
    XMMATRIX matRotateY{};
    XMMATRIX matRotateZ{};
    matRotateX = XMMatrixRotationX(XMConvertToRadians(rotate.x));
    matRotateY = XMMatrixRotationY(XMConvertToRadians(rotate.y));
    matRotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate.z));
    matRotate = matRotateZ * matRotateX * matRotateY;

    matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX const Transform::GetWorldMatrix()
{
    Calculation();

    if (pParent != nullptr)
    {
        //return (matScale_ * matRotate_ * matTranslate_) * pParent->GetWorldMatrix();
        return (matScale * matRotate * matTranslate) * pParent->GetWorldMatrix();
    }

    //return matScale_ * matRotate_ * matTranslate_;
    return matScale * matRotate * matTranslate;
}

const XMMATRIX Transform::GetNormalMatrix()
{
    Calculation();
    if (pParent != nullptr)
    {
        //return matRotate_ * XMMatrixInverse(nullptr, matScale_) * pParent->GetNormalMatrix();
        return matRotate * XMMatrixInverse(nullptr, matScale) * pParent->GetNormalMatrix();
    }

    //return matRotate_ * XMMatrixInverse(nullptr, matScale_);
    return matRotate * XMMatrixInverse(nullptr, matScale);
}
