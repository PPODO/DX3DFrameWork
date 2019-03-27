#pragma once
#include <d3dx9.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include "Singleton.h"
#include "MessageQueueClass.h"

const int MaxRenderedThread = 1;

class GraphicClass : public Singleton<GraphicClass> {
private:
	LPDIRECT3DDEVICE9 m_Device;
	LPD3DXSPRITE m_Sprite;
	std::mutex m_Lock;
	std::vector<std::thread> m_RenderThread;
	bool m_bIsStop;

private:
	void Excute(std::tuple<MessageState, std::function<void()>>&);
	void Shutdown();
	void StopThread();

public:
	GraphicClass();
	~GraphicClass();

public:
	void Init(LPDIRECT3DDEVICE9, LPD3DXSPRITE);
	void Render(const std::function<void()>&);

};