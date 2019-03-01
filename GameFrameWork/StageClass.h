#pragma once
#include "Actor.h"
#include <d3dx9.h>
#include <vector>
#include <chrono>

enum StageState { SS_LOADING = -1, SS_MENU, SS_PHASE1 };

class StageClass : public Actor {
private:
	std::chrono::system_clock::time_point m_StartTime;

protected:
	class BackGroundUIClass* m_BackGround;
	std::chrono::duration<float> m_StageTime;
	bool m_bUseTimer, m_bIsStop, m_bNotificationForStop;

protected:
	void StartTimer() {
		m_StartTime = std::chrono::system_clock::now();
	}
	virtual void ReleaseForChangingStage();

public:
	StageClass(std::vector<StageClass*>&);
	virtual ~StageClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) = 0;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ChangeStageNotification();

};