#include "SplitEnemyClass.h"

SplitEnemyClass::SplitEnemyClass() {
	m_Name = "SplitEnemy";
	m_UseAutoSpawn = true;
	m_MinSpawnDelay = 3.f;
	m_MaxSpawnDelay = 6.f;
}

SplitEnemyClass::~SplitEnemyClass() {
}

bool SplitEnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"SplitEnemy.png");

	return true;
}

void SplitEnemyClass::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);

}

void SplitEnemyClass::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);

}

void SplitEnemyClass::Destroy() {
	EnemyClass::Destroy();
}

void SplitEnemyClass::ClearObject() {
	EnemyClass::ClearObject();

}

void SplitEnemyClass::EnemyMoveProcessing() {
}