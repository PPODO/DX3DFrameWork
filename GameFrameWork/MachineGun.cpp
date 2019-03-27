#include "MachineGun.h"

MachineGun::MachineGun() {
	m_AngleScale = 2.f;
	m_MinRotation = 0.f;
	m_CurrentRotation = 0.f;
	m_MaxRotation = 60.f;
}

MachineGun::~MachineGun() {
}

bool MachineGun::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Weapon::Init(Device, L"Player/Weapon/MachineGun.png");

	return true;
}

void MachineGun::Update(float DeltaTime, float ActorHeight) {
	Weapon::Update(DeltaTime, ActorHeight);
}

void MachineGun::Render(LPD3DXSPRITE Sprite) {
	Weapon::Render(Sprite);
}

void MachineGun::CalculateWeaponAngle() {
	D3DXVECTOR3 CenterLocation = GetActorImage()->GetPosition() * -1;
	D3DXMatrixTranslation(&m_CenterLocation, CenterLocation.x, CenterLocation.y - (GetActorImage()->GetImageCenter().y / 2), 0.f);
	D3DXMatrixRotationZ(&m_RotationAngle, D3DXToRadian(m_CurrentRotation));
	D3DXMatrixTranslation(&m_Location, CenterLocation.x * -1, (CenterLocation.y * -1), 0.f);
	m_Matrix = m_CenterLocation * m_RotationAngle * m_Location;
	
	m_Offset = D3DXVECTOR3(FLOAT(GetActorImage()->GetRect().bottom * sin(D3DXToRadian(m_CurrentRotation))) * -1, FLOAT(GetActorImage()->GetRect().bottom * cos(D3DXToRadian(m_CurrentRotation))), 0.f) * -1.f;
}