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
			// ������ �� �� ������, ���� �Լ��� �Ѱ��ִ� �������� Sprite���ڸ� �״�� �Ѱ��� ��� ��Ÿ�� ������ ��.
			// �Լ� ȣ�� �Ծ࿡�� �Ѱܹ��� ���ڸ� ��� ���ÿ��� �������� ���ñ� ��µ� �� �������� �߻��ϴ� ������ �Ǵ�, �Ѱܹ��� ���ڸ� ���� ������ ������ ���� �Լ��� �н����ִ� ��� ä��
			// ��� : �� �� �ð�,,,,7
			// -> ����� �غ� ��� ���� ��� ���� �������� ������ �� �־���.
			// -> LoadingClass�� �����ڰ� ȣ��� ���� ������� �� �����ڿ��� ������� ���� ������� ���� �ٸ� ���� ������ ������ �ֱ� ������, �Ű������� ���� �ּ� ���� �ٸ�.
			// �����带 ������ �������� �������� �μ��� �Ѿ�� Sprite�� ������ �ٸ� �����̰�, �� �������� ������ ���� �� �־��� ��. ���� ������� �غ��� DLL�� �ٸ� �� Ȯ���� �� �־���.
			// dll������ ���� ������ ���� �� �ƴѰ� ����.
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