#pragma once
#include "GameObject.hpp"

class TitleScene : public GameObject
{
public:
    TitleScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};