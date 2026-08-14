#include "ResultScene.h"

void ResultScene::Initialize()
{

	__super::Initialize();
}

eSceneType ResultScene::Update(const float& delta_second)
{

	// Œ»İ‚ÌƒV[ƒ“î•ñ‚ğ•Ô‹p‚·‚é
	return GetNowSceneType();
}

void ResultScene::Draw() const
{
#ifdef _DEBUG
	DrawFormatString(50, 50, 0xffffff, "Result");
#endif

	__super::Draw();
}

void ResultScene::Finalize()
{
	__super::Finalize();
}

const eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::result;
}