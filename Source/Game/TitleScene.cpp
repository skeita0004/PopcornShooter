#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

TitleScene::TitleScene(GameObject* _pParent) :
    GameObject(_pParent, "TitleScene"),
    pSceneManager_(nullptr)
{
}

void TitleScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
}

void TitleScene::Update()
{
    if (Input::IsKeyDown(DIK_P))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::PLAY);
    }
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
