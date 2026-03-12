#include "ClearScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "Image.hpp"

ClearScene::ClearScene(GameObject* _pParent) :
    GameObject(_pParent, "ClearScene"),
    pSceneManager_(nullptr),
    hImage_(-1),
    hReturnToTilteImage_(-1)
{
}

void ClearScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    hImage_ = Image::Load("Images/clear.png");
    hReturnToTilteImage_ = Image::Load("Images/returntotitle1.png");

}

void ClearScene::Update()
{
    static Transform returnTotitleImageT{};
    returnTotitleImageT.position.y = -0.4f;
    returnTotitleImageT.scale = XMFLOAT3(0.7f, 0.7f, 1.0f);
    Image::SetTransform(hReturnToTilteImage_, returnTotitleImageT);
    if (Input::IsKeyDown(DIK_SPACE) or Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::TITLE);
    }
}

void ClearScene::Draw()
{
    Image::Draw(hImage_);
    Image::Draw(hReturnToTilteImage_);
}

void ClearScene::Release()
{
}
