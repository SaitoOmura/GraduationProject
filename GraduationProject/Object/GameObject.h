#pragma once

#include "DxLib.h"
#include "../Utility/Collision.h"
#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"

class GameObject
{
protected:
	Vector2D			location;	//座標
	Vector2D			img_loc;	//見た目上の座標
	Collision			collision;	//当たり判定
	int					image;		//画像情報
	float				hp;			//オブジェクトのHP
	double				angle;		//画像の角度
	double				img_size;	//画像の大きさ
	bool				active;		//使用中かどうか
	int					layer;		//重なり

public:
	//コンストラクタ
	GameObject() :
		location(0.0f),
		collision(),
		image(),
		hp(0.0f),
		angle(0.0),
		img_size(1.0),
		active(true),
		layer(0)
	{

	}
	
	//デストラクタ
	virtual ~GameObject() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(float delta_second);
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(Vector2D c_pos) const;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize();

public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object);

	/// <summary>
	/// 位置座標取得処理
	/// </summary>
	/// <returns>位置座標情報</returns>
	const Vector2D& GetLocation() const;

	/// <summary>
	/// 位置情報変更処理
	/// </summary>
	/// <param name="location">変更したい位置情報</param>
	virtual void SetLocation(const Vector2D& location);

	/// <summary>
	/// 当たり判定取得処理
	/// </summary>
	/// <returns>当たり判定情報</returns>
	const Collision& GetCollision() const;

	/// <summary>
	/// HP取得処理
	/// </summary>
	/// <returns>HPの情報</returns>
	const virtual int GetHP() const;

	/// <summary>
	/// 非アクティブ化
	/// </summary>
	virtual void DeActivate();

	/// <summary>
	/// アクティブ化
	/// </summary>
	/// <param name="Location">アクティブ座標</param>
	virtual void Activate(Vector2D Location);

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">受けたダメージ</param>
	virtual void Damage(float damage);

	/// <summary>
	/// アクティブ状態かどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() const;

	/// <summary>
	/// 角度取得処理
	/// </summary>
	/// <returns>角度</returns>
	const double GetAngle() const;

	/// <summary>
	/// layer取得
	/// </summary>
	/// <returns></returns>
	virtual const int GetLayer() const;
};

