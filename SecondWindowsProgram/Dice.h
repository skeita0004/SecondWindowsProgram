#pragma once
#include "Quad.h"
#include <vector>

class Dice
{
public:
	Dice();
	Dice(float _size, XMFLOAT4 _pos);
	~Dice();

	void Initialize();
	void Draw();
	void Release();

private:
	float size_;
	XMFLOAT4 position_;
	std::vector<Quad*> quads_;
};