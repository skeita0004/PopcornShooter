#pragma once
#include "GameObject.hpp"

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

private:
    int  hModel_;
    bool isAvailable_;
};