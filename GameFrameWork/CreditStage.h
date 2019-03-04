#pragma once
#include "StageClass.h"
class CreditStage : public StageClass {
	using StageClass::StageClass;
private:
	class ButtonClass* m_BackButton;

public:
	virtual ~CreditStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
};