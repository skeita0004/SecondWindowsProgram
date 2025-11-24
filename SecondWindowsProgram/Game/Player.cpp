#include "Player.h"
#include <string>
#include "Input.h"
#include "Bullet.h"
#include "Model.h"
#include "Orbiter.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "SceneManager.h"

namespace
{
    // ﾌｧｲﾙから読むようにしないとクビだぞ！
	const std::string MODEL_PATH{"Assets/models/odenwithbone.fbx"};
}

Player::Player(GameObject* _parent) :
    GameObject(_parent, "Player"),
	hModel_(-1),
    pOrbiterLeft_(nullptr),
    pOrbiterRight_(nullptr),
    pSceneManager_(nullptr)
{
}

Player::~Player()
{
}

void Player::Init()
{
    transform.position = PLAYER_POSITION;
    transform.scale    = PLAYER_SCALE;
	SphereCollider* pSphereCollider_ = new SphereCollider(15.0f);
    AddCollider(pSphereCollider_);
	hModel_ = Model::Load(MODEL_PATH);
    pOrbiterLeft_ = Instantiate<Orbiter>(this);
    pOrbiterRight_ = Instantiate<Orbiter>(this);
   
    pOrbiterLeft_->SetPosition(-3.f, 3.f, 0.f);
    pOrbiterRight_->SetPosition(3.f, 3.f, 0.f);

    pSceneManager_ = FindGameObject<SceneManager>("SceneManager");
} 

void Player::Update()
{
    transform.rotate.y = 0;
    transform.rotate.z = 0;

    if (Input::IsKeyDown(DIK_SPACE))
    {
        Bullet* pBullet = Instantiate<Bullet>(this->GetParent());
        pBullet->SetObjectName("PlayerBullet");
        pBullet->SetOwner(this);

        XMFLOAT3 bulletPos{};
        XMVECTOR vBulletPos = XMLoadFloat3(&transform.position);
        vBulletPos = XMVectorAdd(vBulletPos, XMVectorSet(0.f, 10.f, 1.5f, 0.f));
        XMStoreFloat3(&bulletPos, vBulletPos);
        pBullet->SetPosition(bulletPos);
        pBullet->SetVelocity({0, 0.0f, 1.5f});
        // おでんの先端から発射できるようにしたい
        // ボーンの位置をとって、そこから発射できるように
        // ↑ FBXSDKの深いところで例外発生するせいでできず。
	}
    
    // 移動範囲の制限
    if (transform.position.x > -18.f)
    {
        // 移動処理
        if (Input::IsKey(DIK_A))
        {
            transform.position.x -= 0.2f;
            transform.rotate.z = 15;
        }
    }
    if (transform.position.x < 18.f)
    {
        if (Input::IsKey(DIK_D))
        {
            transform.position.x += 0.2f;
            transform.rotate.z = -15;
        }
    }
}

void Player::Draw()
{
    Model::SetTransForm(hModel_, &transform);
	Model::Draw(hModel_);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "Enemy")
    {
        _pTarget->KillMe();
    }
    if (_pTarget->GetObjectName() == "EnemyBullet")
    {
        KillMe();
        pSceneManager_->ChangeScene(SceneManager::SceneID::SID_OVER);
    }
}
