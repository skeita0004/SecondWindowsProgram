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
	quads_(FACE_NUM, nullptr),
	quadFace_()
{
	quadFace_.clear();
}

Dice::~Dice()
{
}

void Dice::Initialize()
{
	VERTEX vertices[] =
	{
		// {{pos},{uv}}
		{XMVectorSet(position_.x + -size_, position_.y + size_,  position_.z + -size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7, 0.0f)}, // 左上
		{XMVectorSet(position_.x + size_, position_.y + size_,  position_.z + -size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7, 0.0f)}, // 右上
		{XMVectorSet(position_.x + size_, position_.y + -size_, position_.z + -size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7, 0.0f)}, // 右下
		{XMVectorSet(position_.x + -size_, position_.y + -size_, position_.z + -size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7, 0.0f)}, // 左下

		{XMVectorSet(position_.x + -size_, position_.y + size_, position_.z + size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7f, 0.0f)}, // 左上
		{XMVectorSet(position_.x + size_, position_.y + size_, position_.z + size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7f, 0.0f)}, // 右上
		{XMVectorSet(position_.x + size_, position_.y + -size_, position_.z + size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7f, 0.0f)}, // 右下
		{XMVectorSet(position_.x + -size_, position_.y + -size_, position_.z + size_, 1.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f), XMVectorSet(-1, 0.5, -0.7f, 0.0f)}, // 左下
	};

	// front
	QuadFace front
	{
		// top left
		vertices[LEFT_TOP_FRONT],

		// top right
		vertices[RIGHT_TOP_FRONT],

		// bottom right
		vertices[RIGHT_BOTTOM_FRONT],

		// bottom left
		vertices[LEFT_BOTTOM_FRONT],
	};


	QuadFace back
	{
		// top left
		vertices[LEFT_BOTTOM_BACK],

		// top right
		vertices[RIGHT_BOTTOM_BACK],

		// bottom right
		vertices[RIGHT_TOP_BACK],

		// bottom left
		vertices[LEFT_TOP_BACK],
	};

	QuadFace right
	{
		// top left
		vertices[RIGHT_TOP_FRONT],

		// top right
		vertices[RIGHT_TOP_BACK],

		// bottom right
		vertices[RIGHT_BOTTOM_BACK],

		// bottom left
		vertices[RIGHT_BOTTOM_FRONT],
	};

	QuadFace left
	{
		// top left
		vertices[LEFT_TOP_BACK],

		// top right
		vertices[LEFT_TOP_FRONT],

		// bottom right
		vertices[LEFT_BOTTOM_FRONT],

		// bottom left
		vertices[LEFT_BOTTOM_BACK],

	};

	QuadFace top
	{
		// top left
		vertices[LEFT_TOP_BACK],

		// top right
		vertices[RIGHT_TOP_BACK],

		// bottom right
		vertices[RIGHT_TOP_FRONT],

		// bottom left
		vertices[LEFT_TOP_FRONT],

	};

	QuadFace bottom
	{
		// top left
		vertices[LEFT_BOTTOM_FRONT],

		// top right
		vertices[RIGHT_BOTTOM_FRONT],

		// bottom right
		vertices[RIGHT_BOTTOM_BACK],

		// bottom left
		vertices[LEFT_BOTTOM_BACK],
	};

	front.topLeft.uv = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	front.topRight.uv = XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f);
	front.bottomRight.uv = XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f);
	front.bottomLeft.uv = XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f);

	back.topLeft.uv = XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f);
	back.topRight.uv = XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);
	back.bottomRight.uv = XMVectorSet(0.5f, 1.0f, 0.0f, 0.0f);
	back.bottomLeft.uv = XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f);

	right.topLeft.uv = XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f);
	right.topRight.uv = XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f);
	right.bottomRight.uv = XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f);
	right.bottomLeft.uv = XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);

	left.topLeft.uv = XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f);
	left.topRight.uv = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	left.bottomRight.uv = XMVectorSet(1.0f, 0.5f, 0.0f, 0.0f);
	left.bottomLeft.uv = XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f);

	top.topLeft.uv = XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f);
	top.topRight.uv = XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f);
	top.bottomRight.uv = XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);
	top.bottomLeft.uv = XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f);

	bottom.topLeft.uv = XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f);
	bottom.topRight.uv = XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f);
	bottom.bottomRight.uv = XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f);
	bottom.bottomLeft.uv = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	quadFace_.push_back(front);

	quadFace_.push_back(back);

	quadFace_.push_back(right);

	quadFace_.push_back(left);

	quadFace_.push_back(top);

	quadFace_.push_back(bottom);


	for (int i = 0; i < FACE_NUM; i++)
	{
		quads_[i] = new Quad(position_, size_, quadFace_[i]);
		quads_[i]->Initialize();
	}
}

void Dice::Draw(DirectX::XMMATRIX _worldMat)
{
	for (auto& quad : quads_)
	{
		quad->Draw(_worldMat);
	}
}

void Dice::Release()
{

}
