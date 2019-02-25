#pragma once
#include "StageClass.h"

class InGameStage : public StageClass {
private:
	std::vector<class EnemyClass*> m_Enemy;

public:
	InGameStage(std::vector<class Actor*>& Actors, class ObjectPoolClass* ObjectPool);
	virtual ~InGameStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};

