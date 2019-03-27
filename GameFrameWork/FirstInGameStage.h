#pragma once
#include "InGameStage.h"
#include <vector>

class FirstInGameStage : public InGameStage {
	enum EBackGroundType { EBGT_BACKGROUND, EBGT_BACKGROUND2, EBGT_GROUND };
private:
	D3DXVECTOR3 m_EnemySpawnLocation;
	D3DXVECTOR3 m_ObstacleSpawnLocation;

protected:
	virtual void PickUpObject() override;

public:
	FirstInGameStage(class ObjectPoolClass* OP, class PlayerClass* Player);
	virtual ~FirstInGameStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void ChangeStageNotification() override;
	virtual void ReleaseStageNotification() override;

};