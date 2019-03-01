#pragma once
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <algorithm>
#include <d3d9.h>

class ObjectPoolClass {
private:
	LPDIRECT3DDEVICE9 m_Device;
	std::vector<std::pair<std::string, std::stack<class Actor*>>> m_Objects;

public:
	ObjectPoolClass(const LPDIRECT3DDEVICE9 Device);
	~ObjectPoolClass();
	
	template<typename T>
	void CreateObject(std::string ObjectName, size_t ObjectCount);

	template<typename T>
	void GetPoolObject(std::string ObjectName, std::stack<T*>& Actors, size_t ObjectCount);

	template<typename T>
	void ReleaseAll(std::string ObjectName, std::stack<T*>& Object, size_t ObjectSize);

	void Release(std::string ObjectName, class Actor* Object);
};

template<typename T>
inline void ObjectPoolClass::CreateObject(std::string ObjectName, size_t ObjectCount) {
	std::stack<class Actor*> Objects;
	for (size_t i = 0; i < ObjectCount; i++) {
		class Actor* Object = new T;
		Object->Init(m_Device);
		Objects.push(Object);
	}
	m_Objects.push_back(std::make_pair(ObjectName, Objects));
}

template<typename T>
inline void ObjectPoolClass::GetPoolObject(std::string ObjectName, std::stack<T*>& Actors, size_t ObjectCount) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	for (size_t i = 0; i < ObjectCount; i++) {
		Actors.push((T*)It->second.top());
		It->second.pop();
	}
}

template<typename T>
void ObjectPoolClass::ReleaseAll(std::string ObjectName, std::stack<T*>& Object, size_t ObjectSize) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<class Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	for (size_t i = 0; i < ObjectSize; i++) {
		It->second.push(Object.top());
		Object.pop();
	}
}