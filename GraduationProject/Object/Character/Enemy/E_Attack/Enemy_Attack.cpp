#include "Enemy_Attack.h"
#include "../../../../Utility/Camera.h"
#include "../../../GameObjectManager.h"
#include "../../Player/Attack/Player_Attack.h"

// 初期化処理
void Enemy_Attack::Initialize()
{
	collision.object_type = eObjectType::Enemy_Attack;
	collision.is_blocking = true;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.hit_object_type.push_back(eObjectType::Player_Attack);
	collision.hit_object_type.push_back(eObjectType::Terrain);
	hp = 1;
	life_time = 0.0f;
}

// 更新処理
void Enemy_Attack::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (!active)
	{
		return;
	}
	Movement(delta_second);
	if (life_time > 0.0f)
	{
		life_time -= delta_second;
		if (life_time <= 0.0f)
		{
			life_time = 0.0f;
			Deactivate();
		}
	}
	Vector2D d_pos = camera->GetLocation() - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	if (d_pos.x >= 1280 || d_pos.x <= 0)
	{
		Deactivate();
	}
}

// 描画処理
void Enemy_Attack::Draw(Vector2D c_pos) const
{
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	Vector2D box = collision.box_size / 2;

#if _DEBUG
	switch (a_type)
	{
	case EA_T::a_mush:
		DrawCircle(d_pos.x, d_pos.y, collision.radius, 0xe3b0ab, false);
		break;
	case EA_T::ea_beam:
		DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0xff0000, 0);
		break;
	case EA_T::ea_sword:
		DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0xff0000, 0);
		break;
	case EA_T::ea_fire:
		DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0xff0000, 0);
		break;
	default:
		break;
	}

#endif // DEBUG

}

// 終了時処理
void Enemy_Attack::Finalize()
{

}

void Enemy_Attack::OnHitCollision(GameObject* hit_object)
{
	if (a_type == EA_T::a_mush)
	{
		if (hit_object->GetCollision().object_type == eObjectType::Player)
		{
			hp = 0;
		}
		if (hit_object->GetCollision().object_type == eObjectType::Player_Attack)
		{

			switch (dynamic_cast<Player_Attack*>(hit_object)->GetAbilityType())
			{
			case Ability::normal:
			{
				inhaling = true;
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
			}
			default:
				hp = 0;
				break;
			}
		}
	}
}

void Enemy_Attack::Movement(float delta_second)
{
	switch (a_type)
	{
	case EA_T::a_mush:
		if (!inhaling) velocity.y += GRAVITY * delta_second;
		location += velocity * delta_second * 140;
		break;
	case EA_T::ea_beam:
	case EA_T::ea_sword:
	case EA_T::ea_fire:
		if (flip)
		{
			location.x = enemy->GetLocation().x + collision.box_size.x / 2;
		}
		else
		{
			location.x = enemy->GetLocation().x - collision.box_size.x / 2;
		}
		location.y = enemy->GetLocation().y;
		break;
	default:
		break;
	}
}

void Enemy_Attack::Deactivate()
{
	active = false;
	location = 0.0f;
	collision.is_blocking = false;
}

void Enemy_Attack::Activate(GameObject* target, bool f, float time)
{
	active = true;
	enemy = target;
	location = enemy->GetLocation();
	flip = f;
	collision.is_blocking = true;
	if(a_type == EA_T::a_mush) 	velocity.y = -5.0f;
	life_time = time;
}

void Enemy_Attack::SetAbilityType(EA_T ability)
{
	a_type = ability;
	switch (a_type)
	{
	case EA_T::a_mush:
		collision.box_size = 0.0f;
		collision.radius = 16.0f;
		break;
	case EA_T::ea_beam:
		collision.box_size = Vector2D(20.0f, 36.0f);
		collision.radius = 0.0f;
		break;
	case EA_T::ea_sword:
		collision.box_size = Vector2D(20.0f, 36.0f);
		collision.radius = 0.0f;
		break;
	case EA_T::ea_fire:
		collision.box_size = Vector2D(36.0f, 20.0f);
		collision.radius = 0.0f;
		break;
	default:
		break;
	}
}

EA_T Enemy_Attack::GetAbilityType() const
{
	return a_type;
}