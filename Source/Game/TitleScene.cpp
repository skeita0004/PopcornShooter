#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "Image.hpp"

TitleScene::TitleScene(GameObject* _pParent) :
    GameObject(_pParent, "TitleScene"),
    pSceneManager_(nullptr),
    hImage_(-1),
    hStartImage_(-1)
{
}

void TitleScene::Init()
{
    hImage_ = Image::Load("Images/title.png");
    hStartImage_  = Image::Load("Images/start1.png");
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
}

void TitleScene::Update()
{
    static Transform startImageT{};
    startImageT.position.y = -0.4f;
    startImageT.scale = XMFLOAT3(0.7, 0.7, 1.0);
    Image::SetTransform(hStartImage_, startImageT);
    if (Input::IsKeyDown(DIK_SPACE) or Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::PLAY);
    }
}

void TitleScene::Draw()
{
    Image::Draw(hImage_);
    Image::Draw(hStartImage_);
}

void TitleScene::Release()
{
}
