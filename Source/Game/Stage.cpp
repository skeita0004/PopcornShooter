#include "Stage.hpp"
#include "Model.hpp"
#include "Player.hpp"
#include "SkyDome.hpp"

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
    hModel_ = Model::Load("Models/Stage/terrain.fbx");
    Instantiate<Player>(GetParent());
    Instantiate<Skybox>(GetParent());
    const float SCALE{20.f};
    transform.scale = { SCALE, SCALE, SCALE };
}

void Stage::Update()
{
}

void Stage::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}
