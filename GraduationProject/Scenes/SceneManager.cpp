#include "SceneManager.h"
#include "SceneFactory.h"

// 初期化処理
void SceneManager::Initialize()
{
	// 最初のシーンをタイトル画面にする
	ChangeScene(eSceneType::title);

#ifdef _DEBUG
	ProjectConfig::DEBUG = true;
#endif // _DEBUG
}

//  更新処理
void SceneManager::Update(float delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();
	// デバックモードの切り替え
	if (input->GetKeyState(KEY_INPUT_TAB) == eInputState::Pressed)
	{
		ProjectConfig::DEBUG = !ProjectConfig::DEBUG;
	}

	// シーンの更新
	eSceneType next_scene_type = current_scene->Update(delta_second);

	// オブジェクトマネージャーの情報取得
	GameObjectManager* object = GameObjectManager::GetInstance();

	std::vector<GameObject*> all_list = object->GetObjectsList(eObjectType::None);

	for (int i = 0; i < all_list.size(); i++)
	{
		if (!all_list[i]->IsActive() && all_list[i]->GetCollision().object_type != eObjectType::Terrain)
		{
			continue;
		}
		for (int j = 0; j < all_list.size(); j++)
		{
			if (i == j || !all_list[j]->IsActive())
			{
				continue;
			}
			Collision tc = all_list[i]->GetCollision();
			Collision pc = all_list[j]->GetCollision();
			if (tc.object_type == eObjectType::Terrain)
			{
				if (pc.object_type == eObjectType::Player || 
					pc.object_type == eObjectType::Enemy || 
					pc.object_type == eObjectType::Player_Attack)
				{
					Vector2D dis = all_list[i]->GetLocation() - all_list[j]->GetLocation();
					if (fabsf(dis.x) <= 150.0f && fabs(dis.y) <= 150.0f)
					{
						if (!all_list[i]->IsActive())
						{
							all_list[i]->Activate(all_list[i]->GetLocation());
						}
						CheckCollision(all_list[i], all_list[j]);
					}
					else if (all_list[i]->IsActive())
					{
						all_list[i]->DeActivate();
					}
				}
			}
			if (tc.object_type == eObjectType::Player)
			{
				if (pc.object_type == eObjectType::Enemy ||
					pc.object_type == eObjectType::Enemy_Attack)
				{
					CheckCollision(all_list[i], all_list[j]);
				}
			}
			if (tc.object_type == eObjectType::Player_Attack)
			{
				if (pc.object_type == eObjectType::Enemy ||
					pc.object_type == eObjectType::Enemy_Attack)
				{
					CheckCollision(all_list[i], all_list[j]);
				}
			}
			if (tc.object_type == eObjectType::Gate && pc.object_type == eObjectType::Player)
			{
				CheckCollision(all_list[i], all_list[j]);
			}
		}
	}

	// 描画処理
	Draw();

	// 現在のシーンタイプが次のシーンタイプと違っているか？
	if (current_scene->GetNowSceneType() != next_scene_type)
	{
		// シーン切り替え処理
		ChangeScene(next_scene_type);
	}
}

// 終了時処理
void SceneManager::Finalize()
{
	// シーン情報が生成されていれば、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

// 描画処理
void SceneManager::Draw() const
{
	// 画面の初期化
	ClearDrawScreen();

	// 描画範囲の初期化
	SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);

	// シーンの描画処理
	current_scene->Draw();

	// 裏画面の内容を表画面に反映する
	ScreenFlip();
}

// シーン切り替え処理
void SceneManager::ChangeScene(eSceneType next_type)
{
	// 次のシーンを生成する
	SceneBase* next_scene = SceneFactory::CreateScene(next_type);

	// エラーチェック
	if (next_scene == nullptr)
	{
		throw ("シーンが生成できませんでした\n");
	}

	if (current_scene)
	{
		next_scene->SetStageData(current_scene->GetClear());
	}
	// シーン情報が格納されていたら、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// 次のシーンの初期化
	next_scene->Initialize();

	// 現在シーンの上書き
	current_scene = next_scene;
}

// 当たり判定確認処理
void SceneManager::CheckCollision(GameObject* target, GameObject* partner)
{
	// ヌルポチェック
	if (target == nullptr || partner == nullptr)
	{
		return;
	}

	// 当たり判定情報を取得
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	// 通り抜けられないオブジェクト同士は当たり判定確認処理はしない
	if (tc.is_blocking == false || pc.is_blocking == false)
	{
		return;
	}

	// 当たり判定が有効か確認する
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		if (pc.collision_type == eCollisionType::Box)
		{
			if (tc.collision_type == eCollisionType::Box)
			{
				Vector2D diff = tc.box_size / 2 + pc.box_size / 2;
				Vector2D dis = target->GetLocation() - partner->GetLocation();
				if (fabs(dis.x) < fabs(diff.x) && fabs(dis.y) < fabs(diff.y))
				{
					target->OnHitCollision(partner);
					partner->OnHitCollision(target);
				}

			}
			else
			{
				//矩形の中心点を原点とした円の相対位置
				Vector2D ploc = target->GetLocation() - partner->GetLocation();

				//矩形の角度を0とした場合の円の相対位置
				Vector2D local = 0.0f;
				local = L_Rotate(ploc, partner->GetAngle());

				//clampを使って最近接点の確認
				Vector2D near_loc = 0.0f;

				near_loc.x = Clamp(local.x, pc.box_size.x / 2, -pc.box_size.x / 2);
				near_loc.y = Clamp(local.y, pc.box_size.y / 2, -pc.box_size.y / 2);


				Vector2D diff = local - near_loc;
				float dis = diff.x * diff.x + diff.y * diff.y;

				float rad = tc.radius * tc.radius;

				if (dis > rad)
				{
					return;
				}

				target->OnHitCollision(partner);
				partner->OnHitCollision(target);
			}
		}
		else
		{
			if (tc.collision_type == eCollisionType::Box)
			{
				//矩形の中心点を原点とした円の相対位置
				Vector2D ploc = partner->GetLocation() - target->GetLocation();

				//矩形の角度を0とした場合の円の相対位置
				Vector2D local = 0.0f;
				local = L_Rotate(ploc, target->GetAngle());

				//clampを使って最近接点の確認
				Vector2D near_loc = 0.0f;

				near_loc.x = Clamp(local.x, tc.box_size.x / 2, -tc.box_size.x / 2);
				near_loc.y = Clamp(local.y, tc.box_size.y / 2, -tc.box_size.y / 2);


				Vector2D diff = local - near_loc;
				float dis = diff.x * diff.x + diff.y * diff.y;

				float rad = pc.radius * pc.radius;

				if (dis > rad)
				{
					return;
				}

				target->OnHitCollision(partner);
				partner->OnHitCollision(target);
			}
			else
			{
				//２つのオブジェクトの距離を取得
				Vector2D diff = target->GetLocation() - partner->GetLocation();

				float dis = diff.x * diff.x + diff.y * diff.y;

				//２つのオブジェクトの当たり判定の大きさを取得
				float rad = tc.radius + pc.radius;

				// 円の当たり判定
				if (dis > rad * rad)
				{
					return;
				}
				// 当たっていることを通知する
				target->OnHitCollision(partner);
				partner->OnHitCollision(target);
			}
		}
	}
}

//float SceneManager::Clamp(float value, float max, float min)
//{
//	if (value < min) return min;
//	if (value > max) return max;
//	return value;
//}