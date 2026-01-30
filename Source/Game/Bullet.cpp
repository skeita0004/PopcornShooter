#include "Bullet.hpp"
#include "Model.hpp"
#include <imgui.h>
#include <algorithm>
#undef min
#undef max

Bullet::Bullet(GameObject* _pParent) :
    GameObject(_pParent, "Bullet"),
    hModel_(-1),
    isAvailable_(true)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
    hModel_ = Model::Load("Models/Weapon/bullet.fbx");
    pCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 1.f);
    AddCollider(pCollider_);

}

void Bullet::Update()
{
    //if (not(isAvailable_))
    {
        static int lifeTime = 0;

        ImGui::Begin("Bullet Info");
        ImGui::InputFloat("BulletX", &transform.position.x);
        ImGui::InputFloat("BulletY", &transform.position.y);
        ImGui::InputFloat("BulletZ", &transform.position.z);

        ImGui::Text("BulletDirX:%f", XMVectorGetX(vDir_));
        ImGui::Text("BulletDirY:%f", XMVectorGetY(vDir_));
        ImGui::Text("BulletDirZ:%f", XMVectorGetZ(vDir_));
        
        ImGui::End();

        XMVECTOR vPos{XMLoadFloat3(&transform.position)};

        vPos = vPos + (vDir_ * speed_);

        XMStoreFloat3(&transform.position, vPos);

        if (lifeTime >= (60 * 3))
        {
            lifeTime = 0;
            isAvailable_ = true;
            DeleteMe();
        }
        if (transform.position.y < -1.0f)
        {
            DeleteMe();
        }

        lifeTime++;

        XMFLOAT3 dir;
        XMStoreFloat3(&dir, vDir_);

        dir.y -= 0.01f;

        vDir_ = XMLoadFloat3(&dir);

        static float deceleration{-0.02f};

        speed_ += deceleration;
        speed_ = std::max(speed_, 0.1f);

        XMStoreFloat3(&transform.position, vPos);
    }
}

void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform);
    Model::Draw(hModel_);
}

void Bullet::Release()
{
}

void Bullet::Shoot()
{
    EnableDraw();
}

bool Bullet::IsAvailable()
{
    return isAvailable_;
}
