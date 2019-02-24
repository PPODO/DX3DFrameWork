#pragma once
#include "Actor.h"
#include <functional>

class ButtonClass : public Actor {
private:
	bool m_bChangeButtonState;
	unsigned short m_CurrentStage;

private:
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void ChangeButtonState();

public:
	ButtonClass(unsigned short);

	bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, std::function<void()> Work);
	virtual void Render(LPD3DXSPRITE Sprite) override;
};

