#include "PlayerClass.h"
#include "ObjectPoolClass.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "EventClass.h"
#include "MachineGun.h"

PlayerClass::PlayerClass(ObjectPoolClass* OP) : m_MachineGun(nullptr), m_bIsMoving(false) {
	SetPoolManager(OP);
	
	m_CollisionType = ECT_PLAYER;
	EventClass::GetInst()->BindCollisionEvent(this);
	
	m_LocationIsHaveToGo = FLOAT((GetWindowSize().right / 8));
	m_MaximumMovement = { LONG(m_LocationIsHaveToGo), 0, GetWindowSize().right - LONG(m_LocationIsHaveToGo), 0 };
	m_MoveSpeed = { 7.5f, 0.f };

	m_FireDelayTime = std::chrono::duration<float>(0.25f);
	m_ProjectileObjects.resize(EPT_COUNT);
	m_MaxActivatedProjectile = 10;
}

PlayerClass::~PlayerClass() {
	for (auto& It : m_ActivatedProjectileObjects) {
		if (It) {
			m_ProjectileObjects[It->GetProjectileType()].push(It);
		}
	}
	m_ActivatedProjectileObjects.clear();

	for (auto& It : m_ProjectileObjects) {
		if (GetPoolManager()) {
			GetPoolManager()->ReleaseAll(It.top()->GetName(), It, It.size());
		}
	}

	if (m_MachineGun) {
		delete m_MachineGun;
		m_MachineGun = nullptr;
	}
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	PawnClass::Init(Device, L"Player/Player.png");

	m_MachineGun = new MachineGun;
	if (!m_MachineGun || !m_MachineGun->Init(Device)) {
		return false;
	}
	m_MachineGunOffset = D3DXVECTOR3(0.f, m_MachineGun->GetActorImage()->GetImageCenter().y * 2, 0.f);

	if (GetPoolManager()) {
		GetPoolManager()->GetPoolObject("Projectile_Default", m_ProjectileObjects[EPT_DEFAULT], 20);
	}
	return true;
}

void PlayerClass::SetupPlayerInput() {
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_W, IE_Pressed, std::bind(&PlayerClass::Jump, this));

	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_SPACE, IE_Pressed, std::bind(&PlayerClass::PressedFire, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_SPACE, IE_Released, std::bind(&PlayerClass::ReleasedFire, this));

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_D, "MoveRight", std::bind(&PlayerClass::MoveRight, this, std::placeholders::_1), 1.f);

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_NUMPAD4, "MachineGunRight", std::bind(&PlayerClass::MachineGunRotation, this, std::placeholders::_1), -1);
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_NUMPAD6, "MachineGunRight", std::bind(&PlayerClass::MachineGunRotation, this, std::placeholders::_1), 1);
}

void PlayerClass::Update(float DeltaTime) {
	PawnClass::Update(DeltaTime);

	if (!GetStartMoveToLocation()) {
		if (!m_bIsMoving) {
			if (GetActorImage()->GetPosition().x - m_LocationIsHaveToGo >= FLOATNearlyEqual) {
				GetActorImage()->AddXPosition(-1.25 * m_MoveSpeed.x);
			}
		}
		if (GetFireState() && CanFire()) {
			auto Projectile = FireProjectile(m_MachineGun->GetOffset());
			if (Projectile) {
				Projectile->SetProjectileSeta(m_MachineGun->GetCurrentAngle());
			}
		}
		CalculateProjectile(DeltaTime);
	}

	SetMachineGunLocation();
	m_MachineGun->Update(DeltaTime);
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	m_MachineGun->Render(Sprite);

	PawnClass::Render(Sprite);

	for (const auto It : m_ActivatedProjectileObjects) {
		if (It && It->GetActorIsActivated()) {
			It->Render(Sprite);
		}
	}
}

void PlayerClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_OBSTACLE) {
			
		}
		else if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			if (!GetIsJumping() && !GetIsLanded()) {
				IsLanded();
			}
		}
	}
}

void PlayerClass::Jump() {
	if (!GetStartMoveToLocation()) {
		DoJump();
	}
}

void PlayerClass::PressedFire() {
	SetFireState(true);
}

void PlayerClass::ReleasedFire() {
	SetFireState(false);
}

void PlayerClass::MoveRight(float Value) {
	if (!GetStartMoveToLocation()) {
		if (Value != 0.f && GetActorImage() && LONG(GetActorImage()->GetPosition().x + m_MoveSpeed.x * Value) < m_MaximumMovement.right) {
			GetActorImage()->AddXPosition(m_MoveSpeed.x * Value);
			SetMachineGunLocation();
		}
		m_bIsMoving = bool(Value);
	}
}

void PlayerClass::MachineGunRotation(float Value) {
	if (!GetStartMoveToLocation() && Value != 0.f) {
		m_MachineGun->SetRotation(Value);
	}
}

void PlayerClass::PlayStartMoveToLocation(float Value) {
	D3DXVECTOR3 NewLocation, ToGoLocation(m_LocationIsHaveToGo, GetActorImage()->GetPosition().y, 0.f);
	D3DXVec3Lerp(&NewLocation, &GetActorImage()->GetPosition(), &ToGoLocation, Value);
	GetActorImage()->SetPosition(NewLocation);

	if (fabs(NewLocation.x - m_LocationIsHaveToGo) <= FLOATNearlyEqual) {
		SetStartMoveToLocation(false);
	}
}

inline void PlayerClass::SetMachineGunLocation() {
	auto MachineGunObject = m_MachineGun->GetActorImage();
	if (MachineGunObject) {
		MachineGunObject->SetPosition(GetActorImage()->GetPosition() - m_MachineGunOffset);
	}
}

ProjectileClass* PlayerClass::FireProjectile(const D3DXVECTOR3& Offset) {
	if (m_ActivatedProjectileObjects.size() < m_MaxActivatedProjectile) {
		auto Projectile = m_ProjectileObjects[m_ProjectileType].top();
		if (Projectile) {
			Projectile->SpawnActor(GetActorImage()->GetPosition() + Offset);
			m_ActivatedProjectileObjects.push_back(Projectile);
			m_ProjectileObjects[m_ProjectileType].pop();
		}
		return Projectile;
	}
	return nullptr;
}

void PlayerClass::CalculateProjectile(const float& DeltaTime) {
	for (auto Iterator = m_ActivatedProjectileObjects.begin(); Iterator != m_ActivatedProjectileObjects.end();) {
		if (Iterator != m_ActivatedProjectileObjects.cend() && (*Iterator)->GetActorIsActivated()) {
			if ((*Iterator)->GetActorImage()) {
				(*Iterator)->GetActorImage()->AddYPosition(GetCurrentHeight() * -1);
			}
			(*Iterator)->Update(DeltaTime);
			++Iterator;
		}
		else {
			m_ProjectileObjects[(*Iterator)->GetProjectileType()].push((*Iterator));
			Iterator = m_ActivatedProjectileObjects.erase(Iterator);
		}
	}
}