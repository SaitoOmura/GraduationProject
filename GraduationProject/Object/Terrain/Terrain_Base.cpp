#include "Terrain_Base.h"

// 初期化処理
void Terrain_Base::Initialize()
{
	collision.box_size = 64.0f;
	collision.object_type = eObjectType::Terrain;
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
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawBoxAA(d_pos.x - half.x, d_pos.y - half.y, d_pos.x + half.x, d_pos.y + half.y, 0x88ff44, false);

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