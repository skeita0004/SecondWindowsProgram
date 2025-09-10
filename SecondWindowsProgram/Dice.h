#pragma once
#include "Quad.h"
#include <vector>

enum Vertex
{
	LEFT_TOP_FRONT,
	RIGHT_TOP_FRONT,
	RIGHT_BOTTOM_FRONT,
	LEFT_BOTTOM_FRONT,
	LEFT_TOP_BACK,
	RIGHT_TOP_BACK,
	RIGHT_BOTTOM_BACK,
	LEFT_BOTTOM_BACK,
};

class Dice
{
public:
	Dice(Transform _transform);
	~Dice();

	void Initialize();
	void Draw(Transform& _transform);
	void Release();

	void SetTransform(Transform& _transform)
	{
		transform_ = _transform;
	}

	Transform GetTransform()
	{
		return transform_;
	}

private:
	Transform transform_;
	std::vector<Quad*> quads_;
	//std::vector<VERTEX> faces_;
	std::vector<QuadFace> quadFace_;
};