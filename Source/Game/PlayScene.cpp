#include "PlayScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"

PlayScene::PlayScene(GameObject* _pParent) :
    GameObject(_pParent, "PlayScene"),
    pSceneManager_(nullptr)
{
}

void PlayScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    Instantiate<Stage>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
