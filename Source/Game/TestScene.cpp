#include "TestScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

void TestScene::Init()
{
    Instantiate<Stage>(this);
}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
