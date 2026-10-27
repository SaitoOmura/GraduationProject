#include "InGameScene.h"
#include "../../Object/GameObjectManager.h"
#include <fstream>
#include <sstream>
#include <string>

void InGameScene::Initialize()
{
	Camera* camera = Camera::GetInstance();
	GameObjectManager* gm = GameObjectManager::GetInstance();

	Load_Map(data.stage_num);
	Create_Map(data.stage_num);
	Load_Enemy(data.stage_num);
	Create_Enemy();

	player = gm->CreateObject<Player>(0.0f);
	if (data.p_loc.x == 0.0f)
	{
		player->SetLocation(Vector2D(314.0f, 550.0f));
		player->SetAbility(Ability::normal);
	}
	else
	{
		player->SetLocation(data.p_loc);
		player->SetAbility(data.p_ability);
		player->SetLife(data.player_life);
	}
	camera = Camera::GetInstance();
	camera->Initialize();
	camera->SetPlayer(player);
	//if (data.stage_num == 4)
	//{
	//	camera->SetCameraPos(Vector2D(D_WIN_MAX_X / 2,))
	//}
	__super::Initialize();
}

eSceneType InGameScene::Update(const float& delta_second)
{
	Camera* camera = Camera::GetInstance();
	camera->Update(delta_second);
	//for (int i = 0; i < terrain.size(); i++)
	//{
	//	if (terrain[i]->IsActive())
	//	{
	//		continue;
	//	}
	//	if (terrain[i]->GetLocation().x - terrain[i]->GetCollision().box_size.x / 2 <= camera->GetLocation().x + camera->GetCameraSize().x / 1.5 &&
	//		terrain[i]->GetLocation().x + terrain[i]->GetCollision().box_size.x / 2 >= camera->GetLocation().x - camera->GetCameraSize().x / 1.5)
	//	{
	//		terrain[i]->Activate();
	//	}
	//}
	for (int i = 0; i < n_enemy.size(); i++)
	{
		if (n_enemy[i]->IsActive())
		{
			continue;
		}
		if (n_enemy[i]->GetInitLoc().x <= camera->GetLocation().x + camera->GetCameraSize().x / 2 &&
			n_enemy[i]->GetInitLoc().x >= camera->GetLocation().x - camera->GetCameraSize().x / 2)
		{
			if (n_enemy[i]->Is_Alive())
			{
				n_enemy[i]->Activate();
			}
		}
		else
		{
			if (!n_enemy[i]->Is_Alive())
			{
				n_enemy[i]->OutArea();
			}
		}
	}
	for (int i = 0; i < f_enemy.size(); i++)
	{
		if (f_enemy[i]->IsActive())
		{
			continue;
		}
		if (f_enemy[i]->GetInitLoc().x <= camera->GetLocation().x + camera->GetCameraSize().x / 2 &&
			f_enemy[i]->GetInitLoc().x >= camera->GetLocation().x - camera->GetCameraSize().x / 2)
		{
			if (f_enemy[i]->Is_Alive())
			{
				f_enemy[i]->Activate();
			}
		}
	}
	for (int i = 0; i < gate.size(); i++)
	{
		if (gate[i]->IsActive())
		{
			continue;
		}
		if (gate[i]->GetLocation().x - gate[i]->GetCollision().box_size.x / 2 <= camera->GetLocation().x + camera->GetCameraSize().x / 1.5 &&
			gate[i]->GetLocation().x + gate[i]->GetCollision().box_size.x / 2 >= camera->GetLocation().x - camera->GetCameraSize().x / 1.5)
		{
			gate[i]->Activate();
		}
	}
	GameObjectManager* gm = GameObjectManager::GetInstance();
	std::vector<GameObject*> obj_list = gm->GetObjectsList(eObjectType::None);
	for (GameObject* obj : obj_list)
	{
		if (obj->GetHP() <= 0 && obj->IsActive())
		{
			obj->DeActivate();
		}
	}
	if (player->IsWarp())
	{
		switch (player->GetGateId())
		{
		case GateID::Stage1:
			data.p_loc = Vector2D(1294.0f, 576.0f);
			data.stage_num = 1;
			break;
		case GateID::Stage2:
			data.p_loc = Vector2D(634.0f, 320.0f);
			data.stage_num = 2;
			break;
		case GateID::Stage3:
			data.p_loc = Vector2D(314.0f, 550.0f);
			data.stage_num = 3;
			break;
		case GateID::Stage4:
			data.p_loc = Vector2D(442.0f, 1024.0f);
			data.stage_num = 4;
			break;
		default:
			break;
		}
		data.player_life = player->GetHP();
		data.p_ability = player->GetAbility();
		return eSceneType::re_start;
	}
	return __super::Update(delta_second);
}

void InGameScene::Draw() const
{

	__super::Draw();
#ifdef _DEBUG
	Camera* camera = Camera::GetInstance();
	camera->Draw();
	DrawFormatString(50, 50, 0xffffff, "InGame");
	float shift_x = (D_WIN_MAX_X - GAME_WIN_MAX_X) / 2;
	DrawBoxAA(0, 0, shift_x, D_WIN_MAX_Y, 0xdd8822, true);
	DrawBoxAA(D_WIN_MAX_X - shift_x, 0, D_WIN_MAX_X, D_WIN_MAX_Y, 0xdd8822, true);
	for (int i = 0; i < player->GetHP(); i++)
	{
		DrawBoxAA(50, 150 + i * 50, 150, 195 + i * 50, 0x0088ff, true);
	}
	DrawFormatString(50, 50, 0xffffff, "camera:%f", camera->GetLocation().x);
	DrawFormatString(50, 70, 0xffffff, "player:%f", player->GetLocation().x);
#endif




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
	case 2:
		str = "Resource/csv/stage1-2.csv";
		break;
	case 3:
		str = "Resource/csv/stage1-3.csv";
		break;
	case 4:
		str = "Resource/csv/stage1-4.csv";
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
	int t_num = 0;
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
				terrain[t_num]->SetTerrainType(TerrainType::earth);
				t_num++;
				break;
			case 2://斜面
				break;
			case 3://壁
				terrain.push_back(gm->CreateObject<Terrain_Base>(Vector2D(x * 64.0f, y * 64.0f)));
				terrain[t_num]->SetTerrainType(TerrainType::wall);
				t_num++;
				break;
			case 4://通り抜ける壁
				terrain.push_back(gm->CreateObject<Terrain_Base>(Vector2D(x * 64.0f, y * 64.0f)));
				terrain[t_num]->SetTerrainType(TerrainType::throgh);
				t_num++;
				break;
			case 5://ブロック
				terrain.push_back(gm->CreateObject<Terrain_Base>(Vector2D(x * 64.0f, y * 64.0f)));
				terrain[t_num]->SetTerrainType(TerrainType::block);
				t_num++;
				break;
			case 9://扉
				gate.push_back(gm->CreateObject<Gate>(Vector2D(x * 64.0f, y * 64.0f)));
				break;
			case 10://スター
				gate.push_back(gm->CreateObject<Gate>(Vector2D(x * 64.0f, y * 64.0f)));
				break;
			default:
				break;
			}
		}
	}

	float shift_x = (D_WIN_MAX_X - GAME_WIN_MAX_X) / 2;

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
	int i = 0;
	// 1行ずつ読み込み
	while (std::getline(file, line)) {
		// ここで1行分のデータを処理
		std::vector<int> row;
		std::stringstream ss(line);
		std::string val;

		while (std::getline(ss, val, ',')) {
			row.push_back(std::stoi(val));
		}
		if (row[0] == stage)
		{
			switch (row[1])
			{
			case 0:
				gate[i]->SetGateID(GateID::Stage1);
				break;
			case 1:
				gate[i]->SetGateID(GateID::Stage2);
				break;
			case 2:
				gate[i]->SetGateID(GateID::Stage3);
				break;
			case 3:
				gate[i]->SetGateID(GateID::Stage4);
				break;
			default:
				break;
			}
			switch (row[2])
			{
			case 1:
				gate[i]->SetGateType(GateType::gate);
				break;
			case 2:
				gate[i]->SetGateType(GateType::star);
				break;
			default:
				break;
			}
			i++;
		}
	}
}

void InGameScene::Load_Enemy(int stage)
{
	std::string str;
	switch (stage)
	{
	case 1:
	case 2:
	case 3:
	case 4:
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
		if (row[0] == stage)
		{
			enemy_pos.push_back(row);
		}
	}
}

void InGameScene::Create_Enemy()
{
	GameObjectManager* gm = GameObjectManager::GetInstance();
	for (int i = 0; i < enemy_pos.size(); i++)
	{
		switch (enemy_pos[i][1])
		{
		case 1:
			n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			n_enemy[n_enemy.size() - 1]->SetEnemyType(type::ebeam);
			break;
		case 2:
			n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			n_enemy[n_enemy.size() - 1]->SetEnemyType(type::mush);
			break;
		case 3:
			f_enemy.push_back(gm->CreateObject<FlyEnemy>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			break;
		case 4:
			n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			n_enemy[n_enemy.size() - 1]->SetEnemyType(type::piyo);
			break;
		case 5:
			n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			n_enemy[n_enemy.size() - 1]->SetEnemyType(type::enormal);
			break;
		case 6:
			n_enemy.push_back(gm->CreateObject<Normal>(Vector2D(enemy_pos[i][2] * 64.0f, enemy_pos[i][3] * 64.0f)));
			n_enemy[n_enemy.size() - 1]->SetEnemyType(type::esword);
			break;
		default:
			break;
		}
	}

	GameObjectManager* object = GameObjectManager::GetInstance();
	float shift_x = (D_WIN_MAX_X - GAME_WIN_MAX_X) / 2;
	for (GameObject* obj : object->GetCreateList())
	{
		obj->SetLocation(Vector2D(obj->GetLocation().x + shift_x, obj->GetLocation().y));
	}
}