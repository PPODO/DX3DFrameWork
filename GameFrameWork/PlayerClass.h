#pragma once
#include "PawnClass.h"
#include "Weapon.h"

#define FLOATNearlyEqual 2.5f

class PlayerClass : public PawnClass {
private:
	std::vector<class Weapon*> m_CurrentWeapon;
	float m_WeaponDefaultAngle;
	EWEAPONTYPE m_WeaponType;

	D3DXVECTOR3 m_CannonOffset;
	D3DXVECTOR3 m_MachineGunOffset;
	size_t m_MaxMachineGun;
	bool m_bIsTriple;

	void TestChangeMacineGun();

private:
	inline void RePositionWeaponLocation();

private:
	bool m_bIsMoving;
	RECT m_MaximumMovement;

private:
	void Jump();
	void PressedFire();
	void ReleasedFire();

	void MoveRight(float Value);
	void WeaponRotation(float Value);

	void ChangeOtherToMachineGun();
	void ChangeOtherToCannon();

protected:
	virtual void SetupPlayerInput() override;
	virtual void PlayStartMoveToLocation(float Value = 0.05f) override;

public:
	PlayerClass(class ObjectPoolClass* OP);
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void ClearObject() override;

public:
	inline bool GetIsMoving() const { return m_bIsMoving; }

};