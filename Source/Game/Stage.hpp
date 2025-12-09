#pragma once
#include "GameObject.hpp"

class Stage : public GameObject
{
public:
    Stage(GameObject* _pParent);
    ~Stage();

    //初期化
    void Init() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetModelHandle() {return hModel_; }

private:
    int hModel_;
};