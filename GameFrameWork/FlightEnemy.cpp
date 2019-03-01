#include "FlightEnemy.h"

FlightEnemy::FlightEnemy() {
	m_Name = "FlightEnemy";
	m_XMoveSpeed;
	m_YMoveSpeed;
	m_MinSpawnDelay;
	m_MaxSpawnDelay;
}

bool FlightEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"FlightEnemy");

	return true;
}

void FlightEnemy::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);
}

void FlightEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
}

void FlightEnemy::Destroy() {
	EnemyClass::Destroy();
}

void FlightEnemy::EnemyMoveProcessing() {
}