#pragma once
#include "StageClass.h"
#include <vector>

class MainStage : public StageClass {
private:
	std::vector<LPCTSTR> m_FileSrc;
	std::vector<class ButtonClass*> m_Buttons;
	class WidgetClass* m_WidgetClass;

public:
	MainStage();
	virtual ~MainStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);

};