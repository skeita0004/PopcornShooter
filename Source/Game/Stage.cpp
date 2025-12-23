#include "Stage.hpp"
#include "Model.hpp"
#include "Player.hpp"
#include "SkyDome.hpp"
#include "CameraSet.hpp"

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

    //CameraSet camera{};
    //int hCam = camera.Create({0, -1, 0}, 120, 0.1f, 100000.f);
    //camera.SetCurrent(hCam);
}

void Stage::Update()
{
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
