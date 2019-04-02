#pragma once
#include "Actor.h"
#include <iostream>

enum EObstacleType { EOT_HOLE, EObstacleType_COUNT };

class ObstacleClass : public Actor {
private:

protected:
	virtual void ObstacleMovementProcessing() = 0;

public:
	ObstacleClass();
	virtual ~ObstacleClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual bool IsItOutOfScreen() override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	virtual void ClearObject() override;

};