#include "EnemyBase.h"

// 初期化処理
void EnemyBase::Initialize()
{

}

// 更新処理
void EnemyBase::Update(float delta_second)
{
	if (!active)
	{
		return;
	}
	Movement(delta_second);
	location = velocity * delta_second * 140.0f;
}

// 描画処理
void EnemyBase::Draw(Vector2D c_pos) const
{
	__super::Draw(c_pos);
}

// 終了時処理
void EnemyBase::Finalize()
{

}

void EnemyBase::SetEnemyType(EnemyType t)
{
	e_type = t;
}

void EnemyBase::Movement(float delta_second)
{

}

// 攻撃処理
void EnemyBase::Attack(float delta_second)
{

}

//アニメーション制御処理
void EnemyBase::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void EnemyBase::EffectControl(Animation& anim, float delta_second)
{

}