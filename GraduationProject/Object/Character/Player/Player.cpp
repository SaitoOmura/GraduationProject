#include "Player.h"
#include "../../../Utility/Input/InputManager.h"
#include "../../GameObjectManager.h"
#include "../Enemy/EnemyBase.h"
#include "../../Terrain/Terrain_Base.h"
#include <cmath>

// 初期化処理
void Player::Initialize()
{
	collision.object_type = eObjectType::Player;
	collision.collision_type = eCollisionType::Circle;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.hit_object_type.push_back(eObjectType::Terrain);
	collision.is_blocking = true;
	collision.box_size = 36.0f;
	collision.radius = 24.0f;
	dash_period = DASH_DERAY;
	motion_time = 0.0f;
	warp = false;

	GameObjectManager* gm = GameObjectManager::GetInstance();
	for (int i = 0; i < 4; i++)
	{
		bb.push_back(gm->CreateObject<Player_Attack>(0.0f));
		bb[i]->Deactivate();
	}
	flip = false;
	ac = 0;
	now_abi = Ability::normal;
	state = State::idle;
	hp = 10;
}

// 更新処理
void Player::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();
	if (input->GetKeyState(KEY_INPUT_E) == eInputState::Pressed)
	{
		switch (now_abi)
		{
		case normal:
			now_abi = Ability::beam;
			break;
		case beam:
			now_abi = Ability::sword;
			break;
		case sword:
			now_abi = Ability::normal;
			break;
		default:
			break;
		}
	}
	else if (input->GetKeyState(KEY_INPUT_Q) == eInputState::Pressed)
	{
		switch (now_abi)
		{
		case normal:
			now_abi = Ability::sword;
			break;
		case beam:
			now_abi = Ability::normal;
			break;
		case sword:
			now_abi = Ability::beam;
			break;
		default:
			break;
		}
	}
	if (hold && input->GetKeyState(KEY_INPUT_S) == eInputState::Pressed)
	{
		state = State::idle;
		now_abi = hold_enemy;
		hold = false;
	}
	if (hold)
	{
		collision.radius = 26.0f;
	}
	else
	{
		collision.radius = 24.0f;
	}
	if (invisible_time > 0.0f) 
	{
		invisible_time -= delta_second;
		if (invisible_time <= 0.0f)
		{
			invisible_time = 0.0f;
		}
	}
	if (motion_time > 0.0f)
	{
		motion_time -= delta_second;
		if (motion_time <= 0.0f)
		{
			motion_time = 0.0f;
			if (state == State::inhale)
			{
				hold = true;
			}
		}
	}
	Movement(delta_second);
	if (state != State::damage)
	{
		Attack(delta_second);
	}
	if (location.x + velocity.x * delta_second * 140 <= (D_WIN_MAX_X - GAME_WIN_MAX_X) / 2 + collision.radius)
	{
		velocity.x = 0.0f;
	}
	else if (location.x + velocity.x * delta_second * 140 >= ProjectConfig::STAGE_WIDTH)
	{
		velocity.x = 0.0f;
	}
	location += velocity * delta_second * 140;
}

// 描画処理
void Player::Draw(Vector2D c_pos) const
{
	Vector2D size = Vector2D(collision.box_size.x / 2, collision.box_size.y / 2);
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	int color;

	switch (now_abi)
	{
	case normal:
		color = 0xffffff;
		break;
	case beam:
		color = 0xffff00;
		break;
	case sword:
		color = 0x00ff00;
		break;
	default:
		color = 0x00ff00;
		break;
	}
	DrawCircle(d_pos.x, d_pos.y, collision.radius, color, true);
	//DrawBoxAA(d_pos.x - size.x, d_pos.y - size.y, d_pos.x + size.x, d_pos.y + size.y, color, true);
	DrawFormatString(20, 10, 0xffffff, "%f", velocity.y);
}

// 終了時処理
void Player::Finalize()
{

}

void Player::OnHitCollision(GameObject* hit_object)
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
			normal = Vector2D(0.0f ,(local.y >= 0.0f ? 1.0f : -1.0f));
			push = Vector2D(0.0f, normal.y * (dy + rad));
		}

		// 押し出しベクトルをワールド座標に変換
		Vector2D world_push;
		world_push = W_Rotate(push, hit_object->GetAngle());

		// 法線もワールド座標に変換
		Vector2D world_normal;
		world_normal = W_Rotate(normal, hit_object->GetAngle());

		if (dynamic_cast<Terrain_Base*>(hit_object)->GetTerrainType() == TerrainType::throgh)
		{
			if (world_push.y >= 0.0f)
			{
				return;
			}
			if (velocity.y <= 0.0)
			{
				return;
			}
		}
		else if (dynamic_cast<Terrain_Base*>(hit_object)->GetTerrainType() == TerrainType::block)
		{
			if (state == inhale)
			{
				motion_time = 0.2f;
				bb[0]->DeActivate();
				hold_enemy = Ability::normal;
				hit_object->Damage(1);
			}
		}
		location = location + world_push;

		float dot = velocity.x * world_normal.x
			+ velocity.y * world_normal.y;

		if (dot < 0.0f)
		{
			velocity.x -= dot * world_normal.x;
			velocity.y -= dot * world_normal.y;
		}

		if (push.y < -0.01f)
		{
			jump = false;
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::Enemy)
	{
		if (state == State::inhale)
		{
			motion_time = 0.2f;
			bb[0]->DeActivate();
			switch (dynamic_cast<EnemyBase*>(hit_object)->GetEnemyType())
			{
			case EnemyType::e_normal:
				hold_enemy = Ability::normal;
				break;
			case EnemyType::e_beam:
				hold_enemy = Ability::beam;
				break;
			case EnemyType::e_sword:
				hold_enemy = Ability::sword;
				break;
			case EnemyType::e_fire:
				hold_enemy = Ability::fire;
				break;
			default:
				break;
			}
			hit_object->DeActivate();
		}
		else
		{
			if (invisible_time <= 0.0f)
			{
				Damage(hit_object->GetLocation());
				state = State::damage;
				invisible_time = 1.0f;
			}
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::Gate)
	{
		if (dynamic_cast<Gate*>(hit_object)->GetGateType() == GateType::star)
		{
			gate_id = dynamic_cast<Gate*>(hit_object)->GetGateID();
			warp = true;
		}
		InputManager* input = InputManager::GetInstance();
		if (input->GetKeyState(KEY_INPUT_W) == eInputState::Pressed)
		{
			gate_id = dynamic_cast<Gate*>(hit_object)->GetGateID();
			warp = true;
		}
	}
}

const bool Player::IsDash() const
{
	return dash;
}

const bool Player::CheckFlip() const
{
	return flip;
}

const Vector2D Player::GetVelocity() const
{
	return velocity;
}

void Player::SetAbility(Ability ability)
{
	now_abi = ability;
}

Ability Player::GetAbility() const
{
	return now_abi;
}

void Player::SetLife(float life)
{
	hp = life;
}

GateID Player::GetGateId() const
{
	return gate_id;
}

bool Player::IsWarp() const
{
	return warp;
}

void Player::Movement(float delta_second)
{
	InputManager* input = InputManager::GetInstance();
	
	
	if (state != State::walk)
	{
		if (state != State::damage)
		{
			if (flip)
			{
				velocity.x -= delta_second * 4;
				if (velocity.x <= 0.2f)
				{
					velocity.x = 0.0f;
				}
			}
			else
			{
				velocity.x += delta_second * 4;
				if (velocity.x >= -0.2f)
				{
					velocity.x = 0.0f;
				}
			}
		}
		else
		{
			if (velocity.x < 0.0f)
			{
				velocity.x += delta_second * 4;
				if (velocity.x >= -0.2f)
				{
					velocity.x = 0.0f;
					state = State::idle;
				}
			}
			else
			{
				velocity.x -= delta_second * 4;
				if (velocity.x <= 0.2f)
				{
					velocity.x = 0.0f;
					state = State::idle;
				}
			}
		}
	}
	if (!attack && state != State::damage && motion_time <= 0.0f)
	{
		state = State::idle;
		if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
		{
			velocity.x = -1.0;
			state = State::walk;
			dash_period = DASH_DERAY;
			dash_prep = true;
			flip = false;
		}
		if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
		{
			velocity.x = 1.0;
			state = State::walk;
			dash_period = DASH_DERAY;
			dash_prep = true;
			flip = true;
		}
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
			input->GetKeyState(KEY_INPUT_W) == eInputState::Pressed)
		{
			state = State::jump;
			if (jump) 
			{
				if (!hold) 
				{ 
					velocity.y = -3.0f; 
				}
			}
			else
			{
				velocity.y = -4.5f;
				jump = true;
			}
		}
		if (jump)
		{
			dash_prep = false;
			dash = false;
		}
		if (dash_prep)
		{
			if ((input->GetKeyState(KEY_INPUT_A) == eInputState::Pressed && !flip)
				|| (input->GetKeyState(KEY_INPUT_D) == eInputState::Pressed && flip))
			{
				dash = true;
			}
			dash_period -= delta_second;
			if (dash_period <= 0.0f)
			{
				dash_prep = false;
				dash = false;
			}
		}
		if (dash)
		{
			if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold || input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
				velocity.x *= 2;
		}
	}
	velocity.y += GRAVITY * delta_second;
}

// 攻撃処理
void Player::Attack(float delta_second)
{
	InputManager* input = InputManager::GetInstance();
	eInputState inst = input->GetKeyState(KEY_INPUT_J);
	if (motion_time <= 0.0f)
	{
		switch (inst)
		{
		case eInputState::Pressed:
			Attack_Push();
			break;
		case eInputState::Hold:
			Attack_Hold(delta_second);
			break;
		case eInputState::Released:
			state = State::idle;

			attack = false;

			switch (now_abi)
			{
			case normal:
				bb[0]->DeActivate();
				break;
			case beam:
				if (hold_tick >= 0.5f)
				{
					motion_time = 0.5f;
					bb[ac]->Fire();
					ac = 0;
					hold_tick = 0.0f;
				}
				else
				{
					bb[0]->SetAbilityType(now_abi, Ability_Coltype::a_box);
					if (flip)
					{
						bb[0]->Activate(this, flip, 0.5f);
					}
					else
					{
						bb[0]->Activate(this, flip, 0.5f);
					}
					motion_time = 1.0f;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 短押し攻撃
/// </summary>
void Player::Attack_Push()
{
	switch (now_abi)
	{
	case normal:
		if (hold)
		{
			bb[1]->SetAbilityType(Ability::star, Ability_Coltype::a_circle);
			if (flip)
			{
				bb[1]->Activate(this, flip, 3.0f);
			}
			else
			{
				bb[1]->Activate(this, flip, 3.0f);
			}
			bb[1]->Fire();
			hold = false;
			motion_time = 0.5f;
		}
		break;
	case fire:
		break;
	case beam:
		hold_tick = 0.0f;
		break;
	case sword:
		break;
	default:
		break;
	}
}

/// <summary>
/// 長押し攻撃
/// </summary>
/// <param name="delta_second"></param>
void Player::Attack_Hold(float delta_second)
{
	switch (now_abi)
	{
	case normal:
		if (!hold)
		{
			state = State::inhale;
			bb[0]->SetAbilityType(now_abi, Ability_Coltype::a_box);
			if (flip)
			{
				bb[0]->Activate(this, flip, 1.0f);
			}
			else
			{
				bb[0]->Activate(this, flip, 1.0f);
			}
		}
		break;
	case fire:
		break;
	case beam:
		state = State::attack;
		hold_tick += delta_second * 2;
		if (!attack)
		{
			for (int i = 0; i < bb.size(); i++)
			{
				bb[i]->SetAbilityType(now_abi, Ability_Coltype::a_circle);
				if (!bb[i]->IsActive() && hold_tick >= 0.5f)
				{
					if (flip)
					{
						bb[i]->Activate(this, flip, 3.0f);
					}
					else
					{
						bb[i]->Activate(this, flip, 3.0f);
					}
					ac = i;
					attack = true;
					break;
				}
			}
		}
		else
		{
			bb[ac]->Charge(hold_tick, flip);
		}
		break;
	case sword:
		break;
	default:
		break;
	}
}

//アニメーション制御処理
void Player::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void Player::EffectControl(Animation& anim, float delta_second)
{

}