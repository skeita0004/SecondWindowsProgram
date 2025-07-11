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
		{XMVectorSet(-position_.x + -size_,  position_.y + size_,  position_.z + size_, 0.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f)}, // 左上
		{XMVectorSet(position_.y + size_,  position_.y + size_,  position_.z + size_, 0.0f), XMVectorSet(1.0f, 0.0f, 0.f, 0.f)}, // 右上
		{XMVectorSet(position_.y + size_, -position_.y + -size_,  position_.z + size_, 0.0f), XMVectorSet(1.0f, 1.0f, 0.f, 0.f)}, // 右下
		{XMVectorSet(-position_.y + -size_, -position_.y + -size_,  position_.z + size_, 0.0f), XMVectorSet(0.0f, 1.0f, 0.f, 0.f)}, // 左下
		{XMVectorSet(-position_.y + -size_,  position_.y + size_, -position_.z + -size_, 0.0f), XMVectorSet(0.0f, 0.0f, 0.f, 0.f)}, // 左上
		{XMVectorSet(position_.y + size_,  position_.y + size_, -position_.z + -size_, 0.0f), XMVectorSet(1.0f, 0.0f, 0.f, 0.f)}, // 右上
		{XMVectorSet(position_.y + size_, -position_.y + -size_, -position_.z + -size_, 0.0f), XMVectorSet(1.0f, 1.0f, 0.f, 0.f)}, // 右下
		{XMVectorSet(-position_.y + -size_, -position_.y + -size_, -position_.z + -size_, 0.0f), XMVectorSet(0.0f, 1.0f, 0.f, 0.f)}, // 左下
	};

	quadFace_.push_back(
		// front
		{
			// top left
			vertices[LEFT_TOP_FRONT],

			// top right
			vertices[RIGHT_TOP_FRONT],

			// bottom right
			vertices[RIGHT_BOTTOM_FRONT],

			// bottom left
			vertices[LEFT_BOTTOM_FRONT],
		});

	quadFace_.push_back(
		// back
		{
			// top left
			vertices[LEFT_TOP_BACK],

			// top right
			vertices[RIGHT_TOP_BACK],

			// bottom right
			vertices[RIGHT_BOTTOM_BACK],

			// bottom left
			vertices[LEFT_BOTTOM_BACK],

		});

	quadFace_.push_back(
		// right
		{
			// top left
			vertices[RIGHT_TOP_FRONT],

			// top right
			vertices[RIGHT_TOP_FRONT],

			// bottom right
			vertices[RIGHT_TOP_BACK],

			// bottom left
			vertices[RIGHT_TOP_BACK],

		});

	quadFace_.push_back(
		// left
		{
			// top left
			vertices[LEFT_TOP_FRONT],

			// top right
			vertices[RIGHT_TOP_FRONT],

			// bottom right
			vertices[RIGHT_TOP_FRONT],

			// bottom left
			vertices[LEFT_TOP_FRONT],

		});

	quadFace_.push_back(
		// top
		{
			// top left
			vertices[LEFT_TOP_BACK],

			// top right
			vertices[RIGHT_TOP_BACK],

			// bottom right
			vertices[RIGHT_TOP_FRONT],

			// bottom left
			vertices[LEFT_TOP_FRONT],

		});

	quadFace_.push_back(
		// bottom
		{
			// top left
			vertices[LEFT_BOTTOM_BACK],

			// top right
			vertices[RIGHT_BOTTOM_BACK],

			// bottom right
			vertices[RIGHT_BOTTOM_BACK],

			// bottom left
			vertices[LEFT_BOTTOM_BACK],
		});

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
