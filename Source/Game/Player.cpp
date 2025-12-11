#include "Player.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include <algorithm>
#include <format>
#include "Stage.hpp"
#include "Model.hpp"
#include "Gun.hpp"
#include <imgui.h>

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
    Stage* pStage = (Stage*)FindObject("Stage");
    hGround_ = pStage->GetModelHandle();
}

void Player::Update()
{
    const  XMVECTOR V_DIR_NONE{XMVectorSet(0, 0, 0, 0)};
    static XMMATRIX rotXMat{XMMatrixIdentity()};
    static XMMATRIX rotYMat{XMMatrixIdentity()};

    XMVECTOR vCamForward{ XMVectorSet(0, 0, 1, 0) };

    GetCamForwardRenew(rotXMat, rotYMat, vCamForward);
    
    // 移動
    XMVECTOR vForward{XMVectorSet(0, 0, 1, 0)};
    XMVECTOR vMoveDir{V_DIR_NONE};
    XMVECTOR vPos{};
    vPos = XMLoadFloat3(&transform.position);

    vForward = XMVector3TransformCoord(vForward, rotYMat);
    
    if (Input::IsKey(DIK_W))
    {
        vMoveDir += vForward;
    }
    if (Input::IsKey(DIK_S))
    {
        vMoveDir += XMVectorNegate(vForward);
    }
    if (Input::IsKey(DIK_A))
    {
        // forwardから見た左 
        // forwardに対して-90°のベクトルを求める
        // 以下の方法は悪手なので、即座にやめること。
        vMoveDir += XMVector3TransformCoord(vForward, XMMatrixRotationY(XMConvertToRadians(-90)));
    }
    if (Input::IsKey(DIK_D))
    {
        // forwardから見た右
        // forwardに対して、90°のベクトルを求める
        vMoveDir += XMVector3TransformCoord(vForward, XMMatrixRotationY(XMConvertToRadians(90)));
    }
    if (not(isJump_)) // <- 二段ジャンプを禁ずる（禁固5年に処す）
    {
        if (Input::IsKeyDown(DIK_SPACE))
        {
            isJump_ = true;
        }
    }

    // 正規化する
    XMFLOAT3 stick = XMFLOAT3(Input::GetPadStickL().x, 0, Input::GetPadStickL().y);
    XMVECTOR vStick = XMLoadFloat3(&stick);
    XMVector3Normalize(vStick);
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

    XMVECTOR velocity{vMoveDir * moveSpeed_};
    XMVECTOR padVelocity{ vStick * moveSpeed_ };

    vPos = XMVectorAdd(vPos, velocity);
    vPos = XMVectorAdd(vPos, padVelocity);
    // 移動_

    // 地面との当たり判定(読みにくい)
    const XMFLOAT3 OFFSET{0, 100.f, 0};
    RayCastData rayCastData;
    XMFLOAT3 rayCastStart{};
    XMVECTOR vRayCastStart = XMVectorAdd(vPos, XMLoadFloat3(&OFFSET)); // プレイヤーの高さ + 100する
    XMStoreFloat3(&rayCastStart, vRayCastStart); // 変換
    rayCastData.start = rayCastStart;            // プレイヤーの高さ + 100のところを始点とする
    rayCastData.dir   = XMFLOAT3(0.f, -1.f, 0.f);
    Model::RayCast(hGround_, &rayCastData);

#pragma region Jump
    static float jumpHei{};
    if (isJump_)
    {
        jumpHei = Jump();
    }

#pragma endregion 
    XMVECTOR vDist = XMVectorSet(0.f, rayCastData.dist - OFFSET.y - jumpHei, 0.f, 0.f);

    if (rayCastData.hit)
    {
        vPos = XMVectorSubtract(vPos, vDist);
    }

    CamRenew(vPos, vCamForward);
   
    ImGui::Begin("PlayerPosition");

    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);

    ImGui::End();

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
    static int callCount{0};
    const float MAX_HEIGHT        {400.f};
    const float GRAVITY           {1.75f};
    const float INITIAL_VELOCITY_Y{sqrtf(2 * (MAX_HEIGHT * GRAVITY))};

    static float jumpVelocityY{ 0.f }; // ここがベクトルじゃないのがまずおかしいわよ。
    // あとVelocityじゃなくてSpeedじゃないかしら？
    // ママ、本来ならば、移動処理とともにやるべきだと思うの。
    // (西麻布のママより)
    static float jumpHei{ 0.f };

    if (callCount == 0)
    {
        jumpVelocityY = INITIAL_VELOCITY_Y;
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

    return jumpHei;
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
    // カメラ位置についての処理
    XMFLOAT3 camPos{};
    XMVECTOR vCamPos{};
    vCamPos = XMVectorAdd(_vPos, XMVectorSet( 0, 20, 0, 0 ));
    XMStoreFloat3(&camPos, vCamPos);
    Camera::SetPosition(camPos);

    // 注視点についての処理
    XMFLOAT3 camTarget{};
    XMVECTOR vCamTarget{};
    vCamTarget = vCamPos + _vCamForward;
    XMStoreFloat3(&camTarget, vCamTarget);
    Camera::SetTarget(camTarget);
}
