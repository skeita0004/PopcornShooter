#include "Skybox.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include <imgui.h>

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
    transform.scale = { 1000.f, 1000.f, 1000.f };
}

void Skybox::Update()
{
    transform.position = Camera::GetPosition();
}

void Skybox::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Skybox::Release()
{
}
