#pragma once
#include "Pawn.h"
#include <string>
#include <random>

enum ENEMYSTYLE { ES_DEFAULT, EnemyStyleCount };

class EnemyClass : public Pawn {
private:
	static std::random_device m_RandomDevice;
	static std::mt19937_64 m_RandomAlgorithm;
	static std::uniform_real_distribution<float> m_Random;

private:
	std::string m_Name;

	bool m_bIsSpawn;
	std::chrono::system_clock::time_point m_LastSpawnTime;
	std::chrono::duration<float> m_SpawnDelay;

protected:
	float m_MinSpawnDelay, m_MaxSpawnDelay;
	D3DXVECTOR3 m_MoveDirection;

protected:
	virtual void ProcessingEnemyMovement();
	virtual void OutOfScreen() override;

public:
	EnemyClass();
	virtual ~EnemyClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void CollisionEventByOtherActor(Actor*) override;
	virtual void ClearObject();
	virtual void SpawnObject();

	void PoolThisObject(std::stack<EnemyClass*>& EnemyList, std::vector<EnemyClass*>& ActivatedEnemy, std::vector<EnemyClass*>::iterator&);

public:
	void SetName(const std::string& str) { m_Name = str; }

	inline std::string GetName() const { return m_Name; }

};