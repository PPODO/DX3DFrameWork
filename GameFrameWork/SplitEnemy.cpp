#include "SplitEnemy.h"
#include "TextureClass.h"

SplitEnemy::SplitEnemy() : m_States(SPLIT_NONE) {
	memset(m_bIsReflect, 0, sizeof(bool) * 2);
	memset(m_ReflectDirection, 0, sizeof(D3DXVECTOR3) * 2);

	m_Name = "SplitEnemy";
	m_XMoveSpeed = 2.f;
	m_YMoveSpeed = 4.f;
	m_MinSpawnDelay = 3.f;
	m_MaxSpawnDelay = 7.f;
}

bool SplitEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"SplitEnemy.png");
	m_SplitImage = new TextureClass(Device, L"SplitEnemy.png");
	if (!m_SplitImage) {
		return false;
	}
	m_SplitImage->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));

	return true;
}

void SplitEnemy::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);

	if (m_bIsActive) {
		if (m_States > SPLIT_NONE) {
			if (CheckHeight(m_Texture, m_NormalVector[FIRST])) {
				CalculateReflectDirection(m_Texture, FIRST);
			}
			if (CheckHeight(m_SplitImage, m_NormalVector[SECOND])) {
				CalculateReflectDirection(m_SplitImage, SECOND);
			}
		}
	}
}

void SplitEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
	if (m_States > SPLIT_NONE) {
		m_SplitImage->Render(Sprite);
	}
}

void SplitEnemy::Destroy() {
	EnemyClass::Destroy();
	if (m_SplitImage) {
		delete m_SplitImage;
		m_SplitImage = nullptr;
	}
}

void SplitEnemy::ClearObject() {
	EnemyClass::ClearObject();

	memset(&m_bIsReflect, 0, sizeof(bool) * 2);
	m_States = SPLIT_NONE;
}

void SplitEnemy::EnemyMoveProcessing() {
	CheckSplitState(m_Texture, FIRST);
	CheckSplitState(m_SplitImage, SECOND);
}