#include "Stage.hpp"
#include "Model.hpp"
#include "Player.hpp"
#include "SkyDome.hpp"
#include "CameraSet.hpp"
#include "Enemy.hpp"
#include "Reticle.hpp"
#include <list>

Stage::Stage(GameObject* _pParent) :
    GameObject(_pParent, "Stage"),
    hModel_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
    hModel_ = Model::Load("Models/Stage/StagePlane.fbx");
    pPlayer_ = static_cast<Player*>(Instantiate<Player>(GetParent()));

    struct EnemyInfo
    {
        EnemyInfo(int _num, XMFLOAT3 _pos) : 
            num(_num),
            pos(_pos)
        {
        }
        int num;
        XMFLOAT3 pos;
    };

    // Enemy Setting Process
    EnemyInfo enemies[]
    {
        {7, XMFLOAT3(-35, 0, -75)},
        {7, XMFLOAT3(  0, 0, -35)},
        {7, XMFLOAT3(  0, 0,  35)},
        {7, XMFLOAT3( 75, 0,  35)},
        {7, XMFLOAT3(  0, 0,  75)},
        {7, XMFLOAT3(-35, 0,  35)},
        {7, XMFLOAT3(-75, 0,  75)},
    };

    for (int i = 0; i < sizeof(enemies) / sizeof(enemies[0]); i++)
    {
        PutEnemy(enemies[i].num, enemies[i].pos);
    }

    Instantiate<Skybox>(GetParent());
    Instantiate<Reticle>(GetParent());
    const float SCALE{0.75f};
    transform.scale = { SCALE, 0.5, SCALE };
    transform.position = {0.f, 0.f, 0.f};

    //CameraSet camera{};
    //int hCam = camera.Create({0, -1, 0}, 120, 0.1f, 100000.f);
    //camera.SetCurrent(hCam);


}

void Stage::Update()
{
    // 倒されたEnemyは削除
    for (auto it = pEnemys_.begin(); it != pEnemys_.end();)
    {
        if ((*it)->IsDelete())
        {
            it = pEnemys_.erase(it);
        }
        else
        {
            it++;
        }
    }
    //CameraSet camera{};
    //camera.GetCurrent()->GetTransform().position = { 0, 100, 0 };
    //camera.GetCurrent()->GetTargetTransform().position = {0, 0, 0};
}

void Stage::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}

int Stage::GetEnemyNum()
{
    return pEnemys_.size();
}

bool Stage::PlayerIsDead()
{
    return (pPlayer_->GetHp() < 0);
}

void Stage::PutEnemy(int _enemyNum, XMFLOAT3 _enemyPos)
{
    for (int i = 0; i < _enemyNum; i++)
    {
        pEnemys_.push_back(static_cast<Enemy*>(Instantiate<Enemy>(GetParent())));
        pEnemys_.back()->GetTransform()->position = _enemyPos;
    }
}
