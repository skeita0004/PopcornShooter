#include "OverScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

OverScene::OverScene(GameObject* _pParent) :
    GameObject(_pParent, "OverScene"),
    pSceneManager_(nullptr)
{
}

void OverScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
}

void OverScene::Update()
{
}

void OverScene::Draw()
{
}

void OverScene::Release()
{
}
