#pragma once
#include <vector>
#include <d3d9.h>

class ActorClass {
private:
	std::vector<class Actor*> m_Actors;

public:
	ActorClass();
	~ActorClass();

public:
	bool Init(LPDIRECT3DDEVICE9);
	void Frame(float DeltaTime);

	inline std::vector<class Actor*>* GetActors() { return &m_Actors; }
};