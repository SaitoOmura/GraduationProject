#include "InGameScene.h"
#include "../../Object/GameObjectManager.h"
#include <fstream>
#include <sstream>
#include <string>

void InGameScene::Initialize()
{
	Camera* camera = Camera::GetInstance();
	GameObjectManager* gm = GameObjectManager::GetInstance();
	Load_Map(1);
	Create_Map(1);
	camera = Camera::GetInstance();
	camera->Initialize();
	camera->SetPlayer(player);
	__super::Initialize();
}

eSceneType InGameScene::Update(const float& delta_second)
{
	Camera* camera = Camera::GetInstance();
	camera->Update(delta_second);
	for (int i = 0; i < terrain.size(); i++)
	{
		if (terrain[i]->IsActive())
		{
			continue;
		}
		if (terrain[i]->GetLocation().x <= camera->GetLocation().x + camera->GetCameraSize().x / 2 &&
			terrain[i]->GetLocation().x >= camera->GetLocation().x - camera->GetCameraSize().x / 2)
		{
			terrain[i]->Activate();
		}
	}
	for (int i = 0; i < n_enemy.size(); i++)
	{
		if (n_enemy[i]->IsActive())
		{
			continue;
		}
		if (n_enemy[i]->GetLocation().x <= camera->GetLocation().x + camera->GetCameraSize().x / 2 &&
			n_enemy[i]->GetLocation().x >= camera->GetLocation().x - camera->GetCameraSize().x / 2)
		{
			n_enemy[i]->Activate();
		}
	}
	for (int i = 0; i < f_enemy.size(); i++)
	{
		if (f_enemy[i]->IsActive())
		{
			continue;
		}
		if (f_enemy[i]->GetLocation().x <= camera->GetLocation().x + camera->GetCameraSize().x / 2 &&
			f_enemy[i]->GetLocation().x >= camera->GetLocation().x - camera->GetCameraSize().x / 2)
		{
			f_enemy[i]->Activate();
		}
	}
	return __super::Update(delta_second);
}

void InGameScene::Draw() const
{
#ifdef _DEBUG
	Camera* camera = Camera::GetInstance();
	camera->Draw();
	DrawFormatString(50, 50, 0xffffff, "InGame");
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
		str = "Resource/csv/stage1-1.csv";
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
			row.push_back(std::stoi(val)); // 文字列を1つずつ int に変換
		}
		map_chip.push_back(row);
	}
}

void InGameScene::Create_Map(int stage)
{
	int w = 0;
	int h = 0;
	GameObjectManager* gm = GameObjectManager::GetInstance();
	for (int y = 0; y < map_chip.size(); y++)
	{
		h++;
		for (int x = 0; x < map_chip[y].size(); x++)
		{
			w++;
			switch (map_chip[y][x])
			{
			case 1://地面
				terrain.push_back(gm->CreateObject<Terrain_Base>(Vector2D(x * 64.0f, y * 64.0f)));
				break;
			case 2://斜面
				break;
			case 3://壁
				break;
			case 4://通り抜ける壁
				break;
			case 5://ブロック
				break;
			case 6://プレイヤー
				player = gm->CreateObject<Player>(Vector2D(x * 64.0f, y * 64.0f));
				break;
			case 7://雑魚敵1
				n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(x * 64.0f, y * 64.0f)));
				break;
			case 8://飛ぶ敵
				f_enemy.push_back(gm->CreateObject<FlyEnemy>(Vector2D(x * 64.0f, y * 64.0f)));
				break;
			case 9://扉
				break;
			case 10://スター
				break;
			default:
				break;
			}
		}
	}
	GameObjectManager* object = GameObjectManager::GetInstance();
	float shift_x = (D_WIN_MAX_X - GAME_WIN_MAX_X) / 2;
	for (GameObject* obj : object->GetCreateList())
	{
		obj->SetLocation(Vector2D(obj->GetLocation().x + shift_x, obj->GetLocation().y));
	}

	w /= h;
	ProjectConfig::STAGE_HEIGHT = h * 64.0f;
	ProjectConfig::STAGE_WIDTH = (w * 64.0f) + shift_x;

	std::string str;

	str = "Resource/csv/stage1_gate.csv";

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

		while (std::getline(ss, val, ',')) {
			row.push_back(std::stoi(val));
		}

	}
}

void InGameScene::Load_Enemy(int stage)
{
	std::string str;
	switch (stage)
	{
	case 1:
		str = "Resource/csv/stage1_enemy.csv";
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

		while (std::getline(ss, val, ',')) {
			row.push_back(std::stoi(val)); // 文字列をintに変換
		}
		enemy_pos.push_back(row);
	}
}