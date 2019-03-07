#pragma once
#include "Pawn.h"
#include "TextureClass.h"
#include <stack>
#include <vector>
#include <functional>
#include <string>

enum ProjectileStyle { PS_DEFAULT, PS_LASER, PS_HOMING, PS_COUNT };

class ProjectileClass : public Actor {
private:
	Actor* m_Owner;

protected:
	std::string m_Name;
	D3DXVECTOR3 m_MoveDirection;

protected:
	virtual void OutOfScreen();
	virtual void ProjectileMoveProcessing() = 0;

public:
	ProjectileClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void CollisionEventByOtherActor(Actor*) override;

public:
	void SpawnProjectile(Actor* Owner, const D3DXVECTOR3& Location, const D3DXVECTOR3& Direction);
	void ClearObject();

	inline void PoolThisObject(std::stack<ProjectileClass*>&, std::vector<ProjectileClass*>&, std::vector<ProjectileClass*>::iterator&);

public:
	inline std::string GetName() const { return m_Name; }
	inline Actor* GetOwner() const { return m_Owner; }

};

inline void ProjectileClass::PoolThisObject(std::stack<ProjectileClass*>& Projectiles, std::vector<ProjectileClass*>& ActivatedProjectile, std::vector<class ProjectileClass*>::iterator& Iterator) {
	Projectiles.push((*Iterator));
	Iterator = ActivatedProjectile.erase(Iterator);
	ClearObject();
}