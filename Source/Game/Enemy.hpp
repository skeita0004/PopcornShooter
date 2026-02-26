#pragma once
#include "GameObject.hpp"
#include "BoxCollider.hpp"
#include "Screen.hpp"

class Player;
class Stage;

class Enemy : public GameObject
{
public:
    Enemy(GameObject* _pParent);
    ~Enemy();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    enum EnemyAnimation
    {
        EA_IDLE = 0,
        EA_WALK,
        EA_RUN,
        EA_ATTACK,
        EA_HIT,
        EA_DEAD,
        MAX_ANIMATION
    };

    enum EnemyState
    {
        IDLE,
        CHASE,
        ATTACK,
        DEAD,
        MAX_STATE
    };

    void UpdateIdle();
    void UpdateChase();
    void UpdateAttack();
    void UpdateDead();
    void UpdateCommon();

    void RotateToPlayer();
    void OnCollision(GameObject* _pTarget) override;
    void SetAnimation(EnemyAnimation _enemyAnimation);
    float GetToPlayerDist();

    int hModel_;
    BoxCollider* pBoxCollider_;
    int currAnimFrame_;
    int attackInterval_;

    inline static const float SPEED{0.56f};
    inline static const int ATTACK_INTERVAL_TIME{2 * Screen::FPS};

    int32_t hp_;
    EnemyAnimation eaState_;
    EnemyState     state_;
    bool isAlive_;

    Player* pPlayer_;

    Stage*  pStage_;
    int     hStage_;
};