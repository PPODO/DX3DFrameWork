#include "GraphicClass.h"
#include "Actor.h"

GraphicClass* GraphicClass::m_Application = nullptr;

GraphicClass::GraphicClass() {
	m_RenderThread = std::thread([&]() {
		while (true) {
			if (!MessageQueueClass::GetInst()->IsEmpty()) {
				std::tuple<MessageState, std::function<void()>> Task = MessageQueueClass::GetInst()->PopMessage();
				Excute(Task);
			}
		}
	});
}

GraphicClass::~GraphicClass() {
	m_RenderThread.join();
}

void GraphicClass::Render(LPDIRECT3DDEVICE9 Device, LPD3DXSPRITE Sprite, std::vector<Actor*>* Actors) {
	Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	if(SUCCEEDED(Device->BeginScene())) {
		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		if (Actors) {
			for (Actor* AActor : *Actors) {
				if (AActor) {
					AActor->Render(Sprite);
				}
			}
		}
		Sprite->End();
	}
	Device->EndScene();
	Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void GraphicClass::Excute(std::tuple<MessageState, std::function<void()>> Task) {
	switch (std::get<0>(Task)) {
	case MS_RENDER:
		std::get<1>(Task)();
		break;
	case MS_DESTROY:
		Shutdown();
		break;
	}
}

void GraphicClass::Shutdown() {
	DestroySingleton();
}