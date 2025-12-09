#pragma once
#include "GameObject.h"

class OverScene : public GameObject
{
public:
    OverScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};