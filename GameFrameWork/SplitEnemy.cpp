#include "SplitEnemy.h"
#include "TextureClass.h"

SplitEnemy::SplitEnemy() : m_States(SPLIT_NONE) {
	memset(&m_bIsReflect, 0, sizeof(bool) * 2);

	m_Name = "SplitEnemy";
	m_XMoveSpeed = 4.f;
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
	if (!m_bIsActive && CheckSpawnTime()) {
		m_SplitImage->SetPosition(m_Texture->GetPosition());
		m_bIsActive = true;
	}
	if (m_bIsActive && !CheckOutOfScreen()) {
		EnemyMoveProcessing();
	}
	//
	if (m_Texture->GetPosition().x < 500.f) {
		m_States = SPLIT_SPLIT;
	}
	//
	if (m_States > SPLIT_NONE) {
		if (YScreenCrash(m_Texture, m_NormalVector[FIRST])) {
			CalculateReflectDirection(m_Texture, FIRST);
		}
		if (YScreenCrash(m_SplitImage, m_NormalVector[SECOND])) {
			CalculateReflectDirection(m_SplitImage, SECOND);
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
	m_BeforePosition[FIRST] = m_Texture->GetPosition();
	m_BeforePosition[SECOND] = m_SplitImage->GetPosition();

	CheckSplitState(m_Texture, FIRST);
	CheckSplitState(m_SplitImage, SECOND);
}