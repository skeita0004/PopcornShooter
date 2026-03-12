#pragma once
#include "GameObject.hpp"
#include <vector>

class Enemy;
class Player;

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

    int GetEnemyNum();

    bool PlayerIsDead();

private:
    void PutEnemy(int _enemyNum, XMFLOAT3 _enemyPos);

    int hModel_;
    std::vector<Enemy*> pEnemys_;
    Player* pPlayer_;
};