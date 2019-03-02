#include "DefaultEnemy.h"
#include "ProjectileClass.h"
#include <string>

DefaultEnemy::DefaultEnemy() {
	m_Name = std::string("DefaultEnemy");
	m_XMoveSpeed = 5.f;
	m_MinSpawnDelay = 2.f;
	m_MaxSpawnDelay = 5.f;
}

bool DefaultEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"DefaultEnemy.png");

	return true;
}

void DefaultEnemy::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);
}

void DefaultEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
}

void DefaultEnemy::Destroy() {
	EnemyClass::Destroy();

}

void DefaultEnemy::EnemyMoveProcessing() {
	m_Texture->AddXPosition(m_XMoveSpeed * -1);
}