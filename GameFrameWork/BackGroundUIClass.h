#pragma once
#include "Actor.h"
#include "TextureClass.h"

class BackGroundUIClass : public Actor {
private:
	TextureClass* m_SecondImage;

private:
	inline void BackgroundImageMoveProcessing(TextureClass*);

protected:
	bool bIsMovingScreen;
	float ImageMoveSpeed;

public:
	BackGroundUIClass() : m_SecondImage(nullptr), ImageMoveSpeed(5.f) {};

	virtual bool Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2 = nullptr);
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};

inline void BackGroundUIClass::BackgroundImageMoveProcessing(TextureClass* Image) {
	if (Image->GetPosition().x >= Image->GetRect().right * -1) {
		Image->AddXPosition(ImageMoveSpeed * -1);
	}
	else {
		Image->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
	}
}