#pragma once
#include "Pawn.h"
#include "ObjectPoolClass.h"
#include <cfloat>

const size_t MaxAddon = 4;

class PlayerClass : public Pawn {
private:
	size_t m_CurrentAddon;
	std::vector<D3DXVECTOR3> m_AddonPosition;
	std::vector<Pawn*> m_PlayerAddon;
	class InputClass* m_TempInputManager;
	bool m_bCanMove;

private:
	D3DXVECTOR3 GenerateRandomSpawnPoint();

	void MoveRight(float Value);
	void MoveForward(float Value);
	void FireProjectile(float Value);

public:
	PlayerClass(class ObjectPoolClass*);
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void TriggerCollisionEventByOtherActor(Actor* OtherActor) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

	void ResetAllObject();
	bool MoveToPosition(const D3DXVECTOR3&, const float DeltaTime);

public:
	void SetCanMove(bool b) { m_bCanMove = b; }

};