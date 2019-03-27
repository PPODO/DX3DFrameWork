#include "Cannon.h"

Cannon::Cannon() {
	m_MinRotation = 45.f;
	m_MaxRotation = 90.f;
}

Cannon::~Cannon() {
}

bool Cannon::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Weapon::Init(Device, L"Player/Weapon/Cannon.png");

	return true;
}

void Cannon::Update(float DeltaTime, float ActorHeight) {
	Weapon::Update(DeltaTime, ActorHeight);
}

void Cannon::Render(LPD3DXSPRITE Sprite) {
	Weapon::Render(Sprite);
}

void Cannon::CalculateWeaponAngle() {
	D3DXMatrixTranslation(&m_CenterLocation, -GetActorImage()->GetPosition().x, -(GetActorImage()->GetPosition().y + GetActorImage()->GetImageCenter().y), 0.f);
	D3DXMatrixRotationZ(&m_RotationAngle, D3DXToRadian(m_CurrentRotation));
	D3DXMatrixTranslation(&m_Location, GetActorImage()->GetPosition().x, (GetActorImage()->GetPosition().y + GetActorImage()->GetImageCenter().y), 0.f);
	m_Matrix = m_CenterLocation * m_RotationAngle * m_Location;

	m_Offset = D3DXVECTOR3(FLOAT(GetActorImage()->GetRect().bottom * sin(D3DXToRadian(m_CurrentRotation))), FLOAT(GetActorImage()->GetRect().bottom * cos(D3DXToRadian(m_CurrentRotation))) * -1, 0.f);
}