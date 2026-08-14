#include "Terrain_Base.h"

// 初期化処理
void Terrain_Base::Initialize()
{
	collision.box_size = 64.0f;
}

// 更新処理
void Terrain_Base::Update(float delta_second)
{

}

// 描画処理
void Terrain_Base::Draw(Vector2D c_pos) const
{

#if _DEBUG
	Vector2D half = location / 2;
	DrawBoxAA(location.x - half.x, location.y - half.y, location.x + half.x, location.y + half.y, 0x88ff44, true);

#endif // DEBUG

}

// 終了時処理
void Terrain_Base::Finalize()
{

}

// 当たり判定通知処理
void Terrain_Base::OnHitCollision(GameObject* hit_object)
{

}