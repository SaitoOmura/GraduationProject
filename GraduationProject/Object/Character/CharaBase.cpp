#include "CharaBase.h"

// 初期化処理
void CharaBase::Initialize()
{

}

// 更新処理
void CharaBase::Update(float delta_second)
{

}

// 描画処理
void CharaBase::Draw(Vector2D c_pos) const
{
	__super::Draw(c_pos);
}

// 終了時処理
void CharaBase::Finalize()
{

}

void CharaBase::DeActivate()
{
	velocity = 0.0f;
	__super::DeActivate();
}

void CharaBase::SetVelocity(Vector2D velo)
{
	velocity = velo;
}

void CharaBase::OnHitCollision(GameObject* hit_object)
{

}

void CharaBase::Movement(float delta_second)
{

}

// 攻撃処理
void CharaBase::Attack(float delta_second)
{

}

//アニメーション制御処理
void CharaBase::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void CharaBase::EffectControl(Animation& anim, float delta_second)
{

}

void CharaBase::Damage(Vector2D target_loc)
{
	float dis = location.x - target_loc.x;
	velocity.y = -2.0f;
	if (dis < 0.0f)
	{
		velocity.x = -2.0f;
	}
	else
	{
		velocity.x = 2.0f;
	}
	hp--;
}