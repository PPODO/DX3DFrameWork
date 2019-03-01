#pragma once
#include "Pawn.h"
#include <string>
#include <random>

class EnemyClass : public Pawn {
private:
	static std::random_device m_RandomDevice;
	static std::mt19937_64 m_RandomAlgorithm;
	static std::uniform_real_distribution<float> m_Random;

	std::chrono::duration<float> m_SpawnTime;
	std::chrono::system_clock::time_point m_LastSpawnTime;

	bool m_bOutOfScreen;

protected:
	std::string m_Name;
	float m_MinSpawnDelay;
	float m_MaxSpawnDelay;

protected:
	virtual void EnemyMoveProcessing() = 0;
	virtual inline bool CheckOutOfScreen();

	inline bool CheckSpawnTime() const { return m_SpawnTime < std::chrono::system_clock::now() - m_LastSpawnTime; }

public:
	EnemyClass();
	~EnemyClass();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ClearObject();

	void SpawnAtLocation();

public:
	std::string GetName() const { return m_Name; }
	void SetOutOfScreen(bool b) { m_bOutOfScreen = b; }
	bool GetOutOfScreen() const { return m_bOutOfScreen; }

};

inline bool EnemyClass::CheckOutOfScreen() {
	if (LONG(m_Texture->GetPosition().x + m_Texture->GetImageCenter().x) < GetWindowSize().left) {
		m_bOutOfScreen = true;
	}
	return m_bOutOfScreen;
}