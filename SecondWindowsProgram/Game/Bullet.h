#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(GameObject* _parent);
	~Bullet();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

    void SetVelocity(const XMFLOAT3& _velocity)
    {
        velocity_ = _velocity;
    }

private:
    void OnCollision(GameObject* _pTarget) override;

	int hModel_;
    XMFLOAT3 velocity_;
    bool isHit_;
    std::string targetName_;
};