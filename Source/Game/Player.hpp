#pragma once
#include "GameObject.hpp"

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

private:
    float Jump();
    void GetCamForwardRenew(XMMATRIX& _rotXMat,
                            XMMATRIX& _rotYMat,
                            XMVECTOR& _vCamForward);

    void CamRenew(const XMVECTOR& _vPos,
                  const XMVECTOR& _vCamForward);

    static const inline float MAX_SPEED{9.0f};
    static const inline float ACCELARATION{0.05f};

    float moveSpeed_;
    int   hGround_;
    bool  isJump_;
};