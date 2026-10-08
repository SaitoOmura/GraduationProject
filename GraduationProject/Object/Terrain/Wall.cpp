#include "Wall.h"

void Wall::Initialize()
{
	__super::Initialize();
}

// 更新処理
void Wall::Update(float delta_second)
{
	__super::Update(delta_second);
}

// 描画処理
void Wall::Draw(Vector2D c_pos) const
{

#if _DEBUG
	Vector2D half = collision.box_size / 2;
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawBoxAA(d_pos.x - half.x, d_pos.y - half.y, d_pos.x + half.x, d_pos.y + half.y, 0x88ff44, true);

#endif // DEBUG

}

// 終了時処理
void Wall::Finalize()
{

}

bool Wall::Check_Loc(Vector2D loc)
{
	return location.x == loc.x;
}

void Wall::DeActivate()
{
	active = false;
	collision.is_blocking = false;
}

void Wall::Activate()
{
	active = true;
	collision.is_blocking = true;
}

// 当たり判定通知処理
void Wall::OnHitCollision(GameObject* hit_object)
{

}