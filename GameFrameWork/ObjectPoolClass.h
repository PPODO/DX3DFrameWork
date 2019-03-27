#pragma once
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <algorithm>
#include <map>
#include <d3d9.h>

class ObjectPoolClass {
private:
	LPDIRECT3DDEVICE9 m_Device;
	std::vector<std::pair<std::string, std::stack<class Actor*>>> m_Objects;
	std::vector<std::pair<std::string, size_t>> m_ObjectTypeNames;
	std::map<std::string, short> m_ObjectNames;

private:
	inline void GetTypeNameByObjectName(const std::string& ObjectName);

public:
	ObjectPoolClass(const LPDIRECT3DDEVICE9 Device);
	~ObjectPoolClass();
	
	template<typename T>
	void CreateObject(std::string ObjectName, size_t ObjectCount, short);

	template<typename T>
	void GetPoolObject(std::string ObjectName, std::stack<T*>& Actors, size_t ObjectCount);

	template<typename T>
	void ReleaseAll(std::string ObjectName, std::stack<T*>& Object, size_t ObjectSize);

	inline short GetKeyByObjectName(const std::string& ObjectName) {
		auto Object = m_ObjectNames.find(ObjectName);
		if (Object != m_ObjectNames.cend()) {
			return Object->second;
		}
		return -1;
	}

	inline int GetObjectTypeCountByName(const std::string& Name) {
		auto It = std::find_if(m_ObjectTypeNames.begin(), m_ObjectTypeNames.end(), [&](const std::pair<std::string, size_t>& Param) {if (Param.first.compare(Name)) { return true; } return false; });
		if (It != m_ObjectTypeNames.cend()) {
			return It->second;
		}
		return -1;
	}
};

template<typename T>
inline void ObjectPoolClass::CreateObject(std::string ObjectName, size_t ObjectCount, short Key) {
	std::stack<class Actor*> Objects;
	for (size_t i = 0; i < ObjectCount; i++) {
		class Actor* Object = new T;
		Object->Init(m_Device);
		Objects.push(Object);
	}
	m_Objects.push_back(std::make_pair(ObjectName, Objects));
	m_ObjectNames.insert(std::make_pair(ObjectName, Key));
	GetTypeNameByObjectName(ObjectName);
}

template<typename T>
inline void ObjectPoolClass::GetPoolObject(std::string ObjectName, std::stack<T*>& Actors, size_t ObjectCount) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	if (It != m_Objects.end()) {
		if (It->second.top() > 0) {
			for (size_t i = 0; i < ObjectCount; i++) {
				Actors.push((T*)It->second.top());
				It->second.pop();
			}
		}
	}
}

template<typename T>
void ObjectPoolClass::ReleaseAll(std::string ObjectName, std::stack<T*>& Object, size_t ObjectSize) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<class Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	if (It != m_Objects.end()) {
		for (size_t i = 0; i < ObjectSize; i++) {
			It->second.push(Object.top());
			Object.pop();
		}
	}
}

inline void ObjectPoolClass::GetTypeNameByObjectName(const std::string& ObjectName) {
	size_t Distance = 0;
	for (auto Iterator = ObjectName.begin(); Iterator != ObjectName.end(); Iterator++) {
		if ((*Iterator) == '_') {
			Distance = Iterator - ObjectName.begin();
			break;
		}
	}
	if (Distance > 0) {
		auto It = std::find_if(m_ObjectTypeNames.begin(), m_ObjectTypeNames.end(), [&](const std::pair<std::string, size_t>& Param) {if (Param.first.compare(ObjectName.substr(0, Distance)) == 0) { return true; } return false; });
		if (It != m_ObjectTypeNames.cend()) {
			It->second++;
		}
		else {
			m_ObjectTypeNames.push_back(std::make_pair(ObjectName.substr(0, Distance), 1));
		}
	}
}