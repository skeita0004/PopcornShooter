#include "PlayScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"

PlayScene::PlayScene(GameObject* _pParent) :
    GameObject(_pParent, "PlayScene")
{
}

void PlayScene::Init()
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
