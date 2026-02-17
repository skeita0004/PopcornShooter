#pragma once
#include "GameObject.hpp"

class SceneManager;

class Reticle : public GameObject
{
public:
    Reticle(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    int hReticle_;
};