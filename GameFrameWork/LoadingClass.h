#pragma once
#include "BackGroundUIClass.h"
#include "MessageQueueClass.h"
#include "GraphicClass.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <functional>

class LoadingClass : public BackGroundUIClass {
private:
	bool m_bIsRunning, m_bWaitForSignal;
	TCHAR* m_FileSrc;
	size_t m_MaxImages, m_RandomIndex;
	std::vector<class TextureClass*> m_LoadingImages;
	std::thread m_Thread;
	std::mutex m_Lock;
	std::condition_variable m_Condition;
	std::mt19937_64 m_RandomAlgorithm;
	std::uniform_int_distribution<int> m_Random;
	LPD3DXSPRITE m_TempSprite;

private:
	void PickRandomImage();

public:
	LoadingClass(const size_t MaxImages, LPD3DXSPRITE);
	~LoadingClass();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2 = nullptr);
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite);

	inline void BeginDrawImage();
	inline void ClearImage();
};

inline void LoadingClass::BeginDrawImage() {
	PickRandomImage();
	m_Condition.notify_one();
	m_bWaitForSignal = false;
}

inline void LoadingClass::ClearImage() {
	m_bWaitForSignal = true;
}