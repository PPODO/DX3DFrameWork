#include "MainStage.h"
#include "WidgetClass.h"
#include "ButtonClass.h"

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
	m_WidgetClass = new WidgetClass;
	if (!m_WidgetClass || !m_WidgetClass->Init(Device, m_FileSrc)) {
		return false;
	}

	m_Buttons.push_back(new ButtonClass(L"", L""));

	for (const auto& Iterator : m_Buttons) {
		if (Iterator) {
//			Iterator->Init(Device);
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