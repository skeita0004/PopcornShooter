#include "TestScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"

TestScene::TestScene(GameObject * parent) : 
    GameObject(parent, "TestScene"),
    pSceneManager_(nullptr)
{
}

void TestScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    Instantiate<Stage>(this);
}

void TestScene::Update()
{
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
