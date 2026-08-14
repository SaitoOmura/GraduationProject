#pragma once

#include "../CharaBase.h"

enum EnemyType
{
	nomal,
	fly,
	fire,
	beam,
	sword
};

class EnemyBase : public CharaBase
{
private:
	EnemyType e_type;

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(float delta_second) override;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(Vector2D c_pos) const override;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize() override;

	void SetEnemyType(EnemyType t);

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void Movement(float delta_second) override;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Attack(float delta_second) override;

	/// <summary>
	/// アニメーション制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void AnimationControl(Animation& anim, float delta_second) override;
	/// <summary>
	/// エフェクト制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void EffectControl(Animation& anim, float delta_second) override;
};

