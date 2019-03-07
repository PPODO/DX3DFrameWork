#include "PlayerClass.h"
#include "ObjectPoolClass.h"
#include "SystemClass.h"
#include "InputClass.h"

PlayerClass::PlayerClass(class ObjectPoolClass* OP) {
	m_Projectiles.resize(PS_COUNT);

	m_Collision = CT_PLAYER;
	m_PoolManager = OP;
	m_XMoveSpeed = 5.f;
	m_YMoveSpeed = 5.f;
	m_MaxActiveProjectile = 10;
	m_FireDelay = std::chrono::duration<float>(0.25f);
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player/Player.png");

	m_PoolManager->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], m_MaxActiveProjectile);

	return true;
}

void PlayerClass::SetupPlayerInput() {
	InputClass* Input = SystemClass::GetInst()->GetInputManager();

	if (Input) {
		Input->BindActionDelegate(VK_UP, IE_Pressed, std::bind(&PlayerClass::Jump, this));

		Input->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
		Input->BindAxisDelegate(VK_LEFT, std::bind(&PlayerClass::MoveRight, this, -1.f));
		Input->BindAxisDelegate(VK_SPACE, std::bind(&PlayerClass::FireProjectile, this, 0.f));
	}
}

void PlayerClass::Update(float DeltaTime) {
	Pawn::Update(DeltaTime);

}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

}

void PlayerClass::Destroy() {
	Pawn::Destroy();
}

bool PlayerClass::IsItOutOfScreen() {
	if (m_Texture->GetPosition().x - m_Texture->GetImageCenter().x < GetWindowSize().left || m_Texture->GetPosition().x + m_Texture->GetImageCenter().x > GetWindowSize().right || m_Texture->GetPosition().y - m_Texture->GetImageCenter().y < GetWindowSize().top || m_Texture->GetPosition().y + m_Texture->GetImageCenter().y > GetWindowSize().bottom) {
		return true;
	}
	return false;
}

void PlayerClass::OutOfScreen() {

}

void PlayerClass::CollisionEventByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_Collision) {
		if (OtherActor->GetActorCollisionType() == CT_ENEMY) {

		}
		else if (OtherActor->GetActorCollisionType() == CT_PROJECTILE) {
			ProjectileClass* Projectile = (ProjectileClass*)OtherActor;
			if (Projectile && Projectile->GetOwner()->GetActorCollisionType() != m_Collision) {
				MessageBeep(MB_OK);
			}
		}
		else if (OtherActor->GetActorCollisionType() == CT_ALLBLOCK) {
			SetLanded(true);
		}
	}
}

void PlayerClass::MoveRight(float Value) {
	m_Texture->AddXPosition(m_XMoveSpeed * Value);
}

void PlayerClass::Jump() {
	SetJumping(true);
}

void PlayerClass::FireProjectile(float Value) {
	SpawnProjectile(D3DXVECTOR3(1.f, 0.f, 0.f));
}