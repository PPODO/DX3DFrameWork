#include "LoadingClass.h"
#include "TextureClass.h"
#include "SystemClass.h"
#include "D3DXClass.h"
#include <iostream>

LoadingClass::LoadingClass(const size_t MaxImages, LPD3DXSPRITE Sprite) : m_MaxImages(MaxImages), m_bIsRunning(true), m_bWaitForSignal(true), m_RandomIndex(-1), m_TempSprite(Sprite) {
	std::random_device RandDevice;
	m_RandomAlgorithm = std::mt19937_64(RandDevice());

	m_Thread = std::thread([&]() {
		while (m_bIsRunning) {
			{
				std::unique_lock<std::mutex> Lock(m_Lock);
				m_Condition.wait(Lock);
			}
			// ������ �� �� ������, ���� �Լ��� �Ѱ��ִ� �������� Sprite���ڸ� �״�� �Ѱ��� ��� ��Ÿ�� ������ ��.
			// �Լ� ȣ�� �Ծ࿡�� �Ѱܹ��� ���ڸ� ��� ���ÿ��� �������� ���ñ� ��µ� �� �������� �߻��ϴ� ������ �Ǵ�, �Ѱܹ��� ���ڸ� ���� ������ ������ ���� �Լ��� �н����ִ� ��� ä��
			// ��� : �� �� �ð�,,,,
			MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), [this]() { Render(m_TempSprite); }));
			while (m_bWaitForSignal);
		}
	});
}

LoadingClass::~LoadingClass() {
	{
		std::unique_lock<std::mutex> Lock(m_Lock);
		m_bIsRunning = false;
		m_bWaitForSignal = false;
	}
	m_Condition.notify_all();
	m_Thread.join();

	for (auto Iterator : m_LoadingImages) {
		if (Iterator) {
			delete Iterator;
			Iterator = nullptr;
		}
	}
	m_LoadingImages.clear();
}

bool LoadingClass::Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2) {
	TCHAR TempSrc[25];
	for (size_t i = 0; i < m_MaxImages; i++) {
		wsprintf(TempSrc, L"Loading%d.png", (i + 1));
		TextureClass* NewTexture = new TextureClass(Device, TempSrc);
		if (!NewTexture && !NewTexture->m_Texture) {
			return false;
		}
		NewTexture->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_LoadingImages.push_back(NewTexture);
	}
	m_Random = std::uniform_int_distribution<int>(0, m_LoadingImages.size() - 1);
	return true;
}

void LoadingClass::Update(float DeltaTime) {
}

void LoadingClass::Render(LPD3DXSPRITE Sprite) {
	if (m_bIsRunning && m_RandomIndex >= 0 && m_LoadingImages[m_RandomIndex]) {
		m_LoadingImages[m_RandomIndex]->Render(Sprite);
	}
}

void LoadingClass::PickRandomImage() {
	m_RandomIndex = m_Random(m_RandomAlgorithm);
}