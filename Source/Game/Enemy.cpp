#define _CRT_SECURE_NO_WARNINGS

#include "Enemy.hpp"
#include "Model.hpp"
#include <imgui.h>
#include "Player.hpp"
#include <algorithm>
#include "Stage.hpp"
#include "Bullet.hpp"

Enemy::Enemy(GameObject* _pParent) :
    GameObject(_pParent, "Enemy"),
    hModel_(-1)
{
}

Enemy::~Enemy()
{
    instanceCount_ -= 1;
}

void Enemy::Init()
{
    pBoxCollider_ = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 9, 1));
    AddCollider(pBoxCollider_);
    hModel_ = Model::Load("Models/Enemy/ps_enemy.fbx");
    transform.position = {0.f, 0.f, 0.f};
    const float SCALE{1.5f};
    transform.scale = {SCALE, SCALE, SCALE};
    state_ = IDLE;
    SetAnimation(EA_IDLE);
    hp_ = 100;
    isAlive_ = true;

    pPlayer_ = static_cast<Player*>(FindObject<Player>("Player"));
    pStage_  = static_cast<Stage*>(FindObject<Stage>("Stage"));
    hStage_  = pStage_->GetModelHandle();

    instanceCount_ += 1;
}

void Enemy::Update()
{
    char buf[64]{};
    sprintf(buf, "%d", instanceCount_);
    OutputDebugStringA(buf);

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
    ImGui::Value("eaState", (uint32_t)eaState_);
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
    XMVECTOR vPlayerPos{XMLoadFloat3(&pPlayer_->GetTransform()->position)};
    XMVECTOR vPos{XMLoadFloat3(&transform.position)};

    vPlayerPos = XMVectorSetY(vPlayerPos, 0);
    vPos = XMVectorSetY(vPos, 0);

    XMVECTOR vDir{vPlayerPos - vPos};

    float toPlayerDist{XMVectorGetX(XMVector3Length(vDir))};

    if (toPlayerDist <= 10.f) // プレイヤーが視界内に入ったら
    {
        state_ = CHASE;
        SetAnimation(EA_RUN);
    }
}

void Enemy::UpdateChase()
{
    XMVECTOR vPlayerPos{XMLoadFloat3(&pPlayer_->GetTransform()->position)};
    XMVECTOR vPos{XMLoadFloat3(&transform.position)};

    vPlayerPos = XMVectorSetY(vPlayerPos, 0);
    vPos = XMVectorSetY(vPos, 0);

    XMVECTOR vDir{vPlayerPos - vPos};

    float toPlayerDist{XMVectorGetX(XMVector3Length(vDir))};

    if (toPlayerDist <= 6.5f and toPlayerDist >= 6.0f)
    {
        state_ = ATTACK;
    }

    if (eaState_ != EA_RUN)
    {
        SetAnimation(EA_RUN);
    }

    vDir = XMVector3Normalize(vDir);

    if (eaState_ != EA_HIT)
    {
        vPos = vPos + vDir * SPEED;
    }

    RayCastData toWall{};
    XMFLOAT3 start{};
    XMFLOAT3 dir{};
    XMStoreFloat3(&start, vPos);
    XMStoreFloat3(&dir,   vDir);
    toWall.start = start;
    toWall.dir   = dir;

    // RayCastせよ
    Model::RayCast(hStage_, &toWall);

    if (toWall.dist <= 6.0f)
    {
        vPos -= vDir * SPEED;
    }

    XMStoreFloat3(&transform.position, vPos);


}

void Enemy::UpdateAttack()
{
    attackInterval_++;

    XMVECTOR vPlayerPos{ XMLoadFloat3(&pPlayer_->GetTransform()->position) };
    XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

    vPlayerPos = XMVectorSetY(vPlayerPos, 0);
    vPos = XMVectorSetY(vPos, 0);

    XMVECTOR vDir{ vPlayerPos - vPos };

    float toPlayerDist{ XMVectorGetX(XMVector3Length(vDir)) };

    if (toPlayerDist >= 20.0f) // プレイヤーが射程距離から離れたら
    {
        //SetAnimation(EA_RUN);
        state_ = CHASE;
    }

    if (attackInterval_ == ATTACK_INTERVAL_TIME)
    {
        attackInterval_ = 0;
        SetAnimation(EA_ATTACK);
        Bullet* pBullet = static_cast<Bullet*>(Instantiate<Bullet>(GetParent()));
        pBullet->GetTransform()->position = XMFLOAT3(transform.position.x,
                                                     transform.position.y + 3.0f,
                                                     transform.position.z);

        pBullet->SetObjectName("EnemyBullet");
        pBullet->SetDir(vDir);
        pBullet->SetSpeed(0.03f);
    }

    if (currAnimFrame_ == 189)
    {
        SetAnimation(EA_IDLE);
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

    if (hp_ < 0 && eaState_ != EA_DEAD)
    {
        isAlive_ = false;
        SetAnimation(EA_DEAD);
        state_ = DEAD;
        DeleteCollider();
    }
}

void Enemy::RotateToPlayer()
{
    // 双方の位置をXMVECTORにする
    XMVECTOR vPlayerPos{ XMLoadFloat3(&pPlayer_->GetTransform()->position) };
    XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

    // 方向の計算時、Yは考慮しない
    vPlayerPos = XMVectorSetY(vPlayerPos, 0.0f);
    vPos = XMVectorSetY(vPos, 0.0f);

    // 正面ベクトルを求める
    XMMATRIX rotYMat = XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));
    XMVECTOR vForward{ XMVectorSet(0, 0, 1.0f, 0) };
    vForward = XMVector3TransformNormal(vForward, rotYMat);
    vForward = XMVector3Normalize(vForward);

    // プレイヤーへの方向
    XMVECTOR toPlayerDir{ vPlayerPos - vPos };
    toPlayerDir = XMVector3Normalize(toPlayerDir);

    // プレイヤーの方を向くために必要な回転角を算出
    float diffRad = XMVectorGetX(XMVector3Dot(vForward, toPlayerDir));
    diffRad = std::clamp(diffRad, -1.0f, 1.0f);
    diffRad = acosf(diffRad);
    
    // 回転方向の指定
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
        if (state_ != CHASE && state_ != ATTACK)
        {
            state_ = CHASE;
        }
        hp_ -= 1;
    }

    if (_pTarget->GetObjectName() == "Enemy") // 相手のコライダーの種類を取得できるようにする)
    {
        XMFLOAT3 targetPos = _pTarget->GetTransform()->position;
        XMFLOAT3 targetSize = _pTarget->GetBoxColliderSize();
        XMFLOAT3 size = GetBoxColliderSize();

        XMVECTOR vTargetPos{ XMLoadFloat3(&targetPos) };
        XMVECTOR vPos{ XMLoadFloat3(&transform.position) };
        XMVECTOR vTargetSize{ XMLoadFloat3(&targetSize) };
        XMVECTOR vSize{ XMLoadFloat3(&size) };

        vTargetPos = XMVectorSetY(vTargetPos, 0.f);
        vPos = XMVectorSetY(vPos, 0.f);

        XMVECTOR vDelta = vPos - vTargetPos;
        XMFLOAT3 delta{};
        XMStoreFloat3(&delta, vDelta);

        XMFLOAT3 overlap{};
        overlap.x = (size.x * 0.5 + targetSize.x * 0.5) - fabsf(delta.x);
        overlap.y = (size.y * 0.5 + targetSize.y * 0.5) - fabsf(delta.y);
        overlap.z = (size.z * 0.5 + targetSize.z * 0.5) - fabsf(delta.z);

        XMVECTOR dir;
        float penetration;

        if (overlap.x <= overlap.y && overlap.x <= overlap.z)
        {
            penetration = overlap.x;
            if (transform.position.x < targetPos.x)
            {
                dir = XMVectorSet(-1, 0, 0, 0);
            }
            else
            {
                dir = XMVectorSet(1, 0, 0, 0);
            }
        }
        else if (overlap.y <= overlap.z)
        {
            penetration = overlap.y;
            if (transform.position.y < targetPos.y)
            {
                dir = XMVectorSet(0, -1, 0, 0);
            }
            else
            {
                dir = XMVectorSet(0, 1, 0, 0);
            }
        }
        else
        {
            penetration = overlap.z;
            if (transform.position.z < targetPos.z)
            {
                dir = XMVectorSet(0, 0, -1, 0);
            }
            else
            {
                dir = XMVectorSet(0, 0, 1, 0);
            }
        }

        const float SLOP{ 0.1f };

        vPos += dir * ((penetration - SLOP) * 0.5);
        vTargetPos += -dir * ((penetration - SLOP) * 0.5);

        // 速度も殺さねば
        //XMVECTOR velocity = XMLoadFloat3(&velocity_);

        //XMVECTOR vn = XMVector3Dot(velocity, dir);
        //if (XMVectorGetX(vn) < 0)
        //{
        //    velocity -= dir * XMVectorGetX(vn);
        //}

        XMStoreFloat3(&_pTarget->GetTransform()->position, vTargetPos);
        XMStoreFloat3(&transform.position, vPos);
    }

}

void Enemy::SetAnimation(EnemyAnimation _enemyAnimation)
{
    eaState_ = _enemyAnimation;

    // アニメーションのフレーム数がハードコーディングされているので、
    // csvから読み込むようにしたいよなあ。
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

float Enemy::GetToPlayerDist()
{
    return 0.0f;
}

