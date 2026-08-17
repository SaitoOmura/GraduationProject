#include "Player.h"
#include "../../../Utility/Input/InputManager.h"
#include "../../GameObjectManager.h"

// 初期化処理
void Player::Initialize()
{
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.hit_object_type.push_back(eObjectType::Terrain);
	collision.is_blocking = true;
	collision.radius = 36.0f;
	dash_period = DASH_DERAY;


	GameObjectManager* gm = GameObjectManager::GetInstance();
	for (int i = 0; i < 4; i++)
	{
		bb.push_back(gm->CreateObject<Player_Attack>(0.0f));
		bb[i]->Deactivate();
	}
	ac = 0;
	now_abi = Ability::nomal;
	state = State::idle;
}

// 更新処理
void Player::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();
	if (input->GetKeyState(KEY_INPUT_E) == eInputState::Pressed)
	{
		switch (now_abi)
		{
		case nomal:
			now_abi = Ability::beam;
			break;
		case beam:
			now_abi = Ability::sword;
			break;
		case sword:
			now_abi = Ability::nomal;
			break;
		default:
			break;
		}
	}
	else if (input->GetKeyState(KEY_INPUT_Q) == eInputState::Pressed)
	{
		switch (now_abi)
		{
		case nomal:
			now_abi = Ability::sword;
			break;
		case beam:
			now_abi = Ability::nomal;
			break;
		case sword:
			now_abi = Ability::beam;
			break;
		default:
			break;
		}
	}

	Movement(delta_second);
	Attack(delta_second);
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
	case nomal:
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
	DrawFormatString(20, 10, 0xffffff, "%f", velocity.x);
}

// 終了時処理
void Player::Finalize()
{

}

void Player::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::Terrain)
	{
		Vector2D dis = location - hit_object->GetLocation();
		float diff = collision.radius + hit_object->GetCollision().box_size.x;
		if (dis.x - diff <= 0.0f)
		{
			location.x += dis.x - diff;
		}
		if (dis.y - diff <= 0.0f)
		{
			location.y += dis.y - diff;
			jump = false;
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

void Player::Movement(float delta_second)
{
	InputManager* input = InputManager::GetInstance();
	
	if (state == State::idle)
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
	if (!attack)
	{
		if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
		{
			velocity.x = -1.0;

			dash_period = DASH_DERAY;
			dash_prep = true;
			flip = false;
		}
		if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
		{
			velocity.x = 1.0;
			dash_period = DASH_DERAY;
			dash_prep = true;
			flip = true;
		}
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
			input->GetKeyState(KEY_INPUT_W) == eInputState::Pressed)
		{
			if (jump) velocity.y = -2.0f;
			else
			{
				velocity.y = -4.0f;
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
	if (input->GetKeyState(KEY_INPUT_J) == eInputState::Hold)
	{
		state = State::attack;
		switch (now_abi)
		{
		case nomal:
			break;
		case beam:
			hold_tick += delta_second * 2;
			if (!attack)
			{
				for (int i = 0; i < bb.size(); i++)
				{
					if (!bb[i]->IsActive())
					{
						if (flip)
						{
							bb[i]->Activate(Vector2D(location.x + 20,location.y), hold_tick);
						}
						else
						{
							bb[i]->Activate(Vector2D(location.x - 20, location.y), hold_tick);
						}
						ac = i;
						break;
					}
				}
			}
			else
			{
				bb[ac]->Charge(Vector2D(location.x, location.y), hold_tick,flip);
			}
			break;
		default:
			break;
		}
		attack = true;
	}
	if (input->GetKeyState(KEY_INPUT_J) == eInputState::Released)
	{

		state = State::idle;
		attack = false;
		switch (now_abi)
		{
		case nomal:
			break;
		case beam:
			bb[ac]->Fire();
			ac = 0;
			hold_tick = 0.0f;
			break;
		default:
			break;
		}
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