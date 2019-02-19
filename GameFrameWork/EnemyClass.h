#pragma once
#include "ActorClass.h"

class EnemyClass : public Actor {
private:

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) override;
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};