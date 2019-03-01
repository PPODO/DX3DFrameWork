#pragma once
#include "Pawn.h"
#include "ObjectPoolClass.h"

class PlayerClass : public Pawn {
private:
	class ObjectPoolClass* m_TempPoolManager;
	class InputClass* m_TempInputManager;

private:
	void MoveRight(float Value);
	void MoveForward(float Value);
	void FireProjectile(float Value);

public:
	PlayerClass(class ObjectPoolClass*);
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};