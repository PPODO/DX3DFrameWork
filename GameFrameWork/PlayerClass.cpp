#include "PlayerClass.h"
#include "InputClass.h"
#include "SystemClass.h"
#include "DefaultProjectileClass.h"

PlayerClass::PlayerClass(ObjectPoolClass* OP) : m_TempInputManager(nullptr) {
	m_Projectiles.resize(PS_COUNT);

	SetPoolManager(OP);

	m_FireDelay = std::chrono::duration<float>(0.05f);

	m_TempInputManager = SystemClass::GetInst()->GetInputManager();
	m_XMoveSpeed = 5.f;
	m_YMoveSpeed = 5.f;
	m_MaxActivatedProjectile = 10;
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player.png");
	m_Texture->SetPosition(D3DXVECTOR3(0.f, FLOAT(GetWindowSize().bottom) / 2, 0.f) - m_Texture->GetImageCenter());

	if (!GetPoolManager()) {
		return false;
	}
	GetPoolManager()->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], 10);

	return true;
}

void PlayerClass::SetupPlayerInput() {
	if (!m_TempInputManager) {
		m_TempInputManager = SystemClass::GetInst()->GetInputManager();
	}

	m_TempInputManager->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
	m_TempInputManager->BindAxisDelegate(VK_LEFT, std::bind(&PlayerClass::MoveRight, this, -1.f));
	m_TempInputManager->BindAxisDelegate(VK_UP, std::bind(&PlayerClass::MoveForward, this, -1.f));
	m_TempInputManager->BindAxisDelegate(VK_DOWN, std::bind(&PlayerClass::MoveForward, this, 1.f));
	m_TempInputManager->BindAxisDelegate(VK_SPACE, std::bind(&PlayerClass::FireProjectile, this, 1.f));
}

void PlayerClass::Update(float DeltaTime) {
	Pawn::Update(DeltaTime);

	for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
		if (!(*Iterator)->CheckColliding(Iterator, std::bind(&PlayerClass::IsProjectileOutOfScreen, this, *Iterator))) {
			(*Iterator)->Update(DeltaTime);
			Iterator++;
		}
	}
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);
}

void PlayerClass::Destroy() {
	Pawn::Destroy();
}

void PlayerClass::MoveRight(float Value) {
	m_Texture->AddXPosition(Value * m_XMoveSpeed);
}

void PlayerClass::MoveForward(float Value) {
	m_Texture->AddYPosition(Value * m_YMoveSpeed);
}

void PlayerClass::FireProjectile(float Value) {
	SpawnProjectile(D3DXVECTOR3(1.f, 0.f, 0.f));
}

bool PlayerClass::IsProjectileOutOfScreen(class ProjectileClass* ProjectileObject) {
	if (ProjectileObject->GetTexture()->GetPosition().x - ProjectileObject->GetTexture()->GetImageCenter().x > GetWindowSize().right) {
		return true;
	}
	return false;
}