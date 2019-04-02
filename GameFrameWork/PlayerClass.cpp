#include "PlayerClass.h"
#include "ParticleClass.h"
#include "ObjectPoolClass.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "InGameStage.h"
#include "InputClass.h"
#include "EventClass.h"
#include "MachineGun.h"
#include "HomingMissileProjectile.h"
#include "Cannon.h"

PlayerClass::PlayerClass(ObjectPoolClass* OP) : m_bIsTriple(false), m_bIsMoving(false) {
	m_SpriteWidth = 6, m_SpriteHeight = 4;
	SetPoolManager(OP);
	
	m_CollisionType = ECT_PLAYER;
	EventClass::GetInst()->BindCollisionEvent(this);
	
	m_LocationIsHaveToGo = FLOAT((GetWindowSize().right / 8));
	m_MaximumMovement = { LONG(m_LocationIsHaveToGo), 0, GetWindowSize().right - LONG(m_LocationIsHaveToGo), 0 };
	m_MoveSpeed = { 7.5f, 0.f };

	m_FireDelayTime = std::chrono::duration<float>(0.25f);
	m_MaxActivatedProjectile = 30;

	m_WeaponType = EWT_MACHINEGUN;
	m_MaxMachineGun = 3;
	m_WeaponDefaultAngle = 30.f;
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	PawnClass::Init(Device, L"Player/Player.png");

	m_CurrentWeapon.push_back(new MachineGun);
	if (!m_CurrentWeapon.back() || !m_CurrentWeapon.back()->Init(Device)) {
		return false;
	}
	m_CurrentWeapon.back()->SetRotation(m_WeaponDefaultAngle * (m_MaxMachineGun - 1));
	m_MachineGunOffset = D3DXVECTOR3(m_CurrentWeapon.back()->GetActorImage()->GetImageCenter().x * 1.75f, m_CurrentWeapon.back()->GetActorImage()->GetImageCenter().y, 0.f);

	m_CurrentWeapon.push_back(new Cannon);
	if (!m_CurrentWeapon.back() || !m_CurrentWeapon.back()->Init(Device)) {
		return false;
	}
	m_CurrentWeapon.back()->SetRotation(90.f);
	m_CannonOffset = D3DXVECTOR3(-m_CurrentWeapon.back()->GetActorImage()->GetImageCenter().x * 4, m_CurrentWeapon.back()->GetActorImage()->GetImageCenter().y * 2, 0.f);
	
	m_ExplosionEffect = new ParticleClass;
	if (!m_ExplosionEffect || !m_ExplosionEffect->Init(Device, L"Particle/Explosion.png")) {
		return false;
	}

	if (GetPoolManager()) {
		GetPoolManager()->GetPoolObject("Projectile_Default", m_ProjectileObjects[EPT_DEFAULT], 20);
		GetPoolManager()->GetPoolObject("Projectile_Bomb", m_ProjectileObjects[EPT_BOMB], 20);
		GetPoolManager()->GetPoolObject("Projectile_Homing", m_ProjectileObjects[EPT_HOMINGMISSILE], 3);
		GetPoolManager()->GetPoolObject("Projectile_Nuclear", m_ProjectileObjects[EPT_NUCLEAR], 5);
	}
	return true;
}

void PlayerClass::SetupPlayerInput() {
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_NUMPAD0, IE_Pressed, std::bind(&PlayerClass::TestChangeMacineGun, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_1, IE_Pressed, std::bind(&PlayerClass::ChangeOtherToMachineGun, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_2, IE_Pressed, std::bind(&PlayerClass::ChangeOtherToCannon, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_3, IE_Pressed, std::bind(&PlayerClass::ChangeHoming, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_4, IE_Pressed, std::bind(&PlayerClass::ChangeNuclear, this));

	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_W, IE_Pressed, std::bind(&PlayerClass::Jump, this));

	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_SPACE, IE_Pressed, std::bind(&PlayerClass::PressedFire, this));
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_SPACE, IE_Released, std::bind(&PlayerClass::ReleasedFire, this));

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_D, "MoveRight", std::bind(&PlayerClass::MoveRight, this, std::placeholders::_1), 1.f);

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_NUMPAD4, "MachineGunRight", std::bind(&PlayerClass::WeaponRotation, this, std::placeholders::_1), -1);
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_NUMPAD6, "MachineGunRight", std::bind(&PlayerClass::WeaponRotation, this, std::placeholders::_1), 1);
}

void PlayerClass::Update(float DeltaTime, float ActorHeight) {
	PawnClass::Update(DeltaTime, ActorHeight);

	if (!GetStartMoveToLocation()) {
		if (GetFireState() && CanFire()) {
			if (m_WeaponType < m_CurrentWeapon.size()) {
				switch (m_WeaponType) {
				case EWT_MACHINEGUN:
					for (int i = 0; i < (m_bIsTriple ? m_MaxMachineGun : 1); i++) {
						auto Projectile = FireProjectile(m_CurrentWeapon[m_WeaponType]->GetOffset() - m_MachineGunOffset);
						if (Projectile) {
							float CurrentAngle = m_CurrentWeapon[m_WeaponType]->GetCurrentAngle() - (m_bIsTriple ? (m_WeaponDefaultAngle * (i - 1)) : 0);
							Projectile->SetMoveDirection(m_CurrentWeapon[m_WeaponType]->GetOffsetByAngle(CurrentAngle));
							Projectile->SetProjectileSeta(CurrentAngle);
						}
					}
					break;
				case EWT_CANNON:
					if (m_ProjectileType == EPT_HOMINGMISSILE && m_ProjectileObjects[EPT_HOMINGMISSILE].size() > 0) {
						InGameStage* Stage = (InGameStage*)SystemClass::GetInst()->GetActorManager()->GetStage();
						if (Stage) {
							auto Objects = Stage->GetActivatedObjects();
							for (auto& Object : Objects[EOT_ENEMY]) {
								if (Object.second && Object.second->GetActorIsActivated() && Object.second->GetActorImage()->GetPosition().x > GetActorImage()->GetPosition().x + GetActorImage()->GetImageCenter().x) {
									auto Projectile = FireProjectile(m_CurrentWeapon[m_WeaponType]->GetOffset() - m_CannonOffset + D3DXVECTOR3(0.f, m_CurrentWeapon[m_WeaponType]->GetActorImage()->GetImageCenter().y, 0.f));
									if (Projectile && Projectile->GetProjectileType() == EPT_HOMINGMISSILE) {
										Projectile->SetTargetActor(Object.second);
									}
								}
							}
						}
					}
					else if (m_ProjectileObjects[m_ProjectileType].size() > 0){
						auto Projectile = FireProjectile(m_CurrentWeapon[m_WeaponType]->GetOffset() - m_CannonOffset + D3DXVECTOR3(0.f, m_CurrentWeapon[m_WeaponType]->GetActorImage()->GetImageCenter().y, 0.f));
						if (Projectile) {
							float CurrentAngle = m_CurrentWeapon[m_WeaponType]->GetCurrentAngle();
							Projectile->SetMoveDirection(m_CurrentWeapon[m_WeaponType]->GetOffsetByAngle(CurrentAngle));
							Projectile->SetProjectileSeta(CurrentAngle);
						}
					}
				}
			}
		}
	}
	RePositionWeaponLocation();

	for (const auto& It : m_CurrentWeapon) {
		if (It && It->GetActorIsActivated()) {
			It->Update(DeltaTime, ActorHeight);
		}
	}
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	PawnClass::Render(Sprite);

	for (const auto& It : m_CurrentWeapon) {
		if (It && It->GetActorIsActivated()) {
			It->Render(Sprite);
		}
	}
}

void PlayerClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_OBSTACLE) {
			if (!GetStartMoveToLocation()) {
				SpawnExplosionEffect();
				ClearObject();
			}
		}
		else if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			if (!GetIsJumping() && !GetIsLanded()) {
				IsLanded();
			}
		}
	}
}

void PlayerClass::ClearObject() {
	GetActorImage()->SetPosition(D3DXVECTOR3(GetActorImage()->GetImageCenter().x * -1, GetActorImage()->GetPosition().y, 0.f));
	SetStartMoveToLocation(true);
}

void PlayerClass::PlayStartMoveToLocation(float Value) {
	D3DXVECTOR3 NewLocation, ToGoLocation(m_LocationIsHaveToGo, GetActorImage()->GetPosition().y, 0.f);
	D3DXVec3Lerp(&NewLocation, &GetActorImage()->GetPosition(), &ToGoLocation, Value);
	GetActorImage()->SetPosition(NewLocation);

	if (fabs(NewLocation.x - m_LocationIsHaveToGo) <= FLOATNearlyEqual) {
		SetStartMoveToLocation(false);
		m_SaveYLocation = GetActorImage()->GetPosition().y;
	}
}

void PlayerClass::TestChangeMacineGun() {
	m_bIsTriple = !m_bIsTriple;
}

inline void PlayerClass::RePositionWeaponLocation() {
	for (size_t i = 0; i < m_CurrentWeapon.size(); i++) {
		auto WeaponObject = m_CurrentWeapon[i]->GetActorImage();
		D3DXVECTOR3 Offset{ 0.f, 0.f, 0.f };
		
		switch (i) {
		case EWT_MACHINEGUN:
			Offset = m_MachineGunOffset;
			break;
		case EWT_CANNON:
			Offset = m_CannonOffset;
			break;
		}
		if (WeaponObject) {
			WeaponObject->SetPosition(GetActorImage()->GetPosition() - Offset);
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
			RePositionWeaponLocation();
		}
		m_bIsMoving = bool(Value);
	}
}

void PlayerClass::WeaponRotation(float Value) {
	if (m_WeaponType < m_CurrentWeapon.size() && !GetStartMoveToLocation() && Value != 0.f) {
		m_CurrentWeapon[m_WeaponType]->AddRotation(Value);
	}
}

void PlayerClass::ChangeOtherToMachineGun() {
	m_WeaponType = EWT_MACHINEGUN;
	m_ProjectileType = EPT_DEFAULT;
}

void PlayerClass::ChangeOtherToCannon() {
	m_WeaponType = EWT_CANNON;
	m_ProjectileType = EPT_BOMB;
}

void PlayerClass::ChangeHoming() {
	m_WeaponType = EWT_CANNON;
	m_ProjectileType = EPT_HOMINGMISSILE;
}

void PlayerClass::ChangeNuclear() {
	m_WeaponType = EWT_CANNON;
	m_ProjectileType = EPT_NUCLEAR;
}