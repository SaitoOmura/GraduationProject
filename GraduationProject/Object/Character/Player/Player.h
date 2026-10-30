#pragma once

#include "../CharaBase.h"
#include "Attack/Player_Attack.h"
#include "../../Gate/Gate.h"

#define DASH_DERAY (0.2f)

enum State
{
	idle,
	walk,
	attack,
	jump,
	inhale,
	damage
};

class Player : public CharaBase
{
private:
	Ability now_abi;
	State state;
	Ability hold_enemy;
	std::vector<Player_Attack*> bb;
	float dash_period;
	bool dash_prep;
	bool dash;
	bool jump;
	bool attack;
	float hold_tick;
	int ac;
	bool hold;
	float motion_time;
	bool warp;
	GateID gate_id;
	float j_vel;		//ジャンプ直前の移動速度

public:
	Player() :
		now_abi(Ability::normal),
		state(State::idle),
		dash_period(0.0f),
		dash_prep(false),
		dash(false),
		jump(false),
		attack(false),
		hold_tick(0.0f)
	{
	};
	~Player() {};

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

	const bool IsDash() const;

	const bool CheckFlip() const;

	const Vector2D GetVelocity() const;

	virtual void OnHitCollision(GameObject* hit_object) override;

	void SetAbility(Ability ability);

	Ability GetAbility() const;

	void SetLife(float life);

	GateID GetGateId() const;

	bool IsWarp() const;

private:
	/// <summary>
	/// 短押し攻撃
	/// </summary>
	void Attack_Push();

	/// <summary>
	/// 長押し攻撃
	/// </summary>
	void Attack_Hold(float delta_second);

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
	virtual void AnimationControl(Animation& anim, float delta_second);

	/// <summary>
	/// エフェクト制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void EffectControl(Animation& anim, float delta_second);
};