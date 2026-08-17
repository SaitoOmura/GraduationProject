#include "InGameScene.h"
#include "../../Object/GameObjectManager.h"
#include <fstream>
#include <sstream>
#include <string>

void InGameScene::Initialize()
{
	Camera* camera = Camera::GetInstance();
	GameObjectManager* gm = GameObjectManager::GetInstance();
	player = gm->CreateObject<Player>(Vector2D(600, 200));
	camera = Camera::GetInstance();
	camera->Initialize();
	camera->SetPlayer(player);
	Load_Map(1);
	__super::Initialize();
}

eSceneType InGameScene::Update(const float& delta_second)
{
	Camera* camera = Camera::GetInstance();
	camera->Update(delta_second);

	// オブジェクトマネージャーの情報を取得
	GameObjectManager* object = GameObjectManager::GetInstance();
	for (GameObject* obj : object->GetObjectsList(eObjectType::Terrain))
	{
		
	}
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

void InGameScene::Load_Map(int stage)
{
	
	std::string str;
	switch (stage)
	{
	case 1:
		str = "Resource/csv/stage.csv";
		break;
	}
	std::ifstream file(str);
	if (!file.is_open())
	{
		throw (str + "がありません\n");
	}

	std::string line;
	// 1行ずつ読み込み
	while (std::getline(file, line)) {
		// ここで1行分のデータを処理
		std::vector<int> row;
		std::stringstream ss(line);
		std::string val;

		while (std::getline(ss, val,',')) {
			row.push_back(std::stoi(val)); // 文字列をintに変換
		}
		map_chip.push_back(row);
	}
}

void InGameScene::Create_Map()
{

}