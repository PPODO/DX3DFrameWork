#include "DefaultProjectileClass.h"
#include "TextureClass.h"

DefaultProjectileClass::DefaultProjectileClass() {
	m_Name = "DefaultProjectile";
}

bool DefaultProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	ProjectileClass::Init(Device, L"DefaultProjectile.png");

	return true;
}

void DefaultProjectileClass::Update(float DeltaTime) {
	ProjectileClass::Update(DeltaTime);
}

void DefaultProjectileClass::ProjectileMoveProcessing() {
	m_Texture->AddPosition(m_MoveDirection * 15.f);

}