#pragma once
#include <d3dx9.h>

enum EStageType { EST_MAIN };

class StageClass {
private:


public:
	StageClass();
	virtual ~StageClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void ChangeStageNotification() = 0;
	virtual void ReleaseStageNotification() = 0;

};