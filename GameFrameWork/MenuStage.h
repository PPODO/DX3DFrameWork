#pragma once
#include "StageClass.h"
#include <vector>

class MenuStage : public StageClass {
	using StageClass::StageClass;
	enum { BF_START, BF_INTRODUCE, BF_METHOD, BF_SCORE, BF_CREDIT, BF_SHUTDOWN, BF_COUNT };
private:
	class TextClass* m_Text;
	std::vector<class ButtonClass*> m_Button;

public:
	~MenuStage();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ReleaseForChangingStage() override;

};