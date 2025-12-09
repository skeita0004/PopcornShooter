#pragma once
#include "GameObject.hpp"

class Skybox : public GameObject
{
public:
    Skybox(GameObject* _pParent);
    ~Skybox();

    //初期化
    void Init() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

private:
    int hModel_;
};