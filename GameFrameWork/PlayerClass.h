#pragma once
#include "Actor.h"

class PlayerClass : public Actor {
	using Actor::Actor;
private:

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

private:
	void MoveRight(float Value);
};

