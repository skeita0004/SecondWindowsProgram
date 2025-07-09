#include "Dice.h"

namespace
{
	const int FACE_NUM = 6;
}

Dice::Dice() :
	Dice(1.f, { 0.f, 0.f, 0.f, 0.f })
{
}

Dice::Dice(float _size, XMFLOAT4 _pos) :
	size_(_size),
	position_(_pos),
	quads_(FACE_NUM, nullptr)
{
}

Dice::~Dice()
{
}

void Dice::Initialize()
{
	VERTEX vertices[] =
	{
		// {{pos},{uv}}
		XMVectorSet(-position_.x + -size_,  position_.y +  size_,  position_.z + size_, 0.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 左上
		XMVectorSet( position_.y +  size_,  position_.y +  size_,  position_.z + size_, 0.0f), XMVectorSet(1.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 右上
		XMVectorSet( position_.y +  size_, -position_.y + -size_,  position_.z + size_, 0.0f), XMVectorSet(1.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 右下
		XMVectorSet(-position_.y + -size_, -position_.y + -size_,  position_.z + size_, 0.0f), XMVectorSet(0.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 左下
		XMVectorSet(-position_.y + -size_,  position_.y +  size_, -position_.z + -size_, 0.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 左上
		XMVectorSet( position_.y +  size_,  position_.y +  size_, -position_.z + -size_, 0.0f), XMVectorSet(1.0f, 0.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 右上
		XMVectorSet( position_.y +  size_, -position_.y + -size_, -position_.z + -size_, 0.0f), XMVectorSet(1.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 右下
		XMVectorSet(-position_.y + -size_, -position_.y + -size_, -position_.z + -size_, 0.0f), XMVectorSet(0.0f, 1.0f, 0.f, 0.f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), // 左下
	};

	VERTEX vertices[FACE_NUM][4] =
	{
		// front
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},

		// back
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},

		// right
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},

		// left
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},

		// top
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},

		// bottom
		{
			// top left
			
			// top right
			
			// bottom right
			
			// bottom left
		},
	};

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
