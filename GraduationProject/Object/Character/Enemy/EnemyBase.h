#pragma once

#include "../CharaBase.h"

enum EnemyType
{
	e_normal,
	e_fire,
	e_beam,
	e_sword
};

enum EnemyState
{
	e_Idle,
	e_Damage,
	e_Attack,
	e_Move
};


class EnemyBase : public CharaBase
{
protected:
	EnemyType e_type;
	Vector2D init_loc;
	EnemyState e_state;
	bool inhaling;
	bool is_live;

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

	/// <summary>
	/// 非アクティブ化
	/// </summary>
	virtual void DeActivate() override;


	virtual void Activate();

	virtual EnemyType GetEnemyType() const;

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object) override;

	virtual Vector2D GetInitLoc() const;

	virtual void SetLocation(const Vector2D& location) override;

	virtual void OutArea();

	virtual bool Is_Alive() const;

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

