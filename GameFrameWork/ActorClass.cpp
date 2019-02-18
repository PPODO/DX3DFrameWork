#include "ActorClass.h"
#include "GraphicClass.h"
#include "EnemyClass.h"

bool Actor::Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) {
	if (bUseCustomRect) {
		m_Image = new TextureClass(Device, ImageSrc, CustomRect);
	}
	else {
		m_Image = new TextureClass(Device, ImageSrc);
	}
	if (!m_Image) {
		return false;
	}

	return true;
}

void Actor::Destroy() {
	if (m_Image) {
		delete m_Image;
		m_Image = nullptr;
	}
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	Enemy = new EnemyClass;

	m_Actors.push_back(Enemy);
	m_LoadingThread = std::thread([&Device, this]() {
		for (Actor* AActor : m_Actors) {
			try {
				if (!AActor->Init(Device)) {
					throw "Init Actor Failure!";
				}
			}
			catch (const char* Exception){

			}
		}	
	});
	m_LoadingThread.join();
	return true;
}

void ActorClass::Frame() {
	for (Actor* AActor : m_Actors) {
		if (AActor) {
			AActor->Update();
		}
	}
}

void ActorClass::Shutdown() {
	for (Actor* AActor : m_Actors) {
		if (AActor) {
			AActor->Destroy();
			delete AActor;
			AActor = nullptr;
		}
	}
	m_Actors.clear();
}