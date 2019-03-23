#pragma once
#include "PawnClass.h"

#define FLOATNearlyEqual 2.5f

class PlayerClass : public PawnClass {
private:
	class MachineGun* m_MachineGun;
	D3DXVECTOR3 m_MachineGunOffset;

	std::vector<std::stack<ProjectileClass*>> m_ProjectileObjects;
	std::vector<ProjectileClass*> m_ActivatedProjectileObjects;

private:
	bool m_bIsMoving;
	RECT m_MaximumMovement;

private:
	void Jump();
	void PressedFire();
	void ReleasedFire();

	void MoveRight(float Value);
	void MachineGunRotation(float Value);

private:
	inline void SetMachineGunLocation();

protected:
	virtual void SetupPlayerInput() override;
	virtual void PlayStartMoveToLocation(float Value = 0.05f) override;
	virtual ProjectileClass* FireProjectile(const D3DXVECTOR3& Offset = { 0.f, 0.f, 0.f }) override;
	virtual void CalculateProjectile(const float& DeltaTime) override;

public:
	PlayerClass(class ObjectPoolClass* OP);
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

public:
	inline bool GetIsMoving() const { return m_bIsMoving; }

};