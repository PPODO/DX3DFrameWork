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
	bool m_bUseTimer;
	class BackGroundUIClass* m_BackGround;
	std::chrono::duration<float> m_StageTime;

public:
	StageClass(std::vector<Actor*>&);
	virtual ~StageClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) = 0;
	virtual void Destroy() = 0;

};