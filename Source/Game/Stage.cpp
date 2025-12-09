#include "Stage.hpp"
#include "Model.h"
#include "Player.hpp"
#include "Skybox.h"

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
    Instantiate<Player>(this);
    Instantiate<Skybox>(this);
    SetScale(100.f, 100.f, 100.f);
}

void Stage::Update()
{
}

void Stage::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}
