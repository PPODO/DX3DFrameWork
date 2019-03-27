#include "Weapon.h"

Weapon::Weapon() {
	m_AngleScale = 2.f;
	m_MinRotation = 0.f;
	m_CurrentRotation = 0.f;
	m_bIsActivated = true;
}

Weapon::~Weapon() {
}

bool Weapon::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	D3DXMatrixTranslation(&m_Default, 0.f, 0.f, 0.f);
	return true;
}

void Weapon::Update(float DeltaTime, float ActorHeight) {
	CalculateWeaponAngle();
}

void Weapon::Render(LPD3DXSPRITE Sprite) {
	Sprite->SetTransform(&m_Matrix);
	Actor::Render(Sprite);
	Sprite->SetTransform(&m_Default);
}

void Weapon::CollisionEventBeginByOtherActor(Actor *) {
}

void Weapon::ClearObject() {
}

void Weapon::SetRotation(float Value) {
	m_CurrentRotation = Value;
}

void Weapon::AddRotation(float Value) {
	if (m_MinRotation <= m_CurrentRotation + Value && m_MaxRotation >= m_CurrentRotation + Value) {
		m_CurrentRotation += Value * m_AngleScale;
	}
}

D3DXVECTOR3 Weapon::GetOffsetByAngle(const float & Angle) {
	return D3DXVECTOR3(FLOAT(GetActorImage()->GetRect().bottom * sin(D3DXToRadian(Angle))) * -1, FLOAT(GetActorImage()->GetRect().bottom * cos(D3DXToRadian(Angle))), 0.f) * -1.f;
}