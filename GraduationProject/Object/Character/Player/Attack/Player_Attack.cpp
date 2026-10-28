#include "Player_Attack.h"
#include "../../../../Utility/Camera.h"

// ‰Šú‰»ˆ—
void Player_Attack::Initialize()
{
	collision.object_type = eObjectType::Player_Attack;
	collision.is_blocking = true;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.hit_object_type.push_back(eObjectType::Terrain);
	hp = 1;
	life_time = 0.0f;
}

// XVˆ—
void Player_Attack::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (!active)
	{
		return;
	}
	if (fir)
	{
		Movement(delta_second);
		location += velocity * delta_second * 140;
	}
	if (life_time > 0.0f)
	{
		life_time -= delta_second;
		if (a_col == Ability_Coltype::a_box)
		{
			if (flip)
			{
				location.x = player->GetLocation().x + collision.box_size.x / 2;
			}
			else
			{
				location.x = player->GetLocation().x - collision.box_size.x / 2;
			}
			location.y = player->GetLocation().y;
		}
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

// •`‰æˆ—
void Player_Attack::Draw(Vector2D c_pos) const
{
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	Vector2D box = collision.box_size / 2;
	switch (abi)
	{
	case normal:
		DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0xffffaa, 0);
		break;
	case beam:
		if (a_col == Ability_Coltype::a_circle)
		{
			DrawCircle(d_pos.x, d_pos.y, 10 + charge_time * 5, 0xffff00);
		}
		else
		{
			DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0xffff00, 0);
		}
		break;
	case sword:
		break;
	case fire:
		break;
	default:
		DrawCircle(d_pos.x, d_pos.y, collision.radius, 0xffff00);
		break;
		break;
	}


#if _DEBUG


#endif // DEBUG

}

// I—¹Žžˆ—
void Player_Attack::Finalize()
{

}

void Player_Attack::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::Terrain || hit_object->GetCollision().object_type == eObjectType::Enemy)
	{
		switch (abi)
		{
		case normal:
			break;
		case fire:
			break;
		case beam:
			if (a_col == Ability_Coltype::a_circle)
			{
				Deactivate();
			}
			break;
		case sword:
			break;
		case star:
			Deactivate();
			break;
		default:
			break;
		}
	}
}

void Player_Attack::Movement(float delta_second)
{
	if (flip)
	{
		velocity.x = 5.0f;
	}
	else
	{
		velocity.x = -5.0f;
	}
}

void Player_Attack::Deactivate()
{
	active = false;
	location = 0.0f;
	fir = false;
	collision.is_blocking = false;
}

void Player_Attack::Activate(GameObject* p, bool f, float time)
{
	active = true;
	player = p;
	if (abi == Ability::star) location = player->GetLocation();
	flip = f;
	collision.is_blocking = true;
	life_time = time;
}

void Player_Attack::Charge(float time, bool f)
{
	location = player->GetLocation();
	if (f)
	{
		location.x += 20.0f;
	}
	else
	{
		location.x -= 20.0f;
	}
	flip = f;
	if (time >= 2.0f)
	{
		charge_time = 2.0f;
	}
	else
	{
		charge_time = time;
	}
}

void Player_Attack::Fire()
{
	fir = true;
}

void Player_Attack::SetAbilityType(Ability ability, Ability_Coltype abicol)
{
	abi = ability;
	a_col = abicol;
	switch (a_col)
	{
	case a_box:
		collision.box_size = Vector2D(96.0f, 96.0f);
		collision.radius = 0;
		collision.collision_type = eCollisionType::Box;
		break;
	case a_circle:
		collision.box_size = 0.0f;
		collision.radius = 18.0f;
		collision.collision_type = eCollisionType::Circle;
		break;
	default:

		break;
	}
}

Ability Player_Attack::GetAbilityType() const
{
	return abi;
}