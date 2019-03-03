#pragma once
#include "Pawn.h"
#include <string>
#include <random>
#include <stack>
#include <vector>

enum ENEMYSTYLE { EN_DEFAULT, EN_RUSH, EN_SPLIT, EN_FLIGHT, EnemyStyleCount };

class EnemyClass : public Pawn {
private:
	static std::random_device m_RandomDevice;
	static std::mt19937_64 m_RandomAlgorithm;
	static std::uniform_real_distribution<float> m_Random;

	static std::vector<std::stack<EnemyClass*>>* m_ObjectList;
	static std::vector<EnemyClass*>* m_ActivatedList;

protected:
	static TextureClass* m_Target;

private:
	std::chrono::duration<float> m_SpawnTime;
	std::chrono::system_clock::time_point m_LastSpawnTime;

protected:
	std::string m_Name;

	float m_MinSpawnDelay;
	float m_MaxSpawnDelay;

protected:
	virtual bool IsItOutOfScreen() override;
	virtual void OutOfScreen() override;
	virtual void EnemyMoveProcessing() = 0;
	virtual void ClearObject();

public:
	EnemyClass();
	virtual ~EnemyClass() override;

	void PoolThisObject(std::vector<EnemyClass*>::iterator&);

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void TriggerCollisionEventByOtherActor(Actor*) override;
	virtual void SpawnObject();

public:
	void SetPoolingList(std::vector<std::stack<EnemyClass*>>* ObjectList, std::vector<EnemyClass*>* ActivatedList) { ObjectList ? m_ObjectList = ObjectList : m_ObjectList = nullptr; ActivatedList ? m_ActivatedList = ActivatedList : m_ActivatedList = nullptr; };
	void SetTarget(TextureClass* Target) { m_Target = Target; };

	inline std::string GetName() const { return m_Name; }

};