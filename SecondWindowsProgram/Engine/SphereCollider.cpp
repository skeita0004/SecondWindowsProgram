#include "SphereCollider.h"

SphereCollider::SphereCollider(XMFLOAT3 _position, float _radius):
	transform_(),
	radius_(_radius)
{
	transform_.position = _position;
}

SphereCollider::~SphereCollider()
{
}
