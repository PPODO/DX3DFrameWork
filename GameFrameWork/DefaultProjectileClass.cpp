#include "DefaultProjectileClass.h"

DefaultProjectileClass::DefaultProjectileClass() {
}

bool DefaultProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	ProjectileClass::Init(Device, L"DefaultProjectile.png");

	return true;
}

void DefaultProjectileClass::Update(float DeltaTime) {
	ProjectileClass::Update(DeltaTime);
}

void DefaultProjectileClass::ProjectileMoveProcessing() {
	

}

bool DefaultProjectileClass::CheckColliding() {

	return true;
}