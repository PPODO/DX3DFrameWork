#include "ProjectileClass.h"
#include "TextureClass.h"

ProjectileClass::ProjectileClass() {
}

bool ProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);

	return true;
}

void ProjectileClass::Update(float DeltaTime) {
	ProjectileMoveProcessing();

}

void ProjectileClass::Render(LPD3DXSPRITE Sprite) {
	m_Texture->Render(Sprite);
}

void ProjectileClass::Destroy() {
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}