#include "Enemy.hpp"
#include "Model.hpp"
#include <imgui.h>
#include "Player.hpp"
#include <algorithm>

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
    isAlive_ = true;

    pPlayer_ = static_cast<Player*>(FindObject<Player>("Player"));
}

void Enemy::Update()
{
    switch (state_)
    {
        case Enemy::IDLE:
            UpdateIdle();
            break;
        case Enemy::CHASE:
            UpdateChase();
            break;
        case Enemy::ATTACK:
            UpdateAttack();
            break;
        case Enemy::DEAD:
            UpdateDead();
            break;
        default:
            break;
    }

    UpdateCommon();

#ifdef _DEBUG
    ImGui::Begin("Enemy Info");
    ImGui::Value("HP", hp_);
    ImGui::InputInt("AnimFrame", &currAnimFrame_);
    ImGui::End();
#endif
}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{
    if (true) // プレイヤーが視界内に入ったら
    {
        state_ = CHASE;
    }
}

void Enemy::UpdateChase()
{
    if (true) // プレイヤーが視界内から消えたら
    {
        state_ = IDLE;
    }
}

void Enemy::UpdateAttack()
{
    if (true) // プレイヤーが射程距離から離れたら
    {
        state_ = CHASE;
    }
}

void Enemy::UpdateDead()
{
    if (currAnimFrame_ == (313 + 180))
    {
        DeleteMe();
    }
}

void Enemy::UpdateCommon()
{
    currAnimFrame_ = Model::GetAnimFrame(hModel_);

    // 常にプレイヤーの方向を向く処理(最終的には、視界内に一度入った後に追従する形にする)
    if (pPlayer_ != nullptr and isAlive_)
    {
        RotateToPlayer();
    }

    if (currAnimFrame_ == 224)
    {
        SetAnimation(EA_IDLE);
    }

    if (hp_ < 0 && currAnimFrame_ == 224)
    {
        isAlive_ = false;
        SetAnimation(EA_DEAD);
        state_ = DEAD;
        DeleteCollider();
    }
}

void Enemy::RotateToPlayer()
{
    XMVECTOR vPlayerPos{ XMLoadFloat3(&pPlayer_->GetTransform()->position) };
    XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

    XMMATRIX rotYMat = XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));
    XMVECTOR vForward{ XMVectorSet(0, 0, 1.0f, 0) };

    vForward = XMVector3TransformNormal(vForward, rotYMat);
    vForward = XMVector3Normalize(vForward);

    XMVECTOR toPlayerDir{ vPlayerPos - vPos };
    toPlayerDir = XMVector3Normalize(toPlayerDir);

    float diffRad = XMVectorGetX(XMVector3Dot(vForward, toPlayerDir));

    diffRad = std::clamp(diffRad, -1.0f, 1.0f);
    diffRad = acosf(diffRad);

    float sign = XMVectorGetY(XMVector3Cross(vForward, toPlayerDir));
    if (sign >= 0.0f)
    {
        sign = 1.0f;
    }
    else
    {
        sign = -1.0f;
    }

    transform.rotate.y += XMConvertToDegrees(diffRad * sign);
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

