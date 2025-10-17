#pragma once

#include "Transform.h"

class SphereCollider
{
public:
	SphereCollider(XMFLOAT3 _position, float _radius);
	~SphereCollider();

	float GetRadius()
	{
		return radius_;
	}

private:
	Transform transform_;
	float radius_;
};