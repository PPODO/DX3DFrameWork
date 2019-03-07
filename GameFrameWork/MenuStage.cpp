#include "MenuStage.h"
#include "BackGroundClass.h"
#include "ButtonClass.h"
#include "TextClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

MenuStage::~MenuStage() {
}

bool MenuStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	StageClass::Init(Device);

	m_BackGround = new BackGroundClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, L"Stage/Menu.png");

	{
		ButtonClass* StartButton = new ButtonClass(SS_MENU);
		if (!StartButton || !StartButton->Init(Device, L"Button/GameStartButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_PHASE1); })) {
			return false;
		}

		ButtonClass* IntroduceButton = new ButtonClass(SS_MENU);
		if (!IntroduceButton || !IntroduceButton->Init(Device, L"Button/IntroduceGameButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_INTRODUCE); })) {
			return false;
		}

		ButtonClass* MethodButton = new ButtonClass(SS_MENU);
		if (!MethodButton || !MethodButton->Init(Device, L"Button/GameplayMethodButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_PLAYMETOHD); })) {
			return false;
		}

		ButtonClass* ScoreButton = new ButtonClass(SS_MENU);
		if (!ScoreButton || !ScoreButton->Init(Device, L"Button/GameScoreButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_SCORE); })) {
			return false;
		}

		ButtonClass* CreditButton = new ButtonClass(SS_MENU);
		if (!CreditButton || !CreditButton->Init(Device, L"Button/GameCreditButton.png", [&]() { SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SS_CREDIT); })) {
			return false;
		}

		ButtonClass* ShutdownButton = new ButtonClass(SS_MENU);
		if (!ShutdownButton || !ShutdownButton->Init(Device, L"Button/GameShutdownButton.png", [&]() { DestroyWindow(SystemClass::GetInst()->GetWindowHandle()); })) {
			return false;
		}

		m_Button.push_back(StartButton);
		m_Button.push_back(IntroduceButton);
		m_Button.push_back(MethodButton);
		m_Button.push_back(ScoreButton);
		m_Button.push_back(CreditButton);
		m_Button.push_back(ShutdownButton);
	}
	
	FLOAT ButtonDistance = -150.f;
	for (size_t i = 0; i < m_Button.size(); i++) {
		TextureClass* Texture = m_Button[i]->GetTexture();
		if (Texture) {
			Texture->SetPosition(D3DXVECTOR3(FLOAT(GetWindowSize().right / 2), FLOAT(GetWindowSize().bottom / 1.5) + (ButtonDistance + (60 * i)), 0.f));
		}
		m_Button[i]->BindButtonAction();
	}

	m_Text = new TextClass;
	if (!m_Text) {
		return false;
	}
	m_Text->Init(Device, L"Text/GameTitle.png");
	m_Text->GetTexture()->SetPosition(D3DXVECTOR3(FLOAT(GetWindowSize().right / 2), 100.f, 0.f));

	return true;
}

void MenuStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	for (const auto& It : m_Button) {
		if (It) {
			It->Render(Sprite);
		}
	}
	m_Text->Render(Sprite);
}

void MenuStage::Destroy() {
	StageClass::Destroy();

	if (m_Text) {
		m_Text->Destroy();
		delete m_Text;
		m_Text = nullptr;
	}

	for (auto& It : m_Button) {
		if (It) {
			It->Destroy();
			delete It;
		}
	}
	m_Button.clear();
}

void MenuStage::ReleaseForChangingStage() {
	EventClass::GetInst()->WakeUpEventThread();
}