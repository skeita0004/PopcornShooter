#include "Skybox.h"
#include "Model.h"

Skybox::Skybox(GameObject* _pParent) :
    GameObject(_pParent, "Skybox"),
    hModel_(-1)
{
}

Skybox::~Skybox()
{
}

void Skybox::Init()
{
    hModel_ = Model::Load("Models/Skybox/sky.fbx");
    SetScale(10.f, 10.f, 10.f);
}

void Skybox::Update()
{
}

void Skybox::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Skybox::Release()
{
}
