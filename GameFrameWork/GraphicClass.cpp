#include "GraphicClass.h"
#include "ActorClass.h"
#include "D3DXClass.h"
#include <algorithm>

GraphicClass::~GraphicClass() {
}

bool GraphicClass::Init(int Width, int Height, HWND hWnd) {
	m_D3DX = new D3DXClass;
	if (!m_D3DX || !m_D3DX->Init(Width, Height, hWnd)) {
		return false;
	}
	return true;
}

bool GraphicClass::Render() {
	if (m_D3DX->BeginScene()) {
		for (Actor* AActor : m_TempActorClass->GetActors()) {
			if (AActor) {
				m_D3DX->DrawActors(*AActor->GetTexture());
			}
		}
	}
	m_D3DX->EndScene();
	return true;
}

void GraphicClass::Shutdown() {
	if (m_D3DX) {
		delete m_D3DX;
		m_D3DX = nullptr;
	}
}