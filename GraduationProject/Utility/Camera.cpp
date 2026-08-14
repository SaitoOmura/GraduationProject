#include "Camera.h"


// 初期化処理
void Camera::Initialize()
{


	// スクリーンの初期サイズを設定
	this->size.x = D_WIN_MAX_X;
	this->size.y = D_WIN_MAX_Y;

	// カメラの初期座標を設定
	this->location.x = 0  + (size.x / 2);
	this->location.y = size.y / 2;

}

// 更新処理
void Camera::Update(float delta_second)
{
	Scroll(delta_second);
}

// 描画処理
void Camera::Draw()
{
	DrawFormatString(0, 100, 0xffffff, "カメラ座標: %f", location.x);
	DrawFormatString(0, 120, 0xffffff, "プレイヤー座標: %f", player->GetLocation().x);
}

void Camera::Scroll(float delta_second)
{
	tracking_flag = false;
	if (player->CheckFlip())
	{
		if (player->GetLocation().x >= location.x && location.x + size.x / 2 < ProjectConfig::STAGE_WIDTH)
		{
			tracking_flag = true;
			if (tracking_speed <= player->GetVelocity().x)
			{
				tracking_speed += delta_second * 3;
			}
		}
	}
	else
	{
		if (player->GetLocation().x <= location.x - size.x / 2 + 100 && location.x - size.x / 2 > 0)
		{
			tracking_flag = true;
			if (tracking_speed >= player->GetVelocity().x)
			{
				tracking_speed -= delta_second * 3;
			}
		}
	}
	if (!tracking_flag)
	{
		flame += delta_second;
		if (flame >= 1.0f)
		{
			tracking_speed *= 0.9f;
			if (tracking_speed > 0.0f)
			{
				if (tracking_speed <= 0.01f) tracking_speed = 0.0f;
			}
			else if (tracking_speed < 0.0f)
			{
				if (tracking_speed >= -0.01f) tracking_speed = 0.0f;
			}
		}
	}
	location.x += tracking_speed * delta_second * 140;
}

// カメラ座標設定処理
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}

// プレイヤー情報設定処理
void Camera::SetPlayer(Player* p)
{
	player = p;
}

const Vector2D Camera::GetLocation() const
{
	return location;
}