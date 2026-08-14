#include "Player_Attack.h"
#include "../../../../Utility/Camera.h"

// ‰Šú‰»ˆ—
void Player_Attack::Initialize()
{

}

// XVˆ—
void Player_Attack::Update(float delta_second)
{
	Camera* camera = Camera::GetInstance();
	if (!active)
	{
		return;
	}
	if (fire)
	{
		Movement(delta_second);
		location += velocity * delta_second * 140;
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
	DrawCircle(d_pos.x, d_pos.y, 10 + charge_time * 5 , 0xffff00);

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