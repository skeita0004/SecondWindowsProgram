#pragma once

#include <list>
#include <string>
#include "Transform.h"

class SphereCollider;

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
    Transform* GetTransform();

	template <typename T>
	inline T* FindGameObject(const std::string& _name)
	{
		return static_cast<T*>(GetRootJob()->FindChildObject(_name));
	}

    // 名前だけ渡して、型が何かを調べて、返すタイプのが必要

	void DeleteObjectSub(GameObject* _object);
	void DeleteAllChildren();
	
    void SetPosition(XMFLOAT3 _position);
    void SetPosition(float _x, float _y, float _z);

	const std::string& GetObjectName()
    {return this->objectName_;}

    void SetObjectName(const std::string& _name)
    {
        this->objectName_ = _name;
    }

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
	inline T* Instantiate(GameObject* _parent)
	{
		T* pObject{nullptr};
		pObject = new T(_parent);
		_parent->childList_.push_back(pObject);
        //pObject->GetTransform()->pParent = _parent->GetTransform();
		pObject->Init();
        return pObject;
	}

	void AddCollider(SphereCollider* _pSphereCollider);
	void Collision(GameObject* _pGameObject);
    void RoundRobinCollisionDetection(GameObject* _pGameObject);

    virtual void OnCollision(GameObject* _pTarget);

protected:
    Transform transform;

private:
	std::list<GameObject*> childList_;
	GameObject* pParent_;
	std::string objectName_;
	bool isKillMe_;
	SphereCollider* pSphereCollider_;
};
