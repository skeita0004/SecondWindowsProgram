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
		pObject->Init();
		_parent->childList_.push_back(pObject);
	}

private:
	std::list<GameObject*> childList_;
	Transform transForm_;
	GameObject* pParent_;
	std::string objectName_;
	bool isKillMe_;
};