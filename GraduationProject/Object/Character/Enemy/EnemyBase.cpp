#include "EnemyBase.h"
#include "../Player/Attack/Player_Attack.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/Camera.h"

// 初期化処理
void EnemyBase::Initialize()
{
	collision.object_type = eObjectType::Enemy;
	collision.is_blocking = true;
	collision.collision_type = eCollisionType::Circle;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.hit_object_type.push_back(eObjectType::Terrain);
	e_state = EnemyState::e_Idle;
	is_live = true;
}

// 更新処理
void EnemyBase::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (!active)
	{
		return;
	}
	if (location.x > camera->GetLocation().x + camera->GetCameraSize().x / 1.5 ||
		location.x < camera->GetLocation().x - camera->GetCameraSize().x / 1.5)
	{
		DeActivate();
	}
	if (!inhaling)
	{
		Movement(delta_second);
	}
	velocity.y += GRAVITY * delta_second;
	location += velocity * delta_second * 140.0f;
}

// 描画処理
void EnemyBase::Draw(Vector2D c_pos) const
{
	__super::Draw(c_pos);

}

// 終了時処理
void EnemyBase::Finalize()
{

}

void EnemyBase::DeActivate()
{
	__super::DeActivate();
	inhaling = false;
	is_live = false;
}

void EnemyBase::OutArea()
{
	is_live = true;
	__super::DeActivate();
	inhaling = false;
	hp = 1;
}

bool EnemyBase::Is_Alive() const
{
	return is_live;
}

void EnemyBase::Activate()
{
	location = init_loc;
	active = true;
}

EnemyType EnemyBase::GetEnemyType() const
{
	return e_type;
}

void EnemyBase::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::Terrain)
	{
		Collision tc = hit_object->GetCollision();
		//矩形の中心点を原点とした円の相対位置
		Vector2D ploc = location - hit_object->GetLocation();

		//矩形の角度を0とした場合の円の相対位置
		Vector2D local = 0.0f;
		local = L_Rotate(ploc, hit_object->GetAngle());

		//clampを使って最近接点の確認
		Vector2D near_loc = 0.0f;

		near_loc.x = Clamp(local.x, tc.box_size.x / 2, -tc.box_size.x / 2);
		near_loc.y = Clamp(local.y, tc.box_size.y / 2, -tc.box_size.y / 2);


		Vector2D diff = local - near_loc;
		float dis = diff.x * diff.x + diff.y * diff.y;

		float rad = collision.radius;

		if (dis >= rad * rad)
		{
			return;
		}

		Vector2D push = 0.0f;
		Vector2D normal = 0.0f;


		float dx = tc.box_size.x / 2 - fabsf(local.x);
		float dy = tc.box_size.y / 2 - fabsf(local.y);

		if (dx < dy)
		{
			normal = Vector2D((local.x >= 0.0f ? 1.0f : -1.0f), 0.0f);
			push = Vector2D(normal.x * (dx + rad), 0.0f);
		}
		else
		{
			normal = Vector2D(0.0f, (local.y >= 0.0f ? 1.0f : -1.0f));
			push = Vector2D(0.0f, normal.y * (dy + rad));
		}

		// 押し出しベクトルをワールド座標に変換
		Vector2D world_push;
		world_push = W_Rotate(push, hit_object->GetAngle());

		// 法線もワールド座標に変換
		Vector2D world_normal;
		world_normal = W_Rotate(normal, hit_object->GetAngle());

		location = location + world_push;

		float dot = velocity.x * world_normal.x
			+ velocity.y * world_normal.y;

		if (dot < 0.0f)
		{
			velocity.x -= dot * world_normal.x;
			velocity.y -= dot * world_normal.y;
		}
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
			break;
		}
		default:
			e_state = e_Damage;
			Damage(hit_object->GetLocation());
			break;
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::Player)
	{
		e_state = e_Damage;
		Damage(hit_object->GetLocation());
	}
}

Vector2D EnemyBase::GetInitLoc() const
{
	return init_loc;
}

void EnemyBase::SetLocation(const Vector2D& location)
{
	__super::SetLocation(location);
	init_loc = location;
}

void EnemyBase::Movement(float delta_second)
{
	if (e_state != EnemyState::e_Move)
	{
		if (flip)
		{
			velocity.x -= delta_second * 4;
			if (velocity.x <= 0.2f) velocity.x = 0.0f;
		}
		else
		{
			velocity.x += delta_second * 4;
			if (velocity.x >= -0.2f) velocity.x = 0.0f;
		}
	}
}

// 攻撃処理
void EnemyBase::Attack(float delta_second)
{

}

//アニメーション制御処理
void EnemyBase::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void EnemyBase::EffectControl(Animation& anim, float delta_second)
{

}