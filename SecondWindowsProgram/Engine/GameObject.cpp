#include "GameObject.h"
#include <Windows.h>

GameObject::GameObject() :
	childList_(0, nullptr),
	transform_(),
	pParent_(nullptr),
	objectName_("Unknown Object"),
	isKillMe_(false),
	pSphereCollider_(nullptr)
{
}

GameObject::GameObject(GameObject* _pParent, const std::string& _name) :
	childList_(0, nullptr),
	transform_(),
	pParent_(_pParent),
	objectName_(_name),
	isKillMe_(false),
	pSphereCollider_(nullptr)
{
    if (_pParent != nullptr)
    {
        transform_.pParent = &_pParent->transform_;
    }
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

    for (auto object : childList_)
    {
        Collision(GetRootJob());
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

void GameObject::SetPosition(XMFLOAT3 _position)
{
    transform_.position = _position;
}

void GameObject::SetPosition(float _x, float _y, float _z)
{
    transform_.position = {_x, _y, _z};
}

void GameObject::AddCollider(SphereCollider* _pSphereCollider)
{
	pSphereCollider_ = _pSphereCollider;
}

void GameObject::Collision(GameObject* _pGameObject)
{
    if (_pGameObject->pSphereCollider_ == nullptr)
    {
        return;
    }

	if (_pGameObject->GetChildList()->empty())
	{
		return;
	}

#pragma region CollisionDetection

	float x1 = this->transform_.position.x;
	float x2 = _pGameObject->transform_.position.x;

	float y1 = this->transform_.position.y;
	float y2 = _pGameObject->transform_.position.y;

	float z1 = this->transform_.position.z;
	float z2 = _pGameObject->transform_.position.z;

	float r1 = this->pSphereCollider_->GetRadius();
	float r2 = _pGameObject->pSphereCollider_->GetRadius();

	float dx = (x2 - x1) * (x2 - x1);
	float dy = (y2 - y1) * (y2 - y1);
	float dz = (z2 - z1) * (z2 - z1);

	float rSumPow = (r1 + r2) * (r1 + r2);

	bool isHit = dx + dy + dz <= rSumPow;

	if (isHit)
	{

	}
#pragma endregion

	auto itr = _pGameObject->GetChildList()->begin();
	auto itrEnd = _pGameObject->GetChildList()->end();
	auto object = (*itr);

	while (itr != itrEnd)
	{
		Collision(object);
	}
}
