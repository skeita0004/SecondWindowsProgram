#pragma once
#include "Quad.h"
#include <vector>

class Dice
{
public:
	Dice();
	Dice(float _size);
	~Dice();

	void Initialize();
	void Draw();
	void Release();

private:
	float size_;
	std::vector<Quad*> quads_;
};