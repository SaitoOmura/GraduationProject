#pragma once

#include "../../../GameObject.h"

enum Ability
{
	normal,
	fire,
	beam,
	sword,
	star
};

enum Ability_Coltype
{
	a_box,
	a_circle
};

class Player_Attack : public GameObject
{
private:
	Ability abi;
	Ability_Coltype a_col;
	float charge_time;
	Vector2D velocity;
	bool flip;
	bool fir;
	float life_time;
	GameObject* player;

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

	void Deactivate();

	void Activate(GameObject* p, bool f, float time);

	void Charge( float time, bool f);

	void Fire();

	void SetAbilityType(Ability ability, Ability_Coltype abicol);

	Ability GetAbilityType() const;

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object) override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	void Movement(float delta_second);
};

