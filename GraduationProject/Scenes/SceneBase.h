#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Object/GameObjectManager.h"
#include "../Utility/Camera.h"

/// <summary>
/// シーンタイプ
/// </summary>
enum class eSceneType
{
	title,
	in_game,
	re_start,
	result,
	end
};

struct stagedata
{
	int player_life;
	bool clear;
	float timer;
};

class SceneBase
{
protected:
	int	image;
	stagedata data;
	std::vector<int> sounds;

public:
	// コンストラクタ
	SceneBase() : 
		image(),
		data()
	{

	}

	// デストラクタ
	virtual ~SceneBase()
	{
		// 解放忘れ防止
		Finalize();
	}

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize()
	{

	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual eSceneType Update(const float& delta_second)
	{
		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリストの更新
		object->CheckCreateObject();

		// リスト内のオブジェクトを更新する
		for (GameObject* obj : object->GetObjectsList(eObjectType::None))
		{
			if (!obj->IsActive())
			{
				continue;
			}
			obj->Update(delta_second);
		}

		// デストロイリストの更新
		object->CheckDestroyObject();

		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const
	{
		Camera* camera = Camera::GetInstance();
		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリスト内のオブジェクトを描画する
		for (GameObject* obj : object->GetObjectsList(eObjectType::None))
		{
			if (!obj->IsActive())
			{
				continue;
			}
			obj->Draw(camera->GetLocation());
		}
	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		//リソースマネージャーの情報を取得
		ResourceManager* rm = ResourceManager::GetInstance();
		//音源の再生停止
		rm->StopAllSounds();

		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリスト内のオブジェクトを破棄
		object->DestroyAllObject();
		// デストロイリストの更新
		object->CheckDestroyObject();

	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

	virtual void SetStageData(stagedata stgdata)
	{
		data = stgdata;
	}

	virtual const stagedata GetClear() const
	{
		return data;
	}
};

