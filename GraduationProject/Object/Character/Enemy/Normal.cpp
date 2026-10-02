#include "Normal.h"

// 初期化処理
void Normal::Initialize()
{
	active = false;
	flip = false;
	rad = 36.0f;
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

	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawCircle(d_pos.x, d_pos.y, rad, 0xffff88, true);
}

// 終了時処理
void Normal::Finalize()
{

}

void Normal::Movement(float delta_second)
{
	if (flip)	velocity.x = 1.0f;
	else velocity.x = -1.0f;
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