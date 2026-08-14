#include "GameObject.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

// 初期化処理
void GameObject::Initialize()
{

}

// 更新処理
void GameObject::Update(float delta_second)
{

}

// 描画処理
void GameObject::Draw(Vector2D c_pos) const
{
	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(img_loc.x, img_loc.y, img_size, angle, image, TRUE);
	

#if _DEBUG

	if (collision.box_size.x > 0.0f && collision.box_size.y > 0.0f)
	{
		float x = collision.box_size.x;
		float y = collision.box_size.y * 0.5f;
		Vector2D p1[4] = { Vector2D(-x,-y), Vector2D(0,-y) ,Vector2D(0,y) ,Vector2D(-x,y) };
		Vector2D p[4];
		for (int i = 0; i < 4; i++)
		{
			p[i].x = p1[i].x * cosf(angle) - p1[i].y * sinf(angle) + img_loc.x;
			p[i].y = p1[i].x * sinf(angle) + p1[i].y * cosf(angle) + img_loc.y;

		}
		DrawLineAA(p[0].x, p[0].y, p[1].x, p[1].y, 0xffffff);
		DrawLineAA(p[1].x, p[1].y, p[2].x, p[2].y, 0xffffff);
		DrawLineAA(p[2].x, p[2].y, p[3].x, p[3].y, 0xffffff);
		DrawLineAA(p[3].x, p[3].y, p[0].x, p[0].y, 0xffffff);
	}
	else
	{
		DrawCircle(img_loc.x, img_loc.y, collision.radius,
			0xffffff, false);
	}

#endif // DEBUG

}

// 終了時処理
void GameObject::Finalize()
{

}

// 当たり判定通知処理
void GameObject::OnHitCollision(GameObject* hit_object)
{

}

// 位置座標取得処理
const Vector2D& GameObject::GetLocation() const
{
	return location;
}

// 位置情報変更処理
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

// 当たり判定取得処理
const Collision& GameObject::GetCollision() const
{
	return collision;
}

// HP取得処理
const int GameObject::GetHP() const
{
	return this->hp;
}

void GameObject::Stop(bool t)
{
	time_stop = t;
}

void GameObject::Damage(float damage)
{
	hp -= damage;
}

bool GameObject::IsActive() const
{
	return active;
}

const double GameObject::GetAngle() const
{
	return angle;
}

const int GameObject::GetLayer() const
{
	return layer;
}

const bool GameObject::IsStop() const
{
	return time_stop;
}