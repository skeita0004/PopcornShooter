#include "Reticle.hpp"
#include "Image.hpp"

Reticle::Reticle(GameObject* _pParent) :
    GameObject(_pParent, "Reticle"),
    hReticle_(-1)
{
}

void Reticle::Init()
{
    hReticle_ = Image::Load("Images/Reticle.png");
    transform.position = XMFLOAT3(0, 0, 0);
}

void Reticle::Update()
{
}

void Reticle::Draw()
{
    Image::SetTransform(hReticle_, transform);
    Image::Draw(hReticle_);
}

void Reticle::Release()
{
}
