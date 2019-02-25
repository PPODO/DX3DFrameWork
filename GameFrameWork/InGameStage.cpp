#include "InGameStage.h"
#include "BackGroundUIClass.h"

InGameStage::~InGameStage() {
}

bool InGameStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundUIClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, true, L"Stage1.png", L"Stage1.png");

	return true;
}

void InGameStage::Update(float DeltaTime) {
	m_BackGround->Update(DeltaTime);
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	m_BackGround->Render(Sprite);
}

void InGameStage::Destroy() {
	if (m_BackGround) {
		m_BackGround->Destroy();
		delete m_BackGround;
		m_BackGround = nullptr;
	}
}