#include "Gun.hpp"
#include "Model.hpp"
#include <imgui.h>

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
    ImGui::Begin("GunPosition");

    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);

    ImGui::End();
    // アニメーション
    // 弾のVelocityはここで決める
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
