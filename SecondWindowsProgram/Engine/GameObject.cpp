#include "GameObject.h"

GameObject::GameObject() :
	childList_(0, nullptr),
	transForm_(),
	pParent_(nullptr),
	objectName_("Unknown Object"),
	isKillMe_(false)
{
}

GameObject::GameObject(GameObject* _pParent, const std::string& _name) :
	childList_(0, nullptr),
	transForm_(),
	pParent_(_pParent),
	objectName_(_name),
	isKillMe_(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
	
	for (auto& object : childList_)
	{
		object->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		auto object = *itr;

		if (object->isKillMe_)
		{
			object->ReleaseSub();
			SafeDelete(object);
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();

	for (auto& object : this->childList_)
	{
		object->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	for (auto& object : this->childList_)
	{
		object->ReleaseSub();
	}

	Release(); // 自分を先にリリースすると破滅するので、最後に呼ぶ。
}

void GameObject::KillMe()
{
	isKillMe_ = true;
}
