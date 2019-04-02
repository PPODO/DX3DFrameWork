#pragma once
#include "StageClass.h"
#include <stack>
#include <vector>
#include <utility>
#include <random>
#include <chrono>
#include <map>

enum EObjectType { EOT_ENEMY, EOT_OBSTACLE, EOT_COUNT };

class InGameStage : public StageClass {
private:
	class ObjectPoolClass* m_PoolManager;
	class PlayerClass* m_Player;

private:
	std::random_device m_RandomDevice;

protected:
	std::mt19937_64 m_RandomAlgorithm;
	
protected:
	std::vector<std::pair<LPCTSTR, LPCTSTR>> m_FileSrc;
	std::vector<std::pair<class BackGroundClass*, class BackGroundClass*>> m_BackGroundImages;

	std::vector<std::vector<std::stack<class Actor*>>> m_PoolObjects;
	std::vector<std::vector<std::pair<size_t, class Actor*>>> m_ActivatedObjects;
	std::vector<std::tuple<std::chrono::system_clock::time_point, std::chrono::duration<float>, float>> m_ObjectSpawnDelayTime;
	std::vector<size_t> m_ObjectMaxSpawnCount;

private:
	void UpdatePoolObjects(const float& DeltaTime, float ActorHeight);
	void RenderActivatedPoolObjects(const LPD3DXSPRITE& Sprite);
	void ReleasePoolObjects();

protected:
	virtual void PickUpObject() = 0;

protected:
	inline class PlayerClass* GetPlayerClass() { return m_Player; }
	inline class ObjectPoolClass* GetPoolClass() { return m_PoolManager; }

public:
	InGameStage(class ObjectPoolClass* OP, class PlayerClass* Player);
	virtual ~InGameStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void ChangeStageNotification() override;
	virtual void ReleaseStageNotification() override;

public:
	inline std::vector<std::vector<std::pair<size_t, class Actor*>>>& GetActivatedObjects() { return m_ActivatedObjects; }

};