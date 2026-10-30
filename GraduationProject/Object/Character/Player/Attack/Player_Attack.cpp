#include "Player_Attack.h"
#include "../../../../Utility/Camera.h"

#define PI	3.141592653589793238462643

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
	location += velocity * delta_second * 140;
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
			Vector2D p1[4] = { Vector2D(-box.x,-box.y), Vector2D(box.x,-box.y) ,Vector2D(box.x,box.y) ,Vector2D(-box.x,box.y) };
			Vector2D p[4];
			for (int i = 0; i < 4; i++)
			{
				p[i].x = p1[i].x * cosf(angle) - p1[i].y * sinf(angle) + d_pos.x;
				p[i].y = p1[i].x * sinf(angle) + p1[i].y * cosf(angle) + d_pos.y;

			}
			DrawLineAA(p[0].x, p[0].y, p[1].x, p[1].y, 0xffff00);
			DrawLineAA(p[1].x, p[1].y, p[2].x, p[2].y, 0xffff00);
			DrawLineAA(p[2].x, p[2].y, p[3].x, p[3].y, 0xffff00);
			DrawLineAA(p[3].x, p[3].y, p[0].x, p[0].y, 0xffff00);
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
	switch (abi)
	{
	case normal:
		if (flip)
		{
			location.x = player->GetLocation().x + collision.box_size.x / 2;
		}
		else
		{
			location.x = player->GetLocation().x - collision.box_size.x / 2;
		}
		location.y = player->GetLocation().y;
		break;
	case fire:
		break;
	case beam:
		switch (a_st)
		{
		case a_normal:
			if (flip)
			{
				location.x = player->GetLocation().x + collision.box_size.x / 2;
				angle += PI / 180 * 0.1f;
			}
			else
			{
				location.x = player->GetLocation().x - collision.box_size.x / 2;
				angle -= PI / 180 * 0.1f;
			}
			location.y = player->GetLocation().y;
			break;
		case a_dash:
			if (life_time >= 1.5f)
			{
				collision.box_size.x += 0.15f;
			}
			else if(life_time <= 0.5f)
			{
				collision.box_size.x -= 0.15f;
			}
			if (flip)
			{
				location.x = player->GetLocation().x + collision.box_size.x / 2;
			}
			else
			{
				location.x = player->GetLocation().x - collision.box_size.x / 2;
			}
			location.y = player->GetLocation().y;
			break;
		case a_djump:
			collision.box_size.x += 0.15f;

			location.x = player->GetLocation().x;
			location.y = player->GetLocation().y;
			break;
		case a_hold:
			if (fir)
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
			break;
		default:
			break;
		}
		break;
	case sword:
		break;
	case star:
		if (fir)
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
		break;
	default:
		break;
	}

}

void Player_Attack::Deactivate()
{
	active = false;
	location = 0.0f;
	fir = false;
	collision.is_blocking = false;
}

void Player_Attack::Activate(GameObject* p, bool f, float time, Ability_State state)
{
	active = true;
	player = p;
	if (abi == Ability::star) location = player->GetLocation();
	flip = f;
	collision.is_blocking = true;
	life_time = time;
	a_st = state;

	switch (abi)
	{
	case normal:
		collision.box_size = Vector2D(96.0f, 96.0f);
		collision.radius = 0.0f;
		break;
	case fire:
		break;
	case beam:
		switch (a_st)
		{
		case a_normal:
			collision.box_size = Vector2D(100.0f, 32.0f);
			collision.radius = 0.0f;
			if (flip) angle = PI / 180 * 120.0f;
			else angle =  PI / 180 * 60.0f;
			break;
		case a_dash:
			collision.box_size = Vector2D(32.0f, 96.0f);
			collision.radius = 0.0f;
			angle = 0.0f;
			break;
		case a_djump:
			collision.box_size = Vector2D(60.0f, 32.0f);
			collision.radius = 0.0f;
			if (flip) angle = PI / 180 * 225.0f;
			else angle = PI / 180 * 315.0f;
			break;
		case a_hold:		
			collision.box_size = 0.0f;
			collision.radius = 18.0f;
			collision.collision_type = eCollisionType::Circle;
			break;
		default:
			break;
		}
		break;
	case sword:
		break;
	case star:
		collision.box_size = 0.0f;
		collision.radius = 18.0f;
		collision.collision_type = eCollisionType::Circle;
		break;
	default:
		break;
	}

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
}

Ability Player_Attack::GetAbilityType() const
{
	return abi;
}