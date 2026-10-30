#include "Normal.h"
#include "../../GameObjectManager.h"

// 初期化処理
void Normal::Initialize()
{
	active = false;
	flip = false;
	collision.radius = 24.0f;
	logic = 0.0f;
	__super::Initialize();
}

// 更新処理
void Normal::Update(float delta_second)
{
	__super::Update(delta_second);

	if (!inhaling)
	{
		if (motion_time > 0.0f)
		{
			motion_time -= delta_second;
			if (motion_time <= 1.5f)
			{
				e_state = EnemyState::e_Idle;
				velocity.x = 0.0f;
				if (motion_time <= 0.0f)
				{
					motion_time = 0.0f;
					if (logic >= 2) logic = 0;
					else logic++;
				}
			}
		}
		else
		{
			switch (logic)
			{
			case 0:
			case 2:
				Movement(delta_second);
				break;
			case 1:
				Attack(delta_second);
				break;
			}
		}
	}
	else
	{
		if (n_type != type::mash)
		{
			if (attack && attack->IsActive())
			{
				attack->Deactivate();
			}
		}
	}
}

// 描画処理
void Normal::Draw(Vector2D c_pos) const
{
	if (!active)
	{
		return;
	}
#ifdef _DEBUG
	float color;
	switch (n_type)
	{
	case enormal:
		color = 0xba6432;
		break;
	case mash:
		color = 0xe3b0ab;
		break;
	case piyo:
		color = 0xffff00;
		break;
	case bear:
		color = 0xe06030;
		break;
	case ebeam:
		color = 0xffff88;
		break;
	case esword:
		color = 0x00896b;
		break;
	case efire:
		color = 0xff4444;
		break;
	default:
		break;
	}
	Vector2D d_pos = c_pos - location;
	d_pos.x = D_WIN_MAX_X / 2 - d_pos.x;
	d_pos.y = D_WIN_MAX_Y / 2 - d_pos.y;
	DrawCircle(d_pos.x, d_pos.y, collision.radius, color, true);
#endif
}

// 終了時処理
void Normal::Finalize()
{

}

void Normal::SetEnemyType(type type)
{
	n_type = type;
	GameObjectManager* gm = GameObjectManager::GetInstance();
	switch (n_type)
	{
	case enormal:
	case piyo:
		hp = 1;
		break;		
	case mash:
		hp = 2;
		attack = gm->CreateObject<Enemy_Attack>(0.0f);
		attack->DeActivate();
		attack->SetAbilityType(EA_T::a_mush);
		break;
	case bear:
		hp = 4;
		break;
	case ebeam:
		hp = 2;
		attack = gm->CreateObject<Enemy_Attack>(0.0f);
		attack->DeActivate();
		attack->SetAbilityType(EA_T::ea_beam);
		break;
	case esword:
		hp = 2;
		attack = gm->CreateObject<Enemy_Attack>(0.0f);
		attack->DeActivate();
		attack->SetAbilityType(EA_T::ea_sword);
		break;
	case efire:
		hp = 2;
		attack = gm->CreateObject<Enemy_Attack>(0.0f);
		attack->DeActivate();
		attack->SetAbilityType(EA_T::ea_fire);
		break;
	default:
		break;
	}
	switch (n_type)
	{
	case enormal:
	case piyo:	
	case mash:
	case bear:
		e_type = EnemyType::e_normal;
		break;
	case ebeam:
		e_type = EnemyType::e_beam;
		break;
	case esword:
		e_type = EnemyType::e_sword;
		break;
	case efire:
		e_type = EnemyType::e_fire;
	default:
		break;
	}
}

void Normal::OutArea()
{	
	__super::OutArea();
	switch (n_type)
	{
	case enormal:
	case piyo:
		hp = 1;
		break;
	case mash:
		hp = 2;
		break;
	case bear:
		hp = 4;
		break;
	case ebeam:
		hp = 2;
		break;
	case esword:
		hp = 2;
		break;
	case efire:
		hp = 2;
		break;
	default:
		break;
	}
}

void Normal::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);
}

void Normal::Movement(float delta_second)
{
	if (e_state != EnemyState::e_Move)
	{
		if (flip)
		{
			velocity.x -= delta_second * 4;
			if (velocity.x <= 0.2f) velocity.x = 0.0f;
		}
		else
		{
			velocity.x += delta_second * 4;
			if (velocity.x >= -0.2f) velocity.x = 0.0f;
		}
	}
	e_state = EnemyState::e_Move;
	switch (n_type)
	{
	case enormal:
		if (flip)	velocity.x = 0.4f;
		else velocity.x = -0.4f;
		break;
	case mash:
		if (flip)	velocity.x = 0.2f;
		else velocity.x = -0.2f;
		break;
	case piyo:
		break;
	case bear:
		break;
	case ebeam:
		if (flip)	velocity.x = 0.4f;
		else velocity.x = -0.4f;
		break;
	case esword:
		break;
	case efire:
		if (flip)	velocity.x = 0.3f;
		else velocity.x = -0.3f;
		break;
	default:
		break;
	}

	motion_time = 3.0f;
}

// 攻撃処理
void Normal::Attack(float delta_second)
{
	e_state = EnemyState::e_Attack;
	switch (n_type)
	{	
	case mash:
		if (attack->GetHP() > 0)
		{
			attack->Activate(this, flip, 1.0f);
		}
		motion_time = 1.5f;
		break;	
	case ebeam:
		attack->Activate(this, flip, 1.5f);
		motion_time = 2.0f;
		break;
	case esword:
		attack->Activate(this, flip, 1.5f);
		motion_time = 2.0f;
		break;
	case efire:
		attack->Activate(this, flip, 1.5f);
		motion_time = 2.0f;
		break;
	case enormal:
	case piyo:
	case bear:
		motion_time = 3.0f;
		break;
	default:
		break;
	}

}

//アニメーション制御処理
void Normal::AnimationControl(Animation& anim, float delta_second)
{

}

// エフェクト制御処理
void Normal::EffectControl(Animation& anim, float delta_second)
{

}