#include "Gate.h"

// 初期化処理
void Gate::Initialize()
{
	collision.box_size = 64.0f;
	collision.collision_type = eCollisionType::Box;
	collision.object_type = eObjectType::Gate;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.is_blocking = true;
	hp = 1;
}

// 更新処理
void Gate::Update(float delta_second)
{

}

// 描画処理
void Gate::Draw(Vector2D c_pos) const
{
	if (!active)
	{
		return;
	}

	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	Vector2D box = collision.box_size / 2;
#if _DEBUG
	DrawBoxAA(d_pos.x - box.x, d_pos.y - box.y, d_pos.x + box.x, d_pos.y + box.y, 0x00ddff, true);

#endif // DEBUG

}

// 終了時処理
void Gate::Finalize()
{

}

void Gate::SetGateType(GateType type)
{
	g_type = type;
}

GateType Gate::GetGateType() const
{
	return g_type;
}

// 当たり判定通知処理
void Gate::OnHitCollision(GameObject* hit_object)
{

}

// 位置座標取得処理
const Vector2D& Gate::GetLocation() const
{
	return location;
}

// 位置情報変更処理
void Gate::SetLocation(const Vector2D& location)
{
	this->location = location;
}

// 当たり判定取得処理
const Collision& Gate::GetCollision() const
{
	return collision;
}

// 非アクティブ化
void Gate::DeActivate()
{
	active = false;
}

// アクティブ化
void Gate::Activate()
{
	active = true;
}

void Gate::SetGateID(GateID gate_id)
{
	id = gate_id;
}

const GateID Gate::GetGateID()
{
	return id;
}