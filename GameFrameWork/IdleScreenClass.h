#pragma once
#include "MessageQueueClass.h"
#include "GraphicClass.h"
#include "Singleton.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <functional>

enum IdleScreenState { ISS_LOADING, ISS_NEXT, ISS_FADE, ISS_COUNT };

class IdleScreenClass : public Singleton<IdleScreenClass> {
private:
	bool m_bIsRunning, m_bWaitForSignal;
	std::vector<std::vector<class TextureClass*>> m_Images;
	std::thread m_Thread;
	std::mutex m_Lock;
	std::condition_variable m_Condition;
	std::mt19937_64 m_RandomAlgorithm;
	std::uniform_int_distribution<int> m_Random;
	LPD3DXSPRITE m_TempSprite;

private:
	TCHAR* m_FileSrc;
	size_t m_RandomIndex;
	size_t m_MaxImages[ISS_COUNT];
	IdleScreenState m_CurrentImage;

private:
	void PickRandomImage();

	inline bool LoadingTexture(LPDIRECT3DDEVICE9, TCHAR*, size_t);

public:
	IdleScreenClass();
	~IdleScreenClass();

	bool Init(LPDIRECT3DDEVICE9 Device, LPD3DXSPRITE Sprite);
	void Render(LPD3DXSPRITE Sprite);

	inline void BeginDrawImage(IdleScreenState);
	inline void ClearImage();

	inline bool GetWaitForSignal() const { return m_bWaitForSignal; }
};

inline void IdleScreenClass::BeginDrawImage(IdleScreenState ISS) {
	m_CurrentImage = ISS;
	PickRandomImage();
	m_Condition.notify_one();
	m_bWaitForSignal = false;
}

inline void IdleScreenClass::ClearImage() {
	m_bWaitForSignal = true;
}