#include "Transform.h"

Transform::Transform() :
    matTranslate_(),
    matRotate_(),
    matScale_(),
    position(),
    rotate(),
    scale({2, 2, 2})
{
}

Transform::Transform(XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale) :
    matTranslate_(XMMatrixIdentity()),
    matRotate_(XMMatrixIdentity()),
    matScale_(XMMatrixIdentity()),
    position(_pos),
    rotate(_rot),
    scale(_scale)
{
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
    /*
    XMVECTOR scale = XMLoadFloat3(&scale_);
    XMVECTOR rotate = XMLoadFloat3(&rotate_);
    XMVECTOR translate = XMLoadFloat3(&position_);

    scale = XMVector3TransformCoord(scale, matScale_);
    rotate = XMVector3TransformCoord(rotate, matRotate_);
    translate = XMVector3TransformCoord(translate, matTranslate_);

    XMStoreFloat3(&scale_, scale);
    XMStoreFloat3(&scale_, rotate);
    XMStoreFloat3(&scale_, translate);
    */

    matTranslate_ = XMMatrixTranslation(position.x, position.y, position.z);

    XMMATRIX matRotateX{};
    XMMATRIX matRotateY{};
    XMMATRIX matRotateZ{};
    matRotateX = XMMatrixRotationX(XMConvertToRadians(rotate.x));
    matRotateY = XMMatrixRotationY(XMConvertToRadians(rotate.y));
    matRotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate.z));
    matRotate_ = matRotateZ * matRotateX * matRotateY;

    matScale_ = XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX const Transform::GetWorldMatrix()
{
    Calculation();
    return matScale_ * matRotate_ * matTranslate_;
}

const XMMATRIX Transform::GetNormalMatrix()
{
    Calculation();
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
