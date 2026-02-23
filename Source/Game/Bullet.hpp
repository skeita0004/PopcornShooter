#pragma once
#include "GameObject.hpp"
#include "SphereCollider.hpp"
#include <map>

class Bullet : public GameObject
{
public:
    Bullet(GameObject* _pParent);
    ~Bullet();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    enum BulletState
    {
        B_CORN,
        B_POPCORN
    };

    void UpdateCorn();
    void UpdatePopCorn();

    void Shoot();
    bool IsAvailable();

    void SetSpeed(float _speed)
    {
        speed_ = _speed;
    }

    float GetSpeed()
    {
        return speed_;
    }

    void     SetDir(XMVECTOR _vDir)
    {
        vDir_ = _vDir;
    }

    XMVECTOR GetVDir()
    {
        return vDir_;
    }

    void SetState(BulletState _state)
    {
        state_ = _state;
    }

private:
    int  hModel_;
    std::map<BulletState, int> hModels_;

    bool isAvailable_;

    BulletState state_;

    XMVECTOR vDir_;
    float    speed_;

    SphereCollider* pCollider_;

    bool isTouchDown_;
    int  lifeLimitCounter_;
    float deceleration_;
};