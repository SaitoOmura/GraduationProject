#include "TitleScene.h"
#include <chrono>

void TitleScene::Initialize()
{
	__super::Initialize();
}

eSceneType TitleScene::Update(const float& delta_second)
{
	
	InputManager* input = InputManager::GetInstance();
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		return eSceneType::in_game;
	}
	__super::Update(delta_second);

	// Œ»İ‚ÌƒV[ƒ“î•ñ‚ğ•Ô‹p‚·‚é
	return GetNowSceneType();
}

void TitleScene::Draw() const
{
#ifdef _DEBUG
	DrawFormatString(50, 50, 0xffffff, "Title");
#endif
	__super::Draw();
}

void TitleScene::Finalize()
{
	__super::Finalize();
}

const eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::title;
}
