#include "Player.hpp"
#include "CameraSet.hpp"
#include "Input.hpp"
#include <algorithm>
#include <format>
#include "Stage.hpp"
#include "Model.hpp"
#include "Gun.hpp"
#include "Bullet.hpp"
#include <imgui.h>
#include <cstdarg>
#include "SceneManager.hpp"

#undef min
#undef max

Player::Player(GameObject* _pParent) :
    GameObject(_pParent, "Player"),
    isJump_(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
    hp_ = 200;
    Stage* pStage = FindObject<Stage>("Stage");
    hGround_ = pStage->GetModelHandle();

    pGun_ = static_cast<Gun*>(Instantiate<Gun>(nullptr));

    pBoxCollider_ = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 8, 1));
    AddCollider(pBoxCollider_);

    gunTemp_ = 0.0f;

    transform.position = XMFLOAT3( 75.f, 0.f, -75.f );
    pSM_ = static_cast<SceneManager*>(FindObject<SceneManager>("SceneManager"));
}

void Player::Update()
{
    // 比較演算用ゼロベクトル
    const  XMVECTOR V_DIR_NONE{ XMVectorSet(0, 0, 0, 0) };

    // 回転行列たち
    static XMMATRIX rotXMat{ XMMatrixIdentity() };
    static XMMATRIX rotYMat{ XMMatrixIdentity() };

    // カメラの正面
    XMVECTOR vCamForward{ XMVectorSet(0, 0, 1, 0) };

    // プレイヤーの正面
    XMVECTOR vForward{ XMVectorSet(0, 0, 1, 0) };

    // プレイヤーの移動方向
    XMVECTOR vMoveDir{ V_DIR_NONE };

    // プレイヤーの位置ベクトル
    XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

    GetCamForwardRenew(rotXMat, rotYMat, vCamForward);

    vForward = XMVector3TransformCoord(vForward, rotYMat);

    // プレイヤーの右を表すベクトル
    XMVECTOR vRight{ XMVector3TransformCoord(vForward, XMMatrixRotationY(XMConvertToRadians(-90))) };

#pragma region GetInput
    // 入力取得
    if (Input::IsKey(DIK_W) or Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP))
    {
        vMoveDir += vForward;
    }
    if (Input::IsKey(DIK_S) or Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN))
    {
        vMoveDir += XMVectorNegate(vForward);
    }
    if (Input::IsKey(DIK_A) or Input::IsPadButton(XINPUT_GAMEPAD_DPAD_LEFT))
    {
        vMoveDir += vRight;
    }
    if (Input::IsKey(DIK_D) or Input::IsPadButton(XINPUT_GAMEPAD_DPAD_RIGHT))
    {
        vMoveDir += XMVectorNegate(vRight);
    }

    // 壁にめり込んじゃうのでジャンプ禁止(禁固2年または5000万円以下の罰金)
    //if (Input::IsKeyDown(DIK_SPACE))
    //{
    //    if (not(isJump_)) //二段ジャンプを禁ずる（禁固5年に処す）
    //    {
    //        isJump_ = true;
    //    }
    //}

    if (Input::IsMouseButton(Input::MOUSE::LEFT) or Input::GetPadTrrigerR() > 0.0f)
    {
        Bullet* pBullet = static_cast<Bullet*>(Instantiate<Bullet>(GetParent()));
        pBullet->GetTransform()->position = XMFLOAT3(transform.position.x,
                                                     transform.position.y + 2.25f,
                                                     transform.position.z);
        pBullet->SetDir(vCamForward);
        pBullet->SetSpeed(2.00f);

        gunTemp_ += 0.5f / 60.f;

        isOverHeat_ = false;
        pBullet->SetState(Bullet::B_CORN);

        if (gunTemp_ >= 2.f)
        {
            gunTemp_ = std::min(gunTemp_, 3.0f);

            isOverHeat_ = true;
            pBullet->SetObjectName("PopcornBullet");
            pBullet->SetState(Bullet::B_POPCORN);
        }
    }
    if (not(Input::IsMouseButton(Input::MOUSE::LEFT) or Input::GetPadTrrigerR() > 0.0f)) // 発射時、ここは通らない
    {
        gunTemp_ -= 0.3f / 60;
        gunTemp_ = std::max(gunTemp_, 0.0f);
    }


    XMFLOAT3 stick = XMFLOAT3(Input::GetPadStickL().x, 0, Input::GetPadStickL().y);
    XMVECTOR vStick = XMLoadFloat3(&stick);
#pragma endregion

#pragma region ComputeMove
    // 正規化する
   vStick =  XMVector3Normalize(vStick);
    vStick = XMVector3TransformCoord(vStick, rotYMat);

    vForward = XMVector3Normalize(vForward);

    // ここで、velocityを求めて代入すればよろしい。
    vMoveDir = { XMVector3Normalize(vMoveDir) }; // 方向ベクトルを正規化する

    // 立ち止まっていたら、速度をリセット
    if (XMVector4Equal(vMoveDir, V_DIR_NONE))
    {
        moveSpeed_ = 0.0f;
    }

    // 最大速度以下の場合、加速
    if (moveSpeed_ <= MAX_SPEED)
    {
        moveSpeed_ += ACCELARATION;
    }
    // 最大速度を超えてしまっている場合、値を最大速度に収める
    else if (moveSpeed_ >= MAX_SPEED)
    {
        moveSpeed_ = MAX_SPEED;
    }

    XMVECTOR velocity{ vMoveDir * moveSpeed_ };
    XMVECTOR padVelocity{ vStick * moveSpeed_ };

    XMStoreFloat3(&velocity_, velocity);


    vPos = XMVectorAdd(vPos, velocity);
    vPos = XMVectorAdd(vPos, padVelocity);

    RayCastData toWall{};
    XMFLOAT3 toWallStart{};
    XMFLOAT3 toWallDir{};
    XMStoreFloat3(&toWallStart, vPos);
    XMStoreFloat3(&toWallDir, vMoveDir);
    toWall.dir = toWallDir;
    toWall.start = toWallStart;
    Model::RayCast(hGround_, &toWall);

    if (toWall.dist <= 3.f)
    {
        vPos -= velocity;
    }

#pragma endregion

    // 地面との当たり判定(読みにくい)
    const XMFLOAT3 OFFSET{ 0, 2.f, 0 };

    RayCastData rayCastData{};
    XMFLOAT3 rayCastStart{};
    XMVECTOR vRayCastStart = XMVectorAdd(vPos, XMLoadFloat3(&OFFSET)); // プレイヤーの高さ + 100する
    XMStoreFloat3(&rayCastStart, vRayCastStart); // 変換
    rayCastData.start = rayCastStart;            // プレイヤーの高さ + 100のところを始点とする
    rayCastData.dir = XMFLOAT3(0.f, -1.f, 0.f);
    Model::RayCast(hGround_, &rayCastData);

#pragma region Jump
    static float jumpHei{};
    if (isJump_)
    {
        jumpHei = Jump();
        XMStoreFloat3(&transform.position, vPos);
        transform.position.y += jumpHei;
        vPos = XMLoadFloat3(&transform.position);
    }
#pragma endregion 

    // 地面との当たり判定
    XMVECTOR vDist = XMVectorSet(0.f, rayCastData.dist - OFFSET.y, 0.f, 0.f);

    if (not(isJump_))
    {
        if (rayCastData.hit)
        {
            vPos = XMVectorSubtract(vPos, vDist);
        }
    }

    CamRenew(vPos, vCamForward);

#pragma region DebugPrint
#ifdef _DEBUG
    ImGui::Begin("PlayerPosition");
    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);

    ImGui::InputFloat("GunTemp: ", &gunTemp_);
    ImGui::InputFloat("jumpHei", &jumpHei);
    ImGui::End();
#endif
#pragma endregion


    XMStoreFloat3(&transform.position, vPos);

} // End Update

void Player::Draw()
{
    // ここで影を描画したらよいのでは。
}

void Player::Release()
{
}

float Player::Jump()
{
    static int callCount{ 0 };
    const float MAX_HEIGHT{ 4.0f };
    const float GRAVITY{ 0.025f };
    const float INITIAL_SPEED_Y{ sqrtf(2 * (MAX_HEIGHT * GRAVITY)) };

    static float jumpVelocityY{ 0.f }; // ここがベクトルじゃないのがまずおかしいわよ。
    // あとVelocityじゃなくてSpeedじゃないかしら？
    // ママ、本来ならば、移動処理とともにやるべきだと思うの。
    // (西麻布のママより)
    static float jumpHei{ 0.f };

    if (callCount == 0)
    {
        jumpVelocityY = INITIAL_SPEED_Y;
        callCount = 1;
    }

    // ジャンプ
    jumpVelocityY -= GRAVITY;
    jumpHei += jumpVelocityY;

    if (jumpHei <= 0.f)
    {
        jumpHei = 0.0f;
        isJump_ = false;
        callCount = 0;
    }

    return jumpVelocityY;
}

void Player::GetCamForwardRenew(XMMATRIX& _rotXMat,
                                XMMATRIX& _rotYMat,
                                XMVECTOR& _vCamForward)
{
    transform.rotate.x += Input::GetMouseMove().y;
    transform.rotate.y += Input::GetMouseMove().x;

    transform.rotate.x -= Input::GetPadStickR(7.5f).y;
    transform.rotate.y += Input::GetPadStickR(7.5f).x;

    transform.rotate.x = std::clamp(transform.rotate.x, -30.f, 30.f);

    _rotXMat = XMMatrixRotationX(XMConvertToRadians(transform.rotate.x));
    _rotYMat = XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));

    _vCamForward = XMVector3TransformCoord(_vCamForward, _rotXMat * _rotYMat);
}

void Player::CamRenew(const XMVECTOR& _vPos,
                      const XMVECTOR& _vCamForward)
{
    static CameraSet cameraSet{};
    // カメラ位置についての処理
    XMFLOAT3 camPos{};
    XMVECTOR vCamPos{};
    vCamPos = XMVectorAdd(_vPos, XMVectorSet(0, 2.5, 0, 0));
    XMStoreFloat3(&camPos, vCamPos);
    cameraSet.GetCurrent()->GetTransform().position = camPos;

    // 注視点についての処理
    XMFLOAT3 camTarget{};
    XMVECTOR vCamTarget{};
    vCamTarget = vCamPos + _vCamForward * 2;
    XMStoreFloat3(&camTarget, vCamTarget);
    cameraSet.GetCurrent()->GetTargetTransform().position = camTarget;
}

void Player::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "Enemy") // 相手のコライダーの種類を取得できるようにする)
    {
        // 2D用の当たり判定処理にする（とてもシンプル）

        XMFLOAT3 targetPos = _pTarget->GetTransform()->position;
        XMFLOAT3 targetSize = _pTarget->GetBoxColliderSize();
        XMFLOAT3 size = GetBoxColliderSize();

        XMVECTOR vTargetPos{ XMLoadFloat3(&targetPos) };
        XMVECTOR vPos{ XMLoadFloat3(&transform.position) };
        XMVECTOR vTargetSize{ XMLoadFloat3(&targetSize) };
        XMVECTOR vSize{ XMLoadFloat3(&size) };

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
        XMVECTOR velocity = XMLoadFloat3(&velocity_);

        XMVECTOR vn = XMVector3Dot(velocity, dir);
        if (XMVectorGetX(vn) < 0)
        {
            velocity -= dir * XMVectorGetX(vn);
        }

        XMStoreFloat3(&_pTarget->GetTransform()->position, vTargetPos);
        XMStoreFloat3(&transform.position, vPos);
    }

    if (_pTarget->GetObjectName() == "EnemyBullet")
    {
         hp_ -= 5;
        _pTarget->DeleteMe();
    }
}
