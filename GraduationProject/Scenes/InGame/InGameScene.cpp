#include "InGameScene.h"
#include "../../Object/GameObjectManager.h"

void InGameScene::Initialize()
{
	Camera* camera = Camera::GetInstance();
	GameObjectManager* gm = GameObjectManager::GetInstance();
	player = gm->CreateObject<Player>(Vector2D(600, 200));
	camera = Camera::GetInstance();
	camera->Initialize();
	camera->SetPlayer(player);
	__super::Initialize();
}

eSceneType InGameScene::Update(const float& delta_second)
{
	Camera* camera = Camera::GetInstance();
	camera->Update(delta_second);
	return __super::Update(delta_second);
}

void InGameScene::Draw() const
{
#ifdef _DEBUG
	Camera* camera = Camera::GetInstance();
	camera->Draw();
	DrawFormatString(50, 50, 0xffffff, "InGame");
	DrawBox(0, GAME_WIN_MAX_Y, D_WIN_MAX_X, D_WIN_MAX_Y, 0xffff88, TRUE);
#endif

	__super::Draw();


}

void InGameScene::Finalize()
{
	GameObjectManager* gm = GameObjectManager::GetInstance();
	gm->DestroyAllObject();
	__super::Finalize();
}

const eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::in_game;
}

void InGameScene::EffectControl(const float& delta_second)
{

}

void InGameScene::AnimationControl(const float& delta_second)
{

}