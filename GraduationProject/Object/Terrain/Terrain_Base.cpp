#include "Terrain_Base.h"
#include "../../Utility/Camera.h"

// 初期化処理
void Terrain_Base::Initialize()
{
	collision.collision_type = eCollisionType::Box;
	collision.box_size = Vector2D(64.0f,64.0f);
	collision.object_type = eObjectType::Terrain;
	collision.is_blocking = false;
	DeActivate();
}

// 更新処理
void Terrain_Base::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (location.x > camera->GetLocation().x + camera->GetCameraSize().x / 2 ||
		location.x < camera->GetLocation().x - camera->GetCameraSize().x / 2)
	{
		DeActivate();
	}
}

// 描画処理
void Terrain_Base::Draw(Vector2D c_pos) const
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
void Terrain_Base::Finalize()
{

}

bool Terrain_Base::Check_Loc(Vector2D loc)
{
	return location.x == loc.x;
}

void Terrain_Base::DeActivate()
{
	active = false;
	collision.is_blocking = false;
}

void Terrain_Base::Activate()
{
	active = true;
	collision.is_blocking = true;
}

// 当たり判定通知処理
void Terrain_Base::OnHitCollision(GameObject* hit_object)
{

}