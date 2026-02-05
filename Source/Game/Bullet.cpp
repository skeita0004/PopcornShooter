#include "Bullet.hpp"
#include "Model.hpp"
#include <imgui.h>
#include <algorithm>
#undef min
#undef max

Bullet::Bullet(GameObject* _pParent) :
    GameObject(_pParent, "Bullet"),
    hModel_(-1),
    isAvailable_(true),
    isTouchDown_(false),
    lifeTime_(0),
    deceleration_(0.0f)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{


    hModel_ = Model::Load("Models/Weapon/bullet.fbx");
    pCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
    AddCollider(pCollider_);

}

void Bullet::Update()
{
    switch (state_)
    {
        case Bullet::B_CORN:
            // モデル変える（二つ読んでおいて、hModelにどっちかを代入すればよい。）
            UpdateCorn();
            break;

        case Bullet::B_POPCORN:

            UpdatePopCorn();
            break;
        default:
            break;
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

void Bullet::UpdateCorn()
{
    //if (not(isAvailable_))
    {
        ImGui::Begin("Bullet Info");
        ImGui::InputFloat("BulletX", &transform.position.x);
        ImGui::InputFloat("BulletY", &transform.position.y);
        ImGui::InputFloat("BulletZ", &transform.position.z);

        ImGui::Text("BulletDirX:%f", XMVectorGetX(vDir_));
        ImGui::Text("BulletDirY:%f", XMVectorGetY(vDir_));
        ImGui::Text("BulletDirZ:%f", XMVectorGetZ(vDir_));

        ImGui::End();

        XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

        vPos = vPos + (vDir_ * speed_);

        XMStoreFloat3(&transform.position, vPos);

        if (lifeTime_ >= (60 * 2))
        {
            lifeTime_ = 0;
            isAvailable_ = true;
            DeleteMe();
        }
        if (transform.position.y < 0.0f)
        {
            isTouchDown_ = true;
            speed_ = 0;
            transform.position.y = 0;
            //   DeleteMe();
        }

        lifeTime_++;

        XMFLOAT3 dir;
        XMStoreFloat3(&dir, vDir_);

        dir.y -= 0.01f;

        vDir_ = XMLoadFloat3(&dir);
        deceleration_ = -0.2f;

        if (isTouchDown_ == false)
        {
            speed_ += deceleration_;

            speed_ = std::max(speed_, 0.1f);
        }

        XMStoreFloat3(&transform.position, vPos);
    }

}

void Bullet::UpdatePopCorn()
{
    DeleteCollider();
    //if (not(isAvailable_))
    {
        ImGui::Begin("Bullet Info");
        ImGui::InputFloat("BulletX", &transform.position.x);
        ImGui::InputFloat("BulletY", &transform.position.y);
        ImGui::InputFloat("BulletZ", &transform.position.z);

        ImGui::Text("BulletDirX:%f", XMVectorGetX(vDir_));
        ImGui::Text("BulletDirY:%f", XMVectorGetY(vDir_));
        ImGui::Text("BulletDirZ:%f", XMVectorGetZ(vDir_));

        ImGui::End();

        XMVECTOR vPos{ XMLoadFloat3(&transform.position) };

        vPos = vPos + (vDir_ * speed_);

        XMStoreFloat3(&transform.position, vPos);

        if (lifeTime_ >= (60 * 2))
        {
            lifeTime_ = 0;
            isAvailable_ = true;
            DeleteMe();
        }
        if (transform.position.y < 0.0f)
        {
            isTouchDown_ = true;
            speed_ = 0;
            vPos = XMVectorSetY(vPos, 0.5f);
            DeleteCollider();
        }

        lifeTime_++;

        XMFLOAT3 dir;
        XMStoreFloat3(&dir, vDir_);

        dir.y -= 0.8f;

        vDir_ = XMLoadFloat3(&dir);

        deceleration_ = -0.9f;

        if (isTouchDown_ == false)
        {
            speed_ += deceleration_;

            speed_ = std::max(speed_, 0.05f);
        }

        XMStoreFloat3(&transform.position, vPos);
    }

}

void Bullet::Shoot()
{
    EnableDraw();
}

bool Bullet::IsAvailable()
{
    return isAvailable_;
}
