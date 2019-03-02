#pragma once
#include "Actor.h"
#include "TextureClass.h"
#include <stack>
#include <vector>
#include <functional>
#include <string>

enum ProjectileStyle { PS_DEFAULT, PS_LASER, PS_HOMING, PS_COUNT };

class ProjectileClass : public Actor {
private:
	std::stack<ProjectileClass*>* m_ObjectList;
	std::vector<ProjectileClass*>* m_ActivatedList;

protected:
	std::string m_Name;

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
	virtual bool CheckColliding(std::vector<class ProjectileClass*>::iterator&, const std::function<bool()>&);

	inline bool IsCrash(const Actor*&);

public:
	void SpawnProjectile(const D3DXVECTOR3& Location, const D3DXVECTOR3& Direction, std::stack<ProjectileClass*>* ObjectList, std::vector<ProjectileClass*>* ActivatedList);
	void ClearObject();

	inline void PoolThisObject(std::vector<class ProjectileClass*>::iterator&);

public:
	inline std::string GetName() const { return m_Name; }

};

inline void ProjectileClass::PoolThisObject(std::vector<class ProjectileClass*>::iterator& Iterator) {
	if (m_ObjectList && m_ActivatedList && Iterator != m_ActivatedList->cend()) {
		m_ObjectList->push(*Iterator);
		Iterator = m_ActivatedList->erase(Iterator);
		ClearObject();
	}
}

inline bool ProjectileClass::IsCrash(const Actor*& Object) {
	D3DXVECTOR3 ObjectPosition = Object->GetTexture()->GetPosition();
	D3DXVECTOR3 ObjectCenter = Object->GetTexture()->GetImageCenter();

	if (m_Texture->GetPosition().x - m_Texture->GetImageCenter().x < ObjectPosition.x + ObjectCenter.x && m_Texture->GetPosition().x + m_Texture->GetImageCenter().x > ObjectPosition.x - ObjectCenter.x && m_Texture->GetPosition().y - m_Texture->GetImageCenter().y < ObjectPosition.y + ObjectCenter.y && m_Texture->GetPosition().y + m_Texture->GetImageCenter().y > ObjectPosition.y - ObjectCenter.y) {
		return true;
	}
	return false;
}