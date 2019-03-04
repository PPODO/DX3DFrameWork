#include "ScoreStage.h"
#include "BackGroundClass.h"
#include "ButtonClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

ScoreStage::~ScoreStage() {
}

bool ScoreStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, false, L"Stage/Score.png");

	m_BackButton = new ButtonClass(SS_SCORE);
	if (!m_BackButton) {
		return false;
	}
	m_BackButton->Init(Device, L"Button/BackButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_MENU); });
	m_BackButton->GetTexture()->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f) + m_BackButton->GetTexture()->GetImageCenter());
	m_BackButton->BindButtonAction();

	return true;
}

void ScoreStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	if (m_BackButton) {
		m_BackButton->Render(Sprite);
	}
}

void ScoreStage::Destroy() {
	StageClass::Destroy();
	
	if (m_BackButton) {
		m_BackButton->Destroy();
		delete m_BackButton;
		m_BackButton = nullptr;
	}
}