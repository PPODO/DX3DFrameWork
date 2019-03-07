#pragma once
#include "Pawn.h"

class PlayerClass : public Pawn {
private:

private:
	void MoveRight(float Value);
	void FireProjectile(float Value);

	void Jump();

protected:
	virtual void OutOfScreen() override;

public:
	PlayerClass(class ObjectPoolClass*);
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual bool IsItOutOfScreen() override;
	virtual void CollisionEventByOtherActor(Actor*) override;

};