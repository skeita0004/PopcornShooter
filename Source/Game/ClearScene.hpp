#pragma once
#include "GameObject.h"

class ClearScene : public GameObject
{
public:
    ClearScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};