#include "PlayScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"
#include "Image.hpp"

PlayScene::PlayScene(GameObject* _pParent) :
    GameObject(_pParent, "PlayScene"),
    pSceneManager_(nullptr)
{
}

void PlayScene::Init()
{
    hReticle_ = Image::Load("Images/Reticle.png");
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    Instantiate<Stage>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
    static Transform imageTransform{};
    imageTransform.position = XMFLOAT3(1, 1, 0);

    Image::SetTransform(hReticle_, imageTransform);
    Image::Draw(hReticle_);
}

void PlayScene::Release()
{
}
