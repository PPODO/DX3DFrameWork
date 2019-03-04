#pragma once
#include "StageClass.h"

class IntroduceStage : public StageClass {
	using StageClass::StageClass;
private:
	class ButtonClass* m_BackButton = nullptr;

protected:

public:
	virtual ~IntroduceStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};