#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/Vector2D.h"
#include "GameObject.h"

#include "string"
#include "vector"
#include <typeinfo>

class GameObjectManager : public Singleton<GameObjectManager>
{
private:
	std::vector<GameObject*>	create_objects;		//生成すオブジェクト
	std::vector<GameObject*>	game_objects_list;	//更新するオブジェクトのリスト
	std::vector<GameObject*>	destroy_objects;	//消去するオブジェクト
	std::vector<GameObject*>	player_list;
	std::vector<GameObject*>	enemy_list;
	std::vector<GameObject*>	terrain_list;

public:
	/// <summary>
	/// オブジェクトリストの更新
	/// </summary>
	void CheckCreateObject()
	{
		// 生成するオブジェクトがあれば、オブジェクトリスト内に挿入する
		if (!create_objects.empty())
		{
			for (GameObject* obj : create_objects)
			{
				int layer = obj->GetLayer();
				std::vector<GameObject*>::iterator itr = game_objects_list.begin();	// オブジェクトリストの先頭
				for (; itr != game_objects_list.end(); itr++)
				{
					if (layer < (*itr)->GetLayer()) break;
				}
				// リストの途中に挿入する
				game_objects_list.insert(itr, obj);

				switch (obj->GetCollision().object_type)
				{
				case eObjectType::Player:
					itr = player_list.begin();	// オブジェクトリストの先頭
					// リストの途中に挿入する
					player_list.insert(itr, obj);
					break;
				case eObjectType::Enemy:
					itr = enemy_list.begin();	// オブジェクトリストの先頭
					// リストの途中に挿入する
					enemy_list.insert(itr, obj);
					break;
				case eObjectType::Terrain:
					itr = terrain_list.begin();	// オブジェクトリストの先頭
					// リストの途中に挿入する
					terrain_list.insert(itr, obj);
					break;
				default:

					break;
				}
				
				


			}
			// 生成リストを解放する
			create_objects.clear();
		}
	}

	/// <summary>
	/// デストロイリストの更新
	/// </summary>
	void CheckDestroyObject()
	{
		// 破棄リスト内が空でない場合、リスト内のオブジェクトを破棄する
		if (!destroy_objects.empty())
		{
			for (GameObject* obj : destroy_objects)
			{
				std::vector<GameObject*>::iterator itr = game_objects_list.begin();	// オブジェクトリストの先頭
				// リストの末尾になるまで走査する
				for (; itr != game_objects_list.end(); itr++)
				{
					// リスト内にあれば、削除する
					if ((*itr) == obj)
					{
						game_objects_list.erase(itr);
						obj->Finalize();
						delete obj;
						break;
					}
				}
			}
			// 動的配列の開放
			destroy_objects.clear();
		}
	}

	/// <summary>
	///  オブジェクトリスト取得処理
	/// </summary>
	/// <returns>オブジェクトリスト</returns>
	const std::vector<GameObject*>& GetObjectsList(eObjectType type) const
	{
		switch (type)
		{
		case eObjectType::None:
		default:
			return game_objects_list;
			break;
		case eObjectType::Player:
			return player_list;
			break;
		case eObjectType::Enemy:
			return enemy_list;
			break;
		case eObjectType::Terrain:
			return terrain_list;
			break;
		}
	}

	/// <summary>
	/// ゲームオブジェクト生成処理
	/// </summary>
	/// <typeparam name="OBJECT">生成するゲームオブジェクトクラス</type_param>
	/// <param name="generate_location">生成位置</param>
	/// <returns>生成したゲームオブジェクトのポインタ</returns>
	template <class T>
	T* CreateObject(const Vector2D& generate_location)
	{
		// 生成するゲームオブジェクトクラスを動的確保
		T* new_instance = new T();
		// GameObjectBaseを継承しているか確認
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);
		// エラーチェック
		if (new_object == nullptr)
		{
			// 動的確保したメモリの開放
			delete new_instance;
			// 例外テキストを通知
			throw std::string("ゲームオブジェクトが生成できませんでした");
			return nullptr;
		}

		// オブジェクトの初期化
		new_object->Initialize();
		// 位置情報の設定
		new_object->SetLocation(generate_location);
		// 生成オブジェクトリストの末尾に格納する
		create_objects.push_back(new_object);
		// 生成したゲームオブジェクトのポインタを返す
		return new_instance;
	}

	/// <summary>
	/// オブジェクト破棄処理
	/// </summary>
	/// <param name="target">破棄を行うゲームオブジェクトのポインタ</param>
	void DestroyObject(GameObject* target)
	{
		// ヌルポチェック
		if (target == nullptr)
		{
			return;
		}

		// 破棄オブジェクトリスト内にあれば、追加しない
		for (GameObject* obj : destroy_objects)
		{
			if (obj == target)
			{
				return;
			}
		}

		// 破棄を行うオブジェクトリストに追加する
		destroy_objects.push_back(target);
	}

	/// <summary>
	/// シーン内オブジェクト破棄処理
	/// </summary>
	void DestroyAllObject()
	{
		// オブジェクトリストが空なら処理を終了する
		if (game_objects_list.empty())
		{
			return;
		}
		// オブジェクトリスト内のオブジェクトを削除する
		for (GameObject* obj : game_objects_list)
		{
			obj->Finalize();
			delete obj;
		}
		// 動的配列の解放
		game_objects_list.clear();
		player_list.clear();
		terrain_list.clear();
		enemy_list.clear();
	}
};

