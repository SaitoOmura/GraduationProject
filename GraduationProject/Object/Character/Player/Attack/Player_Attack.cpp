#include "Player_Attack.h"

// ‰Šú‰»ˆ—
void Player_Attack::Initialize()
{

}

// XVˆ—
void Player_Attack::Update(float delta_second)
{
	if (!active)
	{
		return;
	}
	if (fire)
	{
		Movement(delta_second);
		location += velocity * delta_second * 140;
	}
	if (location.x >= 1280 || location.x <= 0)
	{
		Deactivate();
	}
}

// •`‰æˆ—
void Player_Attack::Draw(Vector2D c_pos) const
{

	DrawCircle(c_pos.x - location.x, c_pos.y - location.y, 10 + charge_time * 5 , 0xffff00);

#if _DEBUG


#endif // DEBUG

}

// I—¹Žžˆ—
void Player_Attack::Finalize()
{

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
	fire = false;
}

void Player_Attack::Activate(Vector2D loc, float time)
{
	active = true;
	location = loc;
	if (time >= 2.0f)
	{
		charge_time = 2.0f;
	}
	else
	{
		charge_time = time;
	}
}

void Player_Attack::Charge(Vector2D loc, float time, bool f)
{
	location = loc;
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
	fire = true;
}