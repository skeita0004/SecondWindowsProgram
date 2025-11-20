#pragma once

#include "GameObject.h"

class Orbiter;
class SceneManager;

class Player : public GameObject
{
public:
	Player(GameObject* _parent);
	~Player();

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Release() override;

private:
    void OnCollision(GameObject* _pTarget) override;

    static inline const XMFLOAT3 PLAYER_POSITION{0.f, -10.f, 0.f};
    static inline const XMFLOAT3 PLAYER_ROTATE{30.f, 0.f, 0.f};
    static inline const XMFLOAT3 PLAYER_SCALE{2.f, 2.f, 2.f};
    static inline const XMFLOAT3 ORBITER_LEFT_OFFSET{-3.0, 3.0, 0};
    static inline const XMFLOAT3 ORBITER_RIGHT_OFFSET{3.0, 3.0, 0};

	int       hModel_;
    Orbiter* pOrbiterLeft_;
    Orbiter* pOrbiterRight_;

    SceneManager* pSceneManager_;
};