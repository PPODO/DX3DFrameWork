#include "DefaultEnemyClass.h"

DefaultEnemyClass::DefaultEnemyClass() {
	m_Name = "DefaultEnemy";
	m_ProjectileDirection = D3DXVECTOR3(-1.f, 0.f, 0.f);
	m_MinSpawnDelay = 0.f;
	m_MaxSpawnDelay = 2.f;
	m_XMoveSpeed = 7.5f;
}

DefaultEnemyClass::~DefaultEnemyClass() {
}

bool DefaultEnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"DefaultEnemy.png");

	return true;
}

void DefaultEnemyClass::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);

}

void DefaultEnemyClass::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);

}

void DefaultEnemyClass::Destroy() {
	EnemyClass::Destroy();
}

void DefaultEnemyClass::EnemyMoveProcessing() {
	m_Texture->AddXPosition(m_XMoveSpeed * -1);
}