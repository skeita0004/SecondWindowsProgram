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
	Dice();
	Dice(float _size, XMFLOAT4 _pos);
	~Dice();

	void Initialize();
	void Draw(DirectX::XMMATRIX _worldMat);
	void Release();

private:
	float size_;
	XMFLOAT4 position_;
	std::vector<Quad*> quads_;
	//std::vector<VERTEX> faces_;
	std::vector<QuadFace> quadFace_;
};