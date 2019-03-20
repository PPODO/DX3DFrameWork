#pragma once
#include "WidgetClass.h"
#include <vector>

enum EButtonStage { EBS_Pressed, EBS_Released };

class ButtonClass : public WidgetClass {
private:
	std::vector<LPCTSTR> m_FileSrc;

private:
	void ChangeButtonState();

public:
	ButtonClass(LPCTSTR, LPCTSTR);
	virtual ~ButtonClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc);

};