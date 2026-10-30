#include "Terrain_Base.h"
#include "../../Utility/Camera.h"
#include "../GameObjectManager.h"

// 初期化処理
void Terrain_Base::Initialize()
{
	collision.collision_type = eCollisionType::Box;
	collision.box_size = Vector2D(64.0f,64.0f);
	collision.object_type = eObjectType::Terrain;
	collision.is_blocking = false;
	hp = 1;
	DeActivate();
}

// 更新処理
void Terrain_Base::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (location.x - collision.box_size.x / 2 > camera->GetLocation().x + camera->GetCameraSize().x ||
		location.x + collision.box_size.x / 2 < camera->GetLocation().x - camera->GetCameraSize().x)
	{
		DeActivate();
	}
	if (t_type == TerrainType::block)
	{
		location += velocity * delta_second * 140.0f;
	}
}

// 描画処理
void Terrain_Base::Draw(Vector2D c_pos) const
{

#if _DEBUG
	float color;
	switch (t_type)
	{
	case earth:
		color = 0x88ff44;
		break;
	case wall:
		color = 0xc14617;
		break;
	case throgh:
		color = 0xb8953b;
		break;
	case block:
		color = 0xffe500;
		break;
	default:
		break;
	}
	Vector2D half = collision.box_size / 2;
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawBoxAA(d_pos.x - half.x, d_pos.y - half.y, d_pos.x + half.x, d_pos.y + half.y, color, true);

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

void Terrain_Base::SetTerrainType(TerrainType type)
{
	t_type = type;
}

TerrainType Terrain_Base::GetTerrainType() const
{
	return t_type;
}

// 当たり判定通知処理
void Terrain_Base::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::Player_Attack && t_type == TerrainType::block)
	{

		switch (dynamic_cast<Player_Attack*>(hit_object)->GetAbilityType())
		{
		case Ability::normal:
		{
			GameObjectManager* gm = GameObjectManager::GetInstance();
			std::vector<GameObject*> p = gm->GetObjectsList(eObjectType::Player);
			// プレイヤーの口元へ向かう方向
			Vector2D direction = p[0]->GetLocation() - location;

			float length = sqrt(
				direction.x * direction.x +
				direction.y * direction.y
			);

			// 方向を正規化
			direction.x /= length;
			direction.y /= length;

			velocity = direction * 4;
			break;
		}
		default:
			hp--;
			break;
		}
	}
}