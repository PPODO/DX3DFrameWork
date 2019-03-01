#include "EnemyClass.h"

std::random_device EnemyClass::m_RandomDevice;
std::mt19937_64 EnemyClass::m_RandomAlgorithm;
std::uniform_real_distribution<float> EnemyClass::m_Random;

EnemyClass::EnemyClass() : m_bOutOfScreen(false) {
	m_RandomAlgorithm = std::mt19937_64(m_RandomDevice());
	m_FireDelay = std::chrono::duration<float>(10.f);
	m_bIsActive = false;
}

EnemyClass::~EnemyClass() {
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, FileSrc, CustomRect);

	return true;
}

void EnemyClass::Update(float DeltaTime) {
	if (!m_bIsActive && CheckSpawnTime()) {
		m_bIsActive = true;
	}
	if (m_bIsActive && !CheckOutOfScreen()) {
		EnemyMoveProcessing();
	}
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

}

void EnemyClass::Destroy() {
	Pawn::Destroy();
}

void EnemyClass::ClearObject() {
	m_bIsActive = m_bOutOfScreen = false;
}

void EnemyClass::SpawnAtLocation() {
	m_LastSpawnTime = std::chrono::system_clock::now();
	m_Random = std::uniform_real_distribution<float>(m_MinSpawnDelay, m_MaxSpawnDelay);
	m_SpawnTime = std::chrono::duration<float>(m_Random(m_RandomAlgorithm));

	m_Random = std::uniform_real_distribution<float>(m_Texture->GetImageCenter().y * 2, GetWindowSize().bottom - m_Texture->GetImageCenter().y * 2);
	m_Texture->SetPosition(D3DXVECTOR3((FLOAT)GetWindowSize().right, m_Random(m_RandomAlgorithm), 0.f) + m_Texture->GetImageCenter());
}