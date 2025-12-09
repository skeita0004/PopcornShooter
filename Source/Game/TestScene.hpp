#pragma once
#include "GameObject.hpp"

//テストシーンを管理するクラス
class TestScene : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TestScene(GameObject* _pParent);

	void Init() override;
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};