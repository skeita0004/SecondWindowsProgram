#pragma once

#include "Transform.h"

class SphereCollider
{
public:
	SphereCollider(float _radius) : radius_(_radius) { }
	~SphereCollider();

	float GetRadius()
	{
		return radius_;
	}

private:
	float radius_;
};