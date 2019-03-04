#pragma once
#include "Actor.h"
#include "TextureClass.h"

class BackGroundClass : public Actor {
private:
	TextureClass* m_SecondImage;

private:
	inline void BackgroundImageMoveProcessing(TextureClass*);

protected:
	bool bIsMovingScreen;
	float ImageMoveSpeed;

public:
	BackGroundClass() : m_SecondImage(nullptr), ImageMoveSpeed(5.f) {};

	virtual bool Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2 = nullptr);
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};

inline void BackGroundClass::BackgroundImageMoveProcessing(TextureClass* Image) {
	if ((LONG)Image->GetPosition().x <= Image->GetRect().right * -1) {
		Image->SetPosition(D3DXVECTOR3((FLOAT)Image->GetRect().right, 0.f, 0.f));
	}
	Image->AddXPosition(ImageMoveSpeed * -1);
}