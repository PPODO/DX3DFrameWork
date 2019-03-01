#include "ProjectileClass.h"
#include "TextureClass.h"
#include <algorithm>

ProjectileClass::ProjectileClass() : m_ActivatedList(nullptr), m_ObjectList(nullptr) {
}

bool ProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);

	return true;
}

void ProjectileClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	ProjectileMoveProcessing();
}

void ProjectileClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void ProjectileClass::Destroy() {
	if (m_ActivatedList) {
		m_ActivatedList = nullptr;
	}
	if (m_ObjectList) {
		m_ObjectList = nullptr;
	}
	Actor::Destroy();
}

bool ProjectileClass::CheckColliding() {
	if (0) {
		auto It = std::find(m_ActivatedList->cbegin(), m_ActivatedList->cend(), this);
		if (It != m_ActivatedList->cend()) {
			ClearObject();
			m_ActivatedList->erase(It);
			m_ObjectList->push(*It);
		}
		return true;
	}
	return false;
}

void ProjectileClass::SpawnProjectile(const D3DXVECTOR3 & Location, const D3DXVECTOR3 & Direction, std::stack<ProjectileClass*>* ObjectList, std::vector<ProjectileClass*>* ActivatedList) {
	ObjectList ? m_ObjectList = ObjectList : m_ObjectList = nullptr;
	ActivatedList ? m_ActivatedList = ActivatedList : m_ActivatedList = nullptr;

	m_Texture->SetPosition(Location);
	m_MoveDirection = Direction;
	m_bIsActive = true;
}

void ProjectileClass::ClearObject() {
	m_ObjectList = nullptr;
	m_ActivatedList = nullptr;
	m_bIsActive = false;
}