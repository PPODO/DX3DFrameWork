#pragma once
#include "ObjectPoolClass.h"
#include <vector>
#include <d3d9.h>

class ActorClass {
private:
	ObjectPoolClass* m_PoolManager;
	std::vector<class Actor*> m_Stages;
	unsigned short m_CurrentStage;

public:
	ActorClass();
	~ActorClass();

public:
	bool Init(LPDIRECT3DDEVICE9);
	void Frame(float DeltaTime);

	inline class Actor* GetCurrentActor() { return m_Stages[m_CurrentStage]; }
	inline unsigned short GetCurrentStage() const { return m_CurrentStage; }
	inline void SetCurrentStage(unsigned short Value) { m_CurrentStage = Value; }
};