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
    SetAnimation(EA_IDLE);
    hp_ = 100;
}

void Enemy::Update()
{
    // 

    Model::SetTransform(hModel_, transform);
    int currAnimFrame = Model::GetAnimFrame(hModel_);

    if (currAnimFrame == 224)
    {
        SetAnimation(EA_IDLE);
    }

    if (hp_ < 0 && currAnimFrame == 224)
    {
        SetAnimation(EA_DEAD);
        DeleteCollider();
    }

    if (currAnimFrame == (313 + 180))
    {
        DeleteMe();
    }

    ImGui::Begin("Enemy Info");
    ImGui::Value("HP", hp_);
    ImGui::InputInt("AnimFrame", &currAnimFrame);
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
            SetAnimation(EA_HIT);
        }
        hp_ -= 1;
    }
}

void Enemy::SetAnimation(EnemyAnimation _enemyAnimation)
{
    eaState_ = _enemyAnimation;

    // アニメーションのフレーム数がハードコーディングされているので、
    // csvから読み込むようにすること。
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
            Model::SetAnimFrame(hModel_, 225, 313 + 180, 1.0f);
            break;

        case Enemy::MAX_ANIMATION:
            break;
        default:
            break;
    }
}

