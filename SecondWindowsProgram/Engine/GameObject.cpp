#include "GameObject.h"
#include <Windows.h>

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

GameObject* GameObject::GetParent()
{
	return this->pParent_;
}

GameObject* GameObject::GetRootJob()
{
	if (GetParent() != nullptr)
	{
		return GetParent()->GetRootJob();
	}
	else if (GetParent() == nullptr)
	{
		return this;
	}

	return nullptr;
}

std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

GameObject* GameObject::FindChildObject(const std::string& _name)
{
	if (this->childList_.empty())
	{
		return nullptr;
	}

	auto itr    = this->childList_.begin();
	auto itrEnd = this->childList_.end();

	while (itr != itrEnd)
	{
		auto object = (*itr);

		if (object->GetObjectName() == _name)
		{
			return object;
		}
		
		GameObject* child = object->FindChildObject(_name);
		if (child != nullptr)
		{
			return child;
		}
		
		itr++;
	}

	return nullptr;
}

void GameObject::DeleteObjectSub(GameObject* _object)
{
	if (not(_object->childList_.empty()))
	{
		auto list   = _object->GetChildList();
		auto itr    = list->begin();
		auto itrEnd = list->end();
		auto object = (*itr);

		while (itr != itrEnd)
		{

			object = (*itr);
			DeleteObjectSub(object);
			SafeDelete(object);
			itr = list->erase(itr);

		}
		list->clear();
	}
	_object->Release();
}

void GameObject::DeleteAllChildren()
{
	if (this->childList_.empty())
	{
		return;
	}

	auto itr    = this->childList_.begin();
	auto itrEnd = this->childList_.end();
	auto object = (*itr);

	while (itr != itrEnd)
	{
		DeleteObjectSub(object);
		SafeDelete(object);
		itr = childList_.erase(itr);
	}

	childList_.clear();
}

void GameObject::AddCollider(SphereCollider* _pSphereCollider)
{
	pSphereCollider_ = _pSphereCollider;
}

void GameObject::Collision(GameObject* _pGameObject)
{
}
