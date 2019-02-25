#pragma once
#include "Pawn.h"

class PlayerClass : public Pawn {
private:

public:
	PlayerClass();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput() override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

private:
	void MoveRight(float Value);
	void MoveForward(float Value);
};

