#pragma once
#include "GameObject.hpp"

class Gun : public GameObject
{
public:
    Gun(GameObject* _pParent);
    ~Gun();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Shoot();

private:
    int hModel_;
};