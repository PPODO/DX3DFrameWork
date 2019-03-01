#pragma once
#include "Actor.h"
#include <stack>
#include <vector>

class ProjectileClass : public Actor {
private:
	std::stack<ProjectileClass*>* m_ObjectList;
	std::vector<ProjectileClass*>* m_ActivatedList;

protected:
	D3DXVECTOR3 m_MoveDirection;

protected:
	virtual void ProjectileMoveProcessing() = 0;

public:
	ProjectileClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual bool CheckColliding();

public:
	void SpawnProjectile(const D3DXVECTOR3& Location, const D3DXVECTOR3& Direction, std::stack<ProjectileClass*>* ObjectList, std::vector<ProjectileClass*>* ActivatedList);
	void ClearObject();

};