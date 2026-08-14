#pragma once

#include "../GameObject.h"

#define GRAVITY (9.81)

struct Animation
{
	float timer;
	int	count;
	int max_count;
	bool active;
};

class CharaBase : public GameObject
{
protected:
	Vector2D			velocity;		//移動量
	std::vector<int>	effect_anim;	//エフェクト
	std::vector<int>	animation;		//アニメーション
	std::vector<int>	sounds;			//効果音
	bool				flip;			//反転

public:
	CharaBase() :
		velocity(),
		effect_anim(),
		animation(),
		sounds(),
		flip(false)
	{};

	~CharaBase() {};

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

	virtual void SetVelocity(Vector2D velo);

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void Movement(float delta_second);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Attack(float delta_second);

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

