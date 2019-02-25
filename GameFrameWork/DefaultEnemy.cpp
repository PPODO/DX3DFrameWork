#include "DefaultEnemy.h"

DefaultEnemy::DefaultEnemy() {
	m_XMoveSpeed = 2.5f;
}

bool DefaultEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"DefaultEnemy.png");

	return true;
}

void DefaultEnemy::ProcessEnemyMovement() {
	m_Texture->AddXPosition(m_XMoveSpeed * -1);
}