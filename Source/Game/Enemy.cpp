#include "Enemy.hpp"
#include "Model.hpp"
#include <imgui.h>

Enemy::Enemy(GameObject* _pParent) :
    GameObject(_pParent, "Enemy"),
    hModel_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
    pBoxCollider_ = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 9, 1));
    AddCollider(pBoxCollider_);
    hModel_ = Model::Load("Models/Enemy/ps_enemy.fbx");
    transform.position = {0.f, 0.f, 0.f};
    Model::SetAnimFrame(hModel_, 1, 60, 1.0f);
    hp_ = 100;
}

void Enemy::Update()
{
    Model::SetTransform(hModel_, transform);
    int currAnimFrame = Model::GetAnimFrame(hModel_);

    if (currAnimFrame == 224)
    {
        ChangeAnimation(EA_IDLE);
    }

    if (hp_ < 0)
    {
        ChangeAnimation(EA_DEAD);
    }

    if (currAnimFrame == 313)
    {
        DeleteMe();
    }

    ImGui::Begin("Enemy Info");
    ImGui::Value("HP", hp_);
    ImGui::End();
}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "Bullet")
    {
        int currAnimFrame{ Model::GetAnimFrame(hModel_) };
        if ((eaState_ != EA_HIT and eaState_ != EA_DEAD))
        {
            ChangeAnimation(EA_HIT);
        }
        hp_ -= 1;
    }
}

void Enemy::ChangeAnimation(EnemyAnimation _enemyAnimation)
{
    eaState_ = _enemyAnimation;
    switch (_enemyAnimation)
    {
        case Enemy::EA_IDLE:
            Model::SetAnimFrame(hModel_, 1, 60, 1.0f);
            break;

        case Enemy::EA_WALK:
            Model::SetAnimFrame(hModel_, 61, 94, 1.0f);
            break;

        case Enemy::EA_RUN:
            Model::SetAnimFrame(hModel_, 95, 110, 1.0f);
            break;

        case Enemy::EA_ATTACK:
            Model::SetAnimFrame(hModel_, 111, 189, 1.0f);
            break;

        case Enemy::EA_HIT:
            Model::SetAnimFrame(hModel_, 190, 224, 1.0f);
            break;

        case Enemy::EA_DEAD:
            Model::SetAnimFrame(hModel_, 225, 313, 1.0f);
            break;

        case Enemy::MAX_ANIMATION:
            break;
        default:
            break;
    }
}

