#pragma once
#include "ObjectPoolClass.h"
#include "StageClass.h"
#include <vector>
#include <d3dx9.h>

class ActorClass {
private:
	ObjectPoolClass* m_PoolManager;
	std::vector<StageClass*> m_Stages;
	unsigned short m_CurrentStage;

private:
	class PlayerClass* m_PlayerClass;

public:
	ActorClass();
	~ActorClass();

public:
	bool Init(LPDIRECT3DDEVICE9);
	void Frame(float DeltaTime);
	void Render(LPD3DXSPRITE);

public:
	inline StageClass* GetStage() const { return m_Stages[m_CurrentStage]; }
	inline unsigned short GetCurrentStage() const { return m_CurrentStage; }
	void SetCurrentStage(unsigned short Value);
};