#pragma once
#include "GameObject.hpp"
#include "SphereCollider.hpp"

class Bullet : public GameObject
{
public:
    Bullet(GameObject* _pParent);
    ~Bullet();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

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

private:
    int  hModel_;
    bool isAvailable_;

    XMVECTOR vDir_;
    float    speed_;

    SphereCollider* pCollider_;
};