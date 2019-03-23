#include "GraphicClass.h"
#include "Actor.h"

GraphicClass* GraphicClass::m_Application;

GraphicClass::GraphicClass() : m_bIsStop(false) {
	for (int i = 0; i < MaxRenderedThread; i++) {
		m_RenderThread.push_back(std::thread([&]() {
			while (!m_bIsStop) {
				try {
					std::unique_lock<std::mutex> Lock(m_Lock);
					if (!MessageQueueClass::GetInst()->IsEmpty()) {
						std::tuple<MessageState, std::function<void()>> Task = MessageQueueClass::GetInst()->PopMessage();
						Lock.unlock();
						Excute(Task);
					}
				}
				catch (const std::exception&) {
				}
			}
		}));
	}
}

GraphicClass::~GraphicClass() {
	std::unique_lock<std::mutex> Lock(m_Lock);
	for (int i = 0; i < MaxRenderedThread; i++) {
		m_bIsStop = true;
		m_RenderThread[i].join();
	}
}

void GraphicClass::Init(LPDIRECT3DDEVICE9 Device, LPD3DXSPRITE Sprite) {
	if (Device) {
		m_Device = Device;
	}
	if (Sprite) {
		m_Sprite = Sprite;
	}
}

void GraphicClass::Render(const std::function<void()>& Function) {
	if (m_Device && m_Sprite) {
		m_Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
		if (SUCCEEDED(m_Device->BeginScene())) {
			m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			Function();
			m_Sprite->End();
		}
		m_Device->EndScene();
		m_Device->Present(nullptr, nullptr, nullptr, nullptr);
	}
	SystemClass::GetInst()->SetWaitForRender(false);
}

void GraphicClass::Excute(std::tuple<MessageState, std::function<void()>>& Task) {
	switch (std::get<0>(Task)) {
	case MS_INIT:
		std::get<1>(Task)();
		break;
	case MS_RENDER:
		std::get<1>(Task)();
		break;
	case MS_DESTROY:
		Shutdown();
		break;
	}
}

void GraphicClass::Shutdown() {
	m_Device = nullptr;
	m_Sprite = nullptr;
	DestroySingleton();
}