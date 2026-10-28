#include "FlyEnemy.h"

// 初期化処理
void FlyEnemy::Initialize()
{
	active = false;
	flip = false;
	anim_timer = 0.0f;
	collision.radius = 24.0f;
	e_type = EnemyType::e_normal;
	hp = 1;
	__super::Initialize();
}

// 更新処理
void FlyEnemy::Update(float delta_second)
{
	__super::Update(delta_second);
	if (!inhaling)
	{
		Movement(delta_second);
	}
}

// 描画処理
void FlyEnemy::Draw(Vector2D c_pos) const
{
	if (!active)
	{
		return;
	}

	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawCircle(d_pos.x, d_pos.y, collision.radius, 0xffaaaa, true);
}

// 終了時処理
void FlyEnemy::Finalize()
{

}

void FlyEnemy::DeActivate()
{
	__super::DeActivate();
	anim_timer = 0.0f;
}


void FlyEnemy::Movement(float delta_second)
{
	if (e_state != e_Damage)
	{
		if (flip)	velocity.x = 0.5f;
		else velocity.x = -0.5f;
		anim_timer += delta_second;
		velocity.y = sinf(anim_timer * 2.5f) * 1.0f;
	}
}

// 攻撃処理
void FlyEnemy::Attack(float delta_second)
{
	Movement(delta_second);
}

//アニメーション制御処理
void FlyEnemy::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void FlyEnemy::EffectControl(Animation& anim, float delta_second)
{

}