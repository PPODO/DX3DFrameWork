#include "MainStage.h"
#include "WidgetClass.h"
#include "ButtonClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

MainStage::MainStage() : m_WidgetClass(nullptr) {
	m_FileSrc.push_back(L"Widget/Menu.png");
	m_FileSrc.push_back(L"Widget/Introduce.png");
	m_FileSrc.push_back(L"Widget/PlayMethod.png");
	m_FileSrc.push_back(L"Widget/Credit.png");
	m_FileSrc.push_back(L"Widget/Score.png");
}

MainStage::~MainStage() {
	if (m_WidgetClass) {
		delete m_WidgetClass;
		m_WidgetClass = nullptr;
	}
}

bool MainStage::Init(LPDIRECT3DDEVICE9 Device) {
	float Distance = -150.f;
	RECT WindowSize = SystemClass::GetInst()->GetWindowSize();
	
	m_WidgetClass = new WidgetClass;
	if (!m_WidgetClass || !m_WidgetClass->Init(Device, m_FileSrc)) {
		return false;
	}

	m_Buttons.push_back(new ButtonClass(L"Button/GameStartButton.png"));
	m_Buttons.push_back(new ButtonClass(L"Button/IntroduceGameButton.png"));
	m_Buttons.push_back(new ButtonClass(L"Button/GameplayMethodButton.png"));
	m_Buttons.push_back(new ButtonClass(L"Button/GameCreditButton.png"));
	m_Buttons.push_back(new ButtonClass(L"Button/GameScoreButton.png"));
	m_Buttons.push_back(new ButtonClass(L"Button/GameShutdownButton.png"));

	for (size_t i = 0; i < m_Buttons.size(); i++) {
		if (i < m_Buttons.size() && m_Buttons[i] && m_Buttons[i]->Init(Device)) {
			m_Buttons[i]->GetTextureByIndex(0)->SetPosition(D3DXVECTOR3(FLOAT(WindowSize.right / 2) - FLOAT(m_Buttons[i]->GetTextureByIndex(0)->GetRect().right / 2), FLOAT(WindowSize.bottom / 2) + (Distance + (i * 100)), 0.f));
			switch (i) {
			case 0:
				m_Buttons[0]->BindMouseAction(EBT_MAIN, [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(EST_STAGE1); });
				break;
			}
			m_Buttons[i]->BindMouseAction(EBT_MAIN, [=]() { m_WidgetClass->SetDrawImageIndex(i); });
		}
	}
	return true;
}

void MainStage::Update(float DeltaTime) {
	
}

void MainStage::Render(LPD3DXSPRITE Sprite) {
	m_WidgetClass->Render(Sprite);

	for (const auto& Iterator : m_Buttons) {
		if (Iterator) {
			Iterator->Render(Sprite);
		}
	}
}

void MainStage::ChangeStageNotification() {
}

void MainStage::ReleaseStageNotification() {
}
