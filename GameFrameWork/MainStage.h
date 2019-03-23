#pragma once
#include "StageClass.h"
#include <vector>

class MainStage : public StageClass {
	enum EButtonType { EBT_MAIN, EBT_INTRO, EBT_METHOD, EBT_SCORE, EBT_CREDIT };
private:
	std::vector<LPCTSTR> m_FileSrc;
	std::vector<class ButtonClass*> m_Buttons;
	class WidgetClass* m_WidgetClass;

public:
	MainStage();
	virtual ~MainStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void ChangeStageNotification() override;
	virtual void ReleaseStageNotification() override;

};