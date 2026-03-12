#pragma once
#include "GameObject.hpp"
#include "BoxCollider.hpp"

class Gun;
class SceneManager;

class Player : public GameObject
{
public:
    Player(GameObject* _pParent);
    ~Player();

    //初期化
    void Init() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetHp()
    {
        return hp_;
    }

    float GetGunTemp()
    {
        return gunTemp_;
    }

private:
    float Jump();

    /// @brief プレイヤーの回転行列から、カメラの正面を求める
    /// @param _rotXMat X軸回転行列
    /// @param _rotYMat Y軸回転行列
    /// @param _vCamForward カメラの正面ベクトル
    void GetCamForwardRenew(XMMATRIX& _rotXMat,
                            XMMATRIX& _rotYMat,
                            XMVECTOR& _vCamForward);

    /// @brief カメラ位置、注視点位置を更新する
    /// @param _vPos プレイヤの位置ベクトル
    /// @param _vCamForward カメラの正面ベクトル
    void CamRenew(const XMVECTOR& _vPos,
                  const XMVECTOR& _vCamForward);

    /// @brief 当たり判定
    /// @param _pTarget 当たった相手へのポインタ
    void OnCollision(GameObject* _pTarget) override;
  
    /// @brief プレイヤ最高速度
    static const inline float MAX_SPEED{0.6f};
    
    /// @brief プレイヤ最高加速度
    static const inline float ACCELARATION{0.02f};

    /// @brief 移動速度
    float moveSpeed_;

    /// @brief 地面のモデルハンドル
    int   hGround_;

    /// @brief プレイヤジャンプフラグ
    bool  isJump_;

    float gunTemp_; // 銃温度（一時的にここで持つ）
    bool  isOverHeat_;

    /// @brief 銃
    Gun* pGun_;

    BoxCollider* pBoxCollider_;

    XMFLOAT3 velocity_;

    int hp_;
    SceneManager* pSM_;
};