#include "SkyDome.hpp"
#include "Model.hpp"
#include "CameraSet.hpp"
#include <imgui.h>

Skybox::Skybox(GameObject* _pParent) :
    GameObject(_pParent, "Skybox"),
    hModel_(-1),
    pCameraSet_(nullptr)
{
}

Skybox::~Skybox()
{
}

void Skybox::Init()
{
    hModel_ = Model::Load("Models/Skybox/sky.fbx");
    pCameraSet_ = new CameraSet();

    transform.scale = { 1000.f, 1000.f, 1000.f };
}

void Skybox::Update()
{
    transform.position = pCameraSet_->GetCurrent()->GetTransform().position;

    ImGui::Begin("SkyDomePosition");
    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);
    ImGui::End();

    ImGui::Begin("SkyDomeScale");
    ImGui::InputFloat("X: ", &transform.scale.x);
    ImGui::InputFloat("Y: ", &transform.scale.y);
    ImGui::InputFloat("Z: ", &transform.scale.z);
    ImGui::End();
}

void Skybox::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Skybox::Release()
{
}
