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
