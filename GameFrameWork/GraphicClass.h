#pragma once
#include <d3dx9.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include "Singleton.h"
#include "MessageQueueClass.h"

class GraphicClass : public Singleton<GraphicClass> {
private:
	LPDIRECT3DDEVICE9 m_Device;
	LPD3DXSPRITE m_Sprite;
	std::thread m_RenderThread;

private:
	void Excute(std::tuple<MessageState, std::function<void()>>&);
	void Shutdown();

public:
	GraphicClass();
	~GraphicClass();

public:
	void Init(LPDIRECT3DDEVICE9, LPD3DXSPRITE);
	void Render(const std::function<void()>&);

};