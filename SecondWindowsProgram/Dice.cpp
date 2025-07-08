#include "Dice.h"

namespace
{
	const int FACE_NUM = 6;
}

Dice::Dice() :
	Dice(1.f)
{

}

Dice::Dice(float _size) :
	size_(_size),
	quads_(FACE_NUM, nullptr)
{
}

Dice::~Dice()
{
}

void Dice::Initialize()
{
	for (auto& quad : quads_)
	{
		quad = new Quad();
	}
}

void Dice::Draw()
{

}

void Dice::Release()
{

}
