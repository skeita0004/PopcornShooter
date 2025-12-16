#pragma once
#include "GameObject.hpp"

class CameraSet;

class Skybox : public GameObject
{
public:
    Skybox(GameObject* _pParent);
    ~Skybox();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    int hModel_;
    CameraSet* pCameraSet_;
};