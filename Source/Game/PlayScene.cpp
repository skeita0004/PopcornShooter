#include "PlayScene.hpp"
#include "SceneManager.hpp"
#include "Stage.hpp"
#include "Input.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Player.hpp"
#include <format>

PlayScene::PlayScene(GameObject* _pParent) :
    GameObject(_pParent, "PlayScene"),
    pSceneManager_(nullptr)
{
}

void PlayScene::Init()
{
    hReticle_ = Image::Load("Images/Reticle.png");
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    pStage_ = static_cast<Stage*>(Instantiate<Stage>(this));
    pText_ = new Text();
    pText_->Initialize();
}

void PlayScene::Update()
{
    if (pStage_->PlayerIsDead())
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::OVER);
    }
    else if (pStage_->GetEnemyNum() == 0)
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::CLEAR);
    }
}

void PlayScene::Draw()
{
    static Transform imageTransform{};
    imageTransform.position = XMFLOAT3(1, 1, 0);

    Image::SetTransform(hReticle_, imageTransform);
    Image::Draw(hReticle_);

    Player* pPlayer = static_cast<Player*>(FindObject<Player>("Player"));
    pText_->Draw(500, 50, std::format("Enemy : {}", pStage_->GetEnemyNum()).c_str());
    pText_->Draw(500, 82, std::format("HP : {}", pPlayer->GetHp()).c_str());
    pText_->Draw(500, 114, std::format("GunTemp : {:.2f}", pPlayer->GetGunTemp()).c_str());
    if (pPlayer->GetGunTemp() >= 2.0f)
    {
        pText_->Draw(800, 114, "OverHeat");
    }
}

void PlayScene::Release()
{
}
