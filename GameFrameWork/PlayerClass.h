#pragma once
#include "ActorClass.h"

class PlayerClass : public Actor {
private:


public:
	PlayerClass();
	~PlayerClass();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE) override;
	virtual void Destroy() override;

private:
	void MoveRight(float Value);
};