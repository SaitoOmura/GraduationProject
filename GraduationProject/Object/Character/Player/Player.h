#pragma once

#include "../CharaBase.h"
#include "Attack/Player_Attack.h"

#define DASH_DERAY (0.1f)

enum Ability
{
	nomal,
	beam,
	sword
};

enum State
{
	idle,
	walk,
	attack,
	jump
};

class Player : public CharaBase
{
private:
	Ability now_abi;
	State state;
	std::vector<Player_Attack*> bb;
	float dash_period;
	bool dash_prep;
	bool dash;
	bool jump;
	bool attack;
	float hold_tick;
	int ac;

public:
	Player() :
		now_abi(Ability::nomal),
		state(State::idle),
		dash_period(0.0f),
		dash_prep(false),
		dash(false),
		jump(false),
		attack(false),
		hold_tick(0.0f)
	{};
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
	virtual void AnimationControl(Animation& anim, float delta_second);

	/// <summary>
	/// エフェクト制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void EffectControl(Animation& anim, float delta_second);
};