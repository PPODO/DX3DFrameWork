#include "IdleScreenClass.h"
#include "TextureClass.h"
#include "SystemClass.h"
#include "D3DXClass.h"

IdleScreenClass* IdleScreenClass::m_Application;

IdleScreenClass::IdleScreenClass() : m_bIsRunning(true), m_bWaitForSignal(true), m_RandomIndex(-1), m_CurrentImage(ISS_LOADING) {
	m_Images.resize(ISS_COUNT);
	m_MaxImages[ISS_LOADING] = 2;
	m_MaxImages[ISS_NEXT] = 0;
	m_MaxImages[ISS_FADE] = 1;

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
			// 결론 : 내 두 시간,,,,7
			// -> 디버깅 해본 결과 대충 어느 곳이 문제인지 짐작할 수 있었음.
			// -> LoadingClass의 생성자가 호출된 메인 스레드와 그 생성자에서 만들어진 서브 스레드는 각자 다른 스택 공간을 가지고 있기 때문에, 매개변수의 값과 주소 또한 다름.
			// 스레드를 생성한 시점부터 생성자의 인수로 넘어온 Sprite는 완전히 다른 변수이고, 그 변수에는 쓰레기 값이 들어가 있었던 것. 또한 디버깅을 해보니 DLL이 다른 걸 확인할 수 있었음.
			// dll때문에 위의 문제가 생긴 게 아닌가 싶음.
			MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), [&]() { Render(m_TempSprite); }));
			while (m_bWaitForSignal);
		}
	});
}

IdleScreenClass::~IdleScreenClass() {
	{
		std::unique_lock<std::mutex> Lock(m_Lock);
		m_bIsRunning = false;
		m_bWaitForSignal = false;
	}
	m_Condition.notify_all();
	m_Thread.join();

	for (auto Iterator : m_Images) {
		if (Iterator.size() > 0) {
			for (auto SecondIterator : Iterator) {
				if (SecondIterator) {
					delete SecondIterator;
				}
			}
		}
	}
	m_Images.clear();
}

bool IdleScreenClass::Init(LPDIRECT3DDEVICE9 Device, LPD3DXSPRITE Sprite) {
	TCHAR TempSrc[30] = { L"\0" };

	for (size_t i = 0; i < m_Images.size(); i++) {
		switch (i) {
		case ISS_LOADING:
			wsprintf(TempSrc, L"Screen/Loading");
			if (!LoadingTexture(Device, TempSrc, i)) {
				return false;
			}
			break;
		case ISS_NEXT:
			wsprintf(TempSrc, L"Screen/Next");
			if (!LoadingTexture(Device, TempSrc, i)) {
				return false;
			}
			break;
		case ISS_FADE:
			wsprintf(TempSrc, L"Screen/Fade");
			if (!LoadingTexture(Device, TempSrc, i)) {
				return false;
			}
			break;
		}
	}

	if (!Sprite) {
		return false;
	}
	m_TempSprite = Sprite;
	return true;
}

void IdleScreenClass::Render(LPD3DXSPRITE Sprite) {
	if (m_bIsRunning && m_RandomIndex >= 0 && m_CurrentImage < ISS_COUNT && m_Images[m_CurrentImage].size() > 0) {
		m_Images[m_CurrentImage][m_RandomIndex]->Render(Sprite);
	}
}

void IdleScreenClass::PickRandomImage() {
	if (m_CurrentImage < ISS_COUNT) {
		m_Random = std::uniform_int_distribution<int>(0, m_Images[m_CurrentImage].size() - 1);
		m_RandomIndex = m_Random(m_RandomAlgorithm);
	}
}

inline bool IdleScreenClass::LoadingTexture(LPDIRECT3DDEVICE9 Device, TCHAR* str, size_t CurrentIndex) {
	for (size_t i = 0; i < m_MaxImages[CurrentIndex]; i++) {
		TCHAR TempStr[35] = { L"\0" };
		wsprintf(TempStr, L"%s%d.png", str, (i + 1));

		TextureClass* NewTexture = new TextureClass(Device, TempStr);
		if (!NewTexture && !NewTexture->m_Texture) {
			return false;
		}
		NewTexture->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_Images[CurrentIndex].push_back(NewTexture);
	}
	return true;
}