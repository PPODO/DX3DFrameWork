#pragma once
#include "Actor.h"
#include <d3dx9.h>
#include <vector>

enum StageState { SS_LOADING = -1, SS_MENU, SS_PHASE1 };

class StageClass : public Actor {
protected:
	class BackGroundUIClass* m_BackGround;

public:
	StageClass(std::vector<Actor*>&);
	virtual ~StageClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Render(LPD3DXSPRITE Sprite) = 0;
	virtual void Destroy() = 0;

};