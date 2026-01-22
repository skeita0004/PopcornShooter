#pragma once
#include "GameObject.hpp"
#include "BoxCollider.hpp"

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
        RETURN,
        DEAD,
        MAX_STATE
    };

    int hModel_;
    BoxCollider* pBoxCollider_;


    void OnCollision(GameObject* _pTarget) override;
    void ChangeAnimation(EnemyAnimation _enemyAnimation);
};