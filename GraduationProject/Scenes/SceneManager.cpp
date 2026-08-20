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
	// オブジェクトリストを取得
	std::vector<GameObject*> target_list = object->GetObjectsList(eObjectType::Terrain);
	std::vector<GameObject*> partner_list = object->GetObjectsList(eObjectType::Player);

	// 当たり判定確認処理
	for (int i = 0; i < target_list.size(); i++)
	{
		if (!target_list[i]->IsActive())
		{
			continue;
		}
		for (int j = 0; j < partner_list.size(); j++)
		{
			if (!partner_list[j]->IsActive())
			{
				continue;
			}
 			CheckCollision(target_list[i], partner_list[j]);
		}
	}

	//target_list = object->GetObjectsList(eObjectType::Terrain);
	//partner_list = object->GetObjectsList(eObjectType::Enemy);

	//for (int i = 0; i < target_list.size(); i++)
	//{
	//	if (!target_list[i]->IsActive())
	//	{
	//		continue;
	//	}
	//	for (int j = 0; j < partner_list.size(); j++)
	//	{
	//		if (!partner_list[j]->IsActive())
	//		{
	//			continue;
	//		}
	//		CheckCollision(target_list[i], partner_list[j]);
	//	}
	//}

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
		if (pc.box_size.x > 0.0f && pc.box_size.y > 0.0f)
		{
			if (tc.box_size.x > 0.0f && tc.box_size.y / 0.0f)
			{
				Vector2D dis = target->GetLocation() - partner->GetLocation();
				Vector2D diff = tc.box_size / 2 + pc.box_size / 2;
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
				local.x = ploc.x * cosf(-partner->GetAngle()) - ploc.y * sinf(-partner->GetAngle());
				local.y = ploc.x * sinf(-partner->GetAngle()) + ploc.y * cosf(-partner->GetAngle());

				//clampを使って最近接点の確認
				Vector2D near_loc = 0.0f;
				if (current_scene->GetNowSceneType() == eSceneType::title)
				{
					near_loc.x = Clamp(local.x, pc.box_size.x / 2, -pc.box_size.x / 2);
					near_loc.y = Clamp(local.y, pc.box_size.y / 2, -pc.box_size.y / 2);
				}
				else
				{
					near_loc.x = Clamp(local.x, pc.box_size.x / 2, -pc.box_size.x / 2);
					near_loc.y = Clamp(local.y, pc.box_size.y / 2, -pc.box_size.y / 2);
				}

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

float SceneManager::Clamp(float value, float max, float min)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}