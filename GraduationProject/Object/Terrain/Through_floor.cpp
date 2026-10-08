#include "Through_floor.h"

void Through_floor::Initialize()
{
	collision.collision_type = eCollisionType::Box;
	collision.box_size = Vector2D(64.0f, 32.0f);
	collision.object_type = eObjectType::Terrain;
	collision.is_blocking = false;
	DeActivate();
}

// 更新処理
void Through_floor::Update(float delta_second)
{
	__super::Update(delta_second);
}

// 描画処理
void Through_floor::Draw(Vector2D c_pos) const
{

#if _DEBUG
	Vector2D half = collision.box_size / 2;
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawBoxAA(d_pos.x - half.x, d_pos.y - collision.box_size.y, d_pos.x + half.x, d_pos.y, 0xffff44, true);

#endif // DEBUG

}

// 終了時処理
void Through_floor::Finalize()
{

}

bool Through_floor::Check_Loc(Vector2D loc)
{
	return location.x == loc.x;
}

void Through_floor::DeActivate()
{
	active = false;
	collision.is_blocking = false;
}

void Through_floor::Activate()
{
	active = true;
	collision.is_blocking = true;
}

// 当たり判定通知処理
void Through_floor::OnHitCollision(GameObject* hit_object)
{

}