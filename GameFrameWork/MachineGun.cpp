#include "MachineGun.h"

MachineGun::MachineGun() {
	m_AngleScale = 2.f;
	m_MinRotation = 0.f;
	m_CurrentRotation = 0.f;
	m_MaxRotation = 30.f;
}

MachineGun::~MachineGun() {
}

bool MachineGun::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Weapon::Init(Device, L"Player/Weapon/MachineGun.png");

	D3DXMatrixTranslation(&m_Default, 0.f, 0.f, 0.f);
	return true;
}

void MachineGun::Update(float DeltaTime) {
	Weapon::Update(DeltaTime);

	CalculateWeaponAngle();
}

void MachineGun::Render(LPD3DXSPRITE Sprite) {
	Sprite->SetTransform(&m_Rotation);
	Weapon::Render(Sprite);
	Sprite->SetTransform(&m_Default);
}

void MachineGun::SetRotation(float Value) {
	if (m_MinRotation <= m_CurrentRotation + Value && m_MaxRotation >= m_CurrentRotation + Value) {
		m_CurrentRotation += Value * m_AngleScale;
	}
}

void MachineGun::CalculateWeaponAngle() {
	D3DXVECTOR3 CenterLocation = GetActorImage()->GetPosition() * -1;
	D3DXMatrixTranslation(&m_ZeroPoint, CenterLocation.x, CenterLocation.y - GetActorImage()->GetImageCenter().y, 0.f);
	D3DXMatrixRotationZ(&m_RotationAngle, D3DXToRadian(m_CurrentRotation));
	D3DXMatrixTranslation(&m_Location, CenterLocation.x * -1, (CenterLocation.y * -1) + GetActorImage()->GetImageCenter().y, 0.f);
	m_Rotation = m_ZeroPoint * m_RotationAngle * m_Location;

	m_Offset = D3DXVECTOR3(FLOAT(cos(D3DXToRadian(m_CurrentRotation + 90)) * GetActorImage()->GetRect().bottom), FLOAT(GetActorImage()->GetRect().bottom) + GetActorImage()->GetImageCenter().y + FLOAT(sin(D3DXToRadian(m_CurrentRotation * -1)) * (GetActorImage()->GetRect().bottom / 2)), 0.f) * -1;
}