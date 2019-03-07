#include "DefaultEnemyClass.h"

DefaultEnemyClass::DefaultEnemyClass() {
	SetName("DefaultEnemy");

	m_XMoveSpeed = 7.5f;
	m_MinSpawnDelay = 1.f;
	m_MaxSpawnDelay = 2.5;
}

DefaultEnemyClass::~DefaultEnemyClass() {
}

bool DefaultEnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"Enemy/DefaultEnemy.png");

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