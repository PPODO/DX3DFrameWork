#pragma once
#include "Actor.h"
#include "TextureClass.h"

class BackGroundUIClass : public Actor {
private:
	TextureClass* m_SecondImage;

protected:
	bool bIsMovingScreen;
	float ImageMoveSpeed;

public:
	BackGroundUIClass() : m_SecondImage(nullptr), ImageMoveSpeed(10.f) {};

	virtual bool Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2 = nullptr);
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void Destroy() override;

};

