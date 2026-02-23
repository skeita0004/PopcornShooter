#include "Gun.hpp"
#include "Model.hpp"
#include <imgui.h>
#include "CameraSet.hpp"
#include "Player.hpp"

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

    // どういう場所なのかを割り出す
    CameraSet camera;
    transform.pParent = &camera.GetCurrent()->GetTransform();
}

void Gun::Update()
{
    static Player* pPlayer = FindObject<Player>("Player");

    XMFLOAT3 playerRot{ pPlayer->GetTransform()->rotate };

    transform.rotate.x = playerRot.x;
    transform.rotate.y = playerRot.y;

    XMMATRIX rot{XMMatrixRotationX(XMConvertToRadians(playerRot.x)) *
                 XMMatrixRotationY(XMConvertToRadians(playerRot.y))};

    XMVECTOR offset{XMVectorSet(0, 50, 60, 0)};
    offset = XMVector3TransformCoord(offset, rot);

    XMVECTOR pos{XMLoadFloat3(&pPlayer->GetTransform()->position) + offset};
    XMStoreFloat3(&transform.position, pos);

#ifdef _DEBUG
   ImGui::Begin("GunPosition");

    ImGui::InputFloat("X: ", &transform.position.x);
    ImGui::InputFloat("Y: ", &transform.position.y);
    ImGui::InputFloat("Z: ", &transform.position.z);

    ImGui::End();
#endif
}

void Gun::Draw()
{
    ImGui::Begin("GunAlive");
    ImGui::Text("Gun Draw Called");
    ImGui::End();

    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Gun::Release()
{
}

void Gun::Shoot()
{
    
}
