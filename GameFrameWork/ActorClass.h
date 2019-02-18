#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include <d3dx9.h>

class TextureClass;
class EnemyClass;

class Actor {
protected:
	TextureClass* m_Image;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc = nullptr, bool bUseCustomRect = false, RECT CustomRect = { -1 });
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Destroy();

public:
	inline TextureClass* GetTexture() const { return m_Image; }

};

class ActorClass {
private:
	EnemyClass* Enemy;

private:
	std::vector<Actor*> m_Actors;
	std::thread m_LoadingThread;

private:

public:
	bool Init(LPDIRECT3DDEVICE9 Device);
	void Frame();
	void Shutdown();

public:
	inline std::vector<Actor*> GetActors() const { return m_Actors; }
};