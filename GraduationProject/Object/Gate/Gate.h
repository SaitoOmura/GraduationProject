#pragma once

#include "../GameObject.h"

enum class GateID
{
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5
};

enum class GateType
{
	gate,
	star
};

class Gate : public GameObject
{
private:
	GateID id;
	GateType g_type;
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

	void SetGateType(GateType type);

	GateType GetGateType() const;

public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object) override;

	/// <summary>
	/// 位置座標取得処理
	/// </summary>
	/// <returns>位置座標情報</returns>
	const Vector2D& GetLocation() const;

	/// <summary>
	/// 位置情報変更処理
	/// </summary>
	/// <param name="location">変更したい位置情報</param>
	virtual void SetLocation(const Vector2D& location) override;

	/// <summary>
	/// 当たり判定取得処理
	/// </summary>
	/// <returns>当たり判定情報</returns>
	const Collision& GetCollision() const;

	/// <summary>
	/// 非アクティブ化
	/// </summary>
	virtual void DeActivate() override;

	/// <summary>
	/// アクティブ化
	/// </summary>
	void Activate();

	void SetGateID(GateID gate_id);

	const GateID GetGateID();
};

