#pragma once
#include "StageClass.h"
#include "WidgetClass.h"
#include <vector>
#include <functional>

enum EButtonStage { EBS_Pressed, EBS_Released };

class ButtonClass : public WidgetClass {
private:
	std::vector<LPCTSTR> m_FileSrc;

private:
	void ChangeButtonState();

public:
	ButtonClass(LPCTSTR);
	virtual ~ButtonClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc = std::vector<LPCTSTR>());

public:
	void BindMouseAction(const size_t& EST, const std::function<void()>& Func);

};