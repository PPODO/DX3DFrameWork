#pragma once
#include "ObjectPoolClass.h"
#include <vector>
#include <d3dx9.h>

class ActorClass {
private:
	ObjectPoolClass* m_PoolManager;
	std::vector<class Actor*> m_Stages;
	class PlayerClass* m_Player;

	unsigned short m_CurrentStage;

public:
	ActorClass();
	~ActorClass();

public:
	bool Init(LPDIRECT3DDEVICE9);
	void Frame(float DeltaTime);
	void Render(LPD3DXSPRITE);

public:
	inline class Actor* GetCurrentActor() { return m_Stages[m_CurrentStage]; }
	inline unsigned short GetCurrentStage() const { return m_CurrentStage; }
	inline void SetCurrentStage(unsigned short Value) { m_CurrentStage = Value; }
};