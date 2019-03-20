#pragma once
#include "Actor.h"

enum EObstacleType { EOT_HOLE };

class ObstacleClass : public Actor {
private:


protected:
	virtual void ObstacleMovementProcessing() = 0;

public:
	ObstacleClass();
	virtual ~ObstacleClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

};