#include "Transform.h"

Transform::Transform() :
    position({0, 0, 0}),
    rotate({0, 0, 0}),
    scale({1, 1, 1}),
    pParent(nullptr)
{
}

Transform::Transform(XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale) :
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
        return (matScale * matRotate * matTranslate) * pParent->GetWorldMatrix();
    }

    return matScale * matRotate * matTranslate;
}

const XMMATRIX Transform::GetNormalMatrix()
{
    Calculation();
    if (pParent != nullptr)
    {
        return matRotate * XMMatrixInverse(nullptr, matScale) * pParent->GetNormalMatrix();
    }

    return matRotate * XMMatrixInverse(nullptr, matScale);
}

const XMMATRIX Transform::GetTranslateMatrix()
{
    Calculation();
    if (pParent)
    {
        return matTranslate * pParent->GetTranslateMatrix();
    }

    return matTranslate;
}

const XMMATRIX Transform::GetRotateMatrix()
{
    Calculation();
    if (pParent)
    {
        return matRotate * pParent->GetRotateMatrix();
    }
    return matRotate;
}

const XMMATRIX Transform::GetScaleMatrix()
{
    Calculation();
    if (pParent)
    {
        return matScale * pParent->GetScaleMatrix();
    }
    return matScale;
}
