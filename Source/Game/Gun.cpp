#include "Gun.hpp"
#include "Model.hpp"
#include <imgui.h>
#include "CameraSet.hpp"

Gun::Gun(GameObject* _pParent) :
    GameObject(_pParent, "Gun"),
    hModel_(-1)
{
}

Gun::~Gun()
{
}

void Gun::Init()
{
    // モデルロード
    hModel_ = Model::Load("Models/Weapon/gun.fbx");
    transform.scale = {5.f, 5.f, 5.f};
}

void Gun::Update()
{
    static CameraSet cameraSet{};
    transform.position = cameraSet.GetCurrent()->GetTransform().position;
    transform.position.z += 10;
    transform.position.y -= 10;
    ImGui::Begin("GunPosition");

    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);

    ImGui::End();
}

void Gun::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Gun::Release()
{
}

void Gun::Shoot()
{

}
