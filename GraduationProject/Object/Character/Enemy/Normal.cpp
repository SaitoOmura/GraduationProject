#include "Normal.h"

// 初期化処理
void Normal::Initialize()
{
	active = false;
	flip = false;
	collision.radius = 24.0f;
	__super::Initialize();
}

// 更新処理
void Normal::Update(float delta_second)
{
	__super::Update(delta_second);
}

// 描画処理
void Normal::Draw(Vector2D c_pos) const
{
	if (!active)
	{
		return;
	}
#ifdef _DEBUG
	float color;
	switch (n_type)
	{
	case enormal:
		color = 0xba6432;
		break;
	case mush:
		color = 0xe3b0ab;
		break;
	case piyo:
		color = 0xffff00;
		break;
	case bear:
		color = 0xe06030;
		break;
	case ebeam:
		color = 0xffff88;
		break;
	case esword:
		color = 0x00896b;
		break;
	case efire:
		color = 0xff0000;
		break;
	default:
		break;
	}
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawCircle(d_pos.x, d_pos.y, collision.radius, color, true);
#endif
}

// 終了時処理
void Normal::Finalize()
{

}

void Normal::SetEnemyType(type type)
{
	n_type = type;
	switch (n_type)
	{
	case enormal:
	case piyo:
		hp = 1;
		break;		
	case mush:
		hp = 2;
		break;
	case bear:
		hp = 4;
		break;
	case ebeam:
		hp = 2;
		break;
	case esword:
		hp = 2;
		break;
	case efire:
		hp = 2;
		break;
	default:
		break;
	}
	switch (n_type)
	{
	case enormal:
	case piyo:	
	case mush:
	case bear:
		e_type = EnemyType::e_normal;
		break;
	case ebeam:
		e_type = EnemyType::e_beam;
		break;
	case esword:
		e_type = EnemyType::e_sword;
		break;
	case efire:
		e_type = EnemyType::e_fire;
	default:
		break;
	}
}

void Normal::OutArea()
{	
	__super::OutArea();
	switch (n_type)
	{
	case enormal:
	case piyo:
		hp = 1;
		break;
	case mush:
		hp = 2;
		break;
	case bear:
		hp = 4;
		break;
	case ebeam:
		hp = 2;
		break;
	case esword:
		hp = 2;
		break;
	case efire:
		hp = 2;
		break;
	default:
		break;
	}
}

void Normal::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);
}

void Normal::Movement(float delta_second)
{
	//if (flip)	velocity.x = 1.0f;
	//else velocity.x = -1.0f;
}

// 攻撃処理
void Normal::Attack(float delta_second)
{

}

//アニメーション制御処理
void Normal::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void Normal::EffectControl(Animation& anim, float delta_second)
{

}