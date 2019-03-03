#include "PlayerClass.h"
#include "InputClass.h"
#include "SystemClass.h"
#include "DefaultProjectileClass.h"
#include <random>

bool IsNearlyEqual(const D3DXVECTOR3& V1, const D3DXVECTOR3& V2, float Epsilon = 1) {
	float X, Y, Z;
	X = fabsf(V1.x - V2.x);
	Y = fabsf(V1.y - V2.y);
	Z = fabsf(V1.z - V2.z);

	return (X <= Epsilon && Y <= Epsilon && Z <= Epsilon);
}

PlayerClass::PlayerClass(ObjectPoolClass* OP) : m_TempInputManager(nullptr), m_bCanMove(false), m_CurrentAddon(0) {
	m_Projectiles.resize(PS_COUNT);
	SetPoolManager(OP);

	m_Health = 200.f;
	m_XMoveSpeed = 5.f;
	m_YMoveSpeed = 5.f;
	m_MaxActivatedProjectile = 10;
	m_FireDelay = std::chrono::duration<float>(0.1f);
	m_TempInputManager = SystemClass::GetInst()->GetInputManager();
	m_Collision = CT_PLAYER;
	SystemClass::GetInst()->GetEventClass()->BindCollisionEvent(this);
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player.png");
	m_Texture->SetPosition(GenerateRandomSpawnPoint());

	if (!GetPoolManager()) {
		return false;
	}
	GetPoolManager()->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], 10);

	for (size_t i = 0; i < MaxAddon; i++) {
		Pawn* NewPawn = new Pawn;
		if (NewPawn && NewPawn->Init(Device, L"PlayerAddon.png")) {
			std::stack<ProjectileClass*> List;
			GetPoolManager()->GetPoolObject("DefaultProjectile", List, 5);
			NewPawn->SetCollisionType(CT_PLAYER);
			NewPawn->SetProjectiles(List, 5, 0.75f, true, D3DXVECTOR3(1.f, 0.f, 0.f));
			m_PlayerAddon.push_back(NewPawn);
			m_AddonPosition.push_back(D3DXVECTOR3(0.f, FLOAT(50 * ((i + 2) / 2)) * (i % 2 == 0 ? -1 : 1), 0.f));
			SystemClass::GetInst()->GetEventClass()->BindCollisionEvent(NewPawn);
		}
		else {
			return false;
		}
	}

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

	for (size_t i = 0; i < m_PlayerAddon.size(); i++) {
		if (m_PlayerAddon[i] && m_PlayerAddon[i]->GetIsActivation()) {
			m_PlayerAddon[i]->GetTexture()->SetPosition(m_Texture->GetPosition() + m_AddonPosition[i]);
			m_PlayerAddon[i]->Update(DeltaTime);
		}
	}
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

	for (const auto& It : m_PlayerAddon) {
		if (It && It->GetIsActivation()) {
			It->Render(Sprite);
		}
	}
}

void PlayerClass::Destroy() {
	Pawn::Destroy();

	for (auto& It : m_PlayerAddon) {
		if (It) {
			It->Destroy();
			delete It;
		}
	}
}

void PlayerClass::TriggerCollisionEventByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this) {
		if (OtherActor->GetActorCollisionType() == CT_PROJECTILE) {
			ProjectileClass* Projectile = (ProjectileClass*)OtherActor;
			if (Projectile && Projectile->GetOwner()->GetActorCollisionType() != CT_PLAYER) {
				
			}
		}
	}
}

void PlayerClass::MoveRight(float Value) {
	if (m_bCanMove) {
		m_Texture->AddXPosition(Value * m_XMoveSpeed);
	}
}

void PlayerClass::MoveForward(float Value) {
	if (m_bCanMove) {
		m_Texture->AddYPosition(Value * m_YMoveSpeed);
	}
}

void PlayerClass::FireProjectile(float Value) {
	if (m_bCanMove) {
		SpawnProjectile(D3DXVECTOR3(1.f, 0.f, 0.f));
	}
}

bool PlayerClass::MoveToPosition(const D3DXVECTOR3& Position, const float DeltaTime) {
	D3DXVECTOR3 Location;
	D3DXVec3Lerp(&Location, &Position, &m_Texture->GetPosition(), 0.0f);

	if (IsNearlyEqual(Location, m_Texture->GetPosition(), 5)) {
		return true;
	}
	Location -= m_Texture->GetPosition();
	m_Texture->AddPosition(Location * DeltaTime);
	return false;
}

void PlayerClass::ResetAllObject() {
	for (auto& It : m_PlayerAddon) {
		if (It) {
			It->ClearActivatedProjectile();
		}
	}
	ClearActivatedProjectile();
	m_Texture->SetPosition(GenerateRandomSpawnPoint());
	m_bCanMove = false;
}

D3DXVECTOR3 PlayerClass::GenerateRandomSpawnPoint() {
	std::random_device RandomDevice;
	std::mt19937_64 RandomAlgorithm(RandomDevice());
	std::uniform_real_distribution<float> Rand(float(GetWindowSize().top), float(GetWindowSize().bottom));

	return D3DXVECTOR3(0.f, Rand(RandomAlgorithm), 0.f) - m_Texture->GetImageCenter();
}