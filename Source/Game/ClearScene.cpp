#include "ClearScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

ClearScene::ClearScene(GameObject* _pParent) :
    GameObject(_pParent, "ClearScene"),
    pSceneManager_(nullptr)
{
}

void ClearScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
}

void ClearScene::Update()
{
}

void ClearScene::Draw()
{
}

void ClearScene::Release()
{
}
