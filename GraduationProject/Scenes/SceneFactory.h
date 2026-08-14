#pragma once

#include "SceneBase.h"
#include "Title/TitleScene.h"
#include "InGame/InGameScene.h"
#include "Result/ResultScene.h"
#include "End/EndScene.h"

class SceneFactory
{
public:

	/// <summary>
	/// シーン生成処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	/// <returns>生成したシーン情報のポインタ</returns>
	static SceneBase* CreateScene(eSceneType next_type)
	{
		// シーンタイプによって、生成するシーンを切り替える
		switch (next_type)
		{
			// タイトル
		case eSceneType::title:
			return dynamic_cast<SceneBase*>(new TitleScene());
			// インゲーム、リスタート
		case eSceneType::in_game:
		case eSceneType::re_start:
			return dynamic_cast<SceneBase*>(new InGameScene());
			// リザルト
		case eSceneType::result:
			return dynamic_cast<SceneBase*>(new ResultScene());
			// エンド
		case eSceneType::end:
			return dynamic_cast<SceneBase*>(new EndScene());
		default:
			return nullptr;
		}
	}
};