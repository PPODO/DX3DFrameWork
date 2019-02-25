#include "MenuStage.h"
#include "BackGroundUIClass.h"
#include "ButtonClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

MenuStage::~MenuStage() {
}

bool MenuStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundUIClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, false, L"Menu.png");

	m_StartButton = new ButtonClass(SS_MENU);
	if (!m_StartButton) {
		return false;
	}
	m_StartButton->Init(Device, L"StartButton.png", []() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_PHASE1); });
	m_StartButton->GetTexture()->SetPosition(m_StartButton->GetTexture()->GetImageCenter());

	return true;
}

void MenuStage::Update(float DeltaTime) {
	m_BackGround->Update(DeltaTime);
}

void MenuStage::Render(LPD3DXSPRITE Sprite) {
	m_BackGround->Render(Sprite);
	m_StartButton->Render(Sprite);
}

void MenuStage::Destroy() {
	if (m_BackGround) {
		m_BackGround->Destroy();
		delete m_BackGround;
		m_BackGround = nullptr;
	}
	if (m_StartButton) {
		delete m_StartButton;
		m_StartButton = nullptr;
	}
}