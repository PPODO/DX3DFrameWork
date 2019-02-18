#pragma once
#include "ActorClass.h"

class EnemyClass : public Actor {
private:

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc = nullptr, bool bUseCustomRect = false, RECT CustomRect = { -1 }) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

public:
	void MoveRight(float Value);

};