#pragma once
#include "ObjectPoolClass.h"
#include <vector>
#include <d3dx9.h>

class ActorClass {
private:
	ObjectPoolClass* m_PoolManager;
	std::vector<class StageClass*> m_Stages;
	std::vector<std::string> m_EnemyName;

	unsigned short m_CurrentStage;

public:
	ActorClass();
	~ActorClass();

public:
	bool Init(LPDIRECT3DDEVICE9);
	void Frame(float DeltaTime);
	void Render(LPD3DXSPRITE);

public:
	inline unsigned short GetCurrentStage() const { return m_CurrentStage; }
	void SetCurrentStage(unsigned short Value);
};