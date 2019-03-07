#pragma once
#include "Actor.h"
#include "TextureClass.h"
#include <vector>

class BackGroundClass : public Actor {
private:
	using Actor::GetTexture;
private:
	std::vector<Actor*> m_BackGroundImages;
	D3DXVECTOR3 m_ImagePos;

private:
	bool Init(LPDIRECT3DDEVICE9 Device) { return true; }
	inline void BackgroundImageMoveProcessing(TextureClass*);

protected:
	bool m_bIsMovingScreen;
	float m_ImageMoveSpeed;
	short m_MoveDirection;

public:
	BackGroundClass();

	template<typename T, typename ...Argc>
	bool Init(LPDIRECT3DDEVICE9 Device, T, Argc...);

	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

public:
	inline TextureClass* GetActorTexture(size_t Index) { if (Index < m_BackGroundImages.size()) { return m_BackGroundImages[Index]->GetTexture(); } }

	void SetCollisionTypeAndBindEvent(CollisionType CT);
	void SetIsMove(bool b) { m_bIsMovingScreen = b; }
	void SetBackGroundMovement(bool IsMoving, float MoveSpeed, short MoveDirection) { m_bIsMovingScreen = IsMoving, m_ImageMoveSpeed = MoveSpeed, m_MoveDirection = MoveDirection; }
	void SetPosition(const D3DXVECTOR3& Pos) {
		m_ImagePos = Pos;
		for (auto& It : m_BackGroundImages) {
			if (It && It->GetTexture()) {
				It->GetTexture()->AddPosition(m_ImagePos);
			}
		}
	}

};

template<typename T, typename ...Argc>
inline bool BackGroundClass::Init(LPDIRECT3DDEVICE9 Device, T Count, Argc... argc) {
	Actor* NewActor = new Actor;
	if (!NewActor || !NewActor->Init(Device, (LPCTSTR)Count)) {
		return false;
	}
	TextureClass* Texture = NewActor->GetTexture();
	if (!Texture && !Texture->m_Texture) {
		return false;
	}
	Texture->SetPosition(D3DXVECTOR3(FLOAT((Texture->GetRect().right / 2) + (Texture->GetRect().right * m_BackGroundImages.size())), Texture->GetImageCenter().y, 0.f));

	m_BackGroundImages.push_back(NewActor);
	Init(Device, argc...);
	return true;
}

inline void BackGroundClass::BackgroundImageMoveProcessing(TextureClass* Image) {
	if ((LONG)Image->GetPosition().x <= Image->GetRect().right * -1) {
		Image->SetPosition(D3DXVECTOR3((FLOAT)Image->GetRect().right, Image->GetImageCenter().y, 0.f) + m_ImagePos);
	}
	Image->AddXPosition(m_ImageMoveSpeed * m_MoveDirection);
}