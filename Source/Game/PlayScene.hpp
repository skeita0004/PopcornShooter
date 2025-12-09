#pragma once
#include "GameObject.hpp"

class PlayScene : public GameObject
{
public:
    PlayScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};