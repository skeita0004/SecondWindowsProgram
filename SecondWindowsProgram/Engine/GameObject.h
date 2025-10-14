#pragma once

#include <list>
#include <string>
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* _pParent, const std::string& _name);
	virtual ~GameObject();

	virtual void Init()    = 0;
	virtual void Update()  = 0;
	virtual void Draw()    = 0;
	virtual void Release() = 0;

	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	void KillMe();

	GameObject* GetParent();
	GameObject* GetRootJob();
	std::list<GameObject*>* GetChildList();
	GameObject* FindChildObject(const std::string& _name);

	template <typename T>
	inline T* FindGameObject(const std::string& _name)
	{
		return static_cast<T*>(GetRootJob()->FindChildObject(_name));
	}

	void DeleteObjectSub(GameObject* _object);
	void DeleteAllChildren();
	
	inline const std::string& GetObjectName() {return this->objectName_;}

	template <typename PTYPE>
	inline static void SafeDelete(PTYPE* _ptr)
	{
		if (_ptr != nullptr)
		{
			delete _ptr;
			_ptr = nullptr;
		}
	}

	template <class T>
	inline void Instantiate(GameObject* _parent)
	{
		T* pObject{nullptr};
		pObject = new T(_parent);
		_parent->childList_.push_back(pObject);
		pObject->Init();
	}

	void AddCollider(SphereCollider* _pSphereCollider);

	void Collision(GameObject* _pGameObject);
private:
	std::list<GameObject*> childList_;
	Transform transForm_;
	GameObject* pParent_;
	std::string objectName_;
	bool isKillMe_;
	SphereCollider* pSphereCollider_;
};
