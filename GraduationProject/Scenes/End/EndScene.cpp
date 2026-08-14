#include "EndScene.h"
#include "../../Application.h"

void EndScene::Initialize()
{
	__super::Initialize();
}

eSceneType EndScene::Update(const float& delta_second)
{
	time += delta_second;
	if (time >= 0.2)
	{
		count++;
		if (count >= 3)
		{
			Application* app = Application::GetInstance();
			app->QuitGame();
		}
		time = 0;
	}
	// Œ»İ‚ÌƒV[ƒ“î•ñ‚ğ•Ô‹p‚·‚é
	return GetNowSceneType();
}

void EndScene::Draw() const
{
#ifdef _DEBUG
	DrawFormatString(50, 50, 0xffffff, "End");
#endif
	DrawFormatString(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2, 0xffffff, "Thank You for Playing");
	__super::Draw();
}

void EndScene::Finalize()
{
	__super::Finalize();
}

const eSceneType EndScene::GetNowSceneType() const
{
	return eSceneType::end;
}