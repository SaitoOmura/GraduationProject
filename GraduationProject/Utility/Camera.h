#pragma once

#include "Singleton.h"
#include "Vector2D.h"
#include "../Object/Character/Player/Player.h"

class Camera:public Singleton<Camera>
{
private:
	Vector2D location;
	Vector2D size;
	Player* player;
	bool tracking_flag;
	float tracking_speed;
	float flame;

public:
	Camera() :
		location(0.0f),
		size(0.0f),
		player(),
		tracking_flag(false)
	{};

	~Camera() {};

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	///  実行処理
	/// </summary>
	void Update(float delta_second);

	/// <summary>
	/// スクリーン描画
	/// </summary>
	/// <param name="back_buffer">表示する画面</param>
	void Draw();

	/// <summary>
	/// スクロール処理
	/// </summary>
	void Scroll(float delta_second);

	/// <summary>
	///	カメラ座標指定処理
	/// </summary>
	/// <param name="location">指定座標</param>
	void SetCameraPos(Vector2D location);

	/// <summary>
	/// 巫女情報設定処理
	/// </summary>
	/// <param name="Player">プレイヤーポイント</param>
	void SetPlayer(Player* p);

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns>カメラ中心座標</returns>
	const Vector2D GetLocation() const;

	/// <summary>
	/// 画面サイズ
	/// </summary>
	/// <returns>カメラの画面の大きさ</returns>
	const Vector2D GetCameraSize() const;
};

