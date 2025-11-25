#pragma once

#include "GameObject.h"
#include "SceneManager.h"

class Player;

class Enemy : public GameObject
{
public:
	Enemy(GameObject* _parent);
	~Enemy();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
    void OnCollision(GameObject* _pTarget) override;
	int           hModel_;
    SceneManager* pSceneManager_;
    Player*       pPlayer_;
};