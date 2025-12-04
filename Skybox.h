#pragma once
#include "Engine/GameObject.h"

class Skybox : public GameObject
{
public:
    Skybox(GameObject* _pParent);
    ~Skybox();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

private:
    int hModel_;
};