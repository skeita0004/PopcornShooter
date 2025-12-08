#include "PlayScene.hpp"
#include "Stage.hpp"

PlayScene::PlayScene(GameObject* _pParent) :
    GameObject(_pParent, "PlayScene")
{
}

void PlayScene::Initialize()
{
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
