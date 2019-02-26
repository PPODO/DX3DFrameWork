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
			// 이유는 알 수 없지만, 람다 함수를 넘겨주는 과정에서 Sprite인자를 그대로 넘겨줄 경우 런타임 에러가 남.
			// 함수 호출 규약에서 넘겨받은 인자를 어떻게 스택에서 제거할지 뭐시기 라는데 그 과정에서 발생하는 문제라 판단, 넘겨받은 인자를 따로 변수에 저장해 람다 함수로 패스해주는 방식 채택
			// 결론 : 내 두 시간,,,,
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