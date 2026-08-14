#pragma once

#include "../../../GameObject.h"


class Player_Attack : public GameObject
{
private:
	float charge_time;
	Vector2D velocity;
	bool flip;
	bool fire;

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

	void Activate(Vector2D loc, float time);

	void Charge(Vector2D loc, float time, bool f);

	void Fire();

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	void Movement(float delta_second);
};

