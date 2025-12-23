#include "Bullet.hpp"
#include "Model.hpp"

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
    hModel_ = Model::Load("Weapon/bullet.fbx");
}

void Bullet::Update()
{
    if (not(isAvailable_))
    {
        static int lifeTime = 0;

        XMVECTOR vPos{XMLoadFloat3(&transform.position)};

        vPos = vDir_ * speed_;

        XMStoreFloat3(&transform.position, vPos);

        if (lifeTime >= (60 * 3))
        {
            lifeTime = 0;
            isAvailable_ = true;
            transform.position = {0, 0, 0};
        }
        lifeTime++;
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
    isAvailable_ = false;
}

bool Bullet::IsAvailable()
{
    return isAvailable_;
}
