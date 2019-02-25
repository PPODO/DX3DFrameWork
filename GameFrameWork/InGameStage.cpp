#include "InGameStage.h"
#include "ObjectPoolClass.h"
#include "BackGroundUIClass.h"
#include "DefaultEnemy.h"
#include "EnemyClass.h"

InGameStage::InGameStage(std::vector<Actor*>& Actors, ObjectPoolClass* ObjectPool) : StageClass(Actors) {
	if (ObjectPool) {
		ObjectPool->GetPoolObject("DefaultEnemy", m_Enemy, 5);
	}
}

InGameStage::~InGameStage() {
}

bool InGameStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundUIClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, true, L"Stage1.png", L"Stage1.png");

	return true;
}

void InGameStage::Update(float DeltaTime) {
	m_BackGround->Update(DeltaTime);
	for (auto Iterator : m_Enemy) {
		Iterator->Update(DeltaTime);
	}
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	m_BackGround->Render(Sprite);
	for (auto Iterator : m_Enemy) {
		Iterator->Render(Sprite);
	}
}

void InGameStage::Destroy() {
	if (m_BackGround) {
		m_BackGround->Destroy();
		delete m_BackGround;
		m_BackGround = nullptr;
	}
}