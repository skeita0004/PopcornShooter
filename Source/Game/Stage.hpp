#pragma once
#include "GameObject.hpp"

class Stage : public GameObject
{
public:
    Stage(GameObject* _pParent);
    ~Stage();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    int GetModelHandle() { return hModel_; }

private:
    void PutEnemy(int _enemyNum, XMFLOAT3 _enemyPos);

    int hModel_;
};